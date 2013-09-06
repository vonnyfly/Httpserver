#include "libcgi.h"

HANDLE hOut,hIn;
FILE* fenv;

void hexdump(const void *_data, unsigned len)
{
    unsigned char *data = (unsigned char *)_data;
    unsigned count;

    for (count = 0; count < len; count++) {
        if ((count & 15) == 0)
            write_data(L"<!-- %04x:", count);
        write_data(L" %02x", *data);
        data++;
        if ((count & 15) == 15)
            write_data(L"-->\n");
    }
    if ((count & 15) != 0)
        write_data(L"-->\n");

    // print view char
    data = (unsigned char *)_data;
    for (count = 0; count < len; count++) {
        if ((count & 15) == 0)
            write_data(L"<!--      ", count);
        write_data(L" %2c",
                   (*data < 32) || (*data > 126) ? '.' : *data);
        data++;
        if ((count & 15) == 15)
            write_data(L"-->\n");
    }
    if ((count & 15) != 0)
        write_data(L"-->\n");
}

char *str_replace(char *orig, char *rep, char *with) {
    char *result; // the return string
    char *ins;    // the next insert point
    char *tmp;    // varies
    int len_rep;  // length of rep
    int len_with; // length of with
    int len_front; // distance between rep and end of last rep
    int count;    // number of replacements

    if (!orig)
        return NULL;
    if (!rep)
        rep = "";
    len_rep = strlen(rep);
    if (!with)
        with = "";
    len_with = strlen(with);

    for (count = 0; tmp = strstr(ins, rep); ++count) {
        ins = tmp + len_rep;
    }

    // first time through the loop, all the variable are set correctly
    // from here on,
    //    tmp points to the end of the result string
    //    ins points to the next occurrence of rep in orig
    //    orig points to the remainder of orig after "end of rep"
    tmp = result = (char*)malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    if (!result)
        return NULL;

    while (count--) {
        ins = strstr(orig, rep);
        len_front = ins - orig;
        tmp = strncpy(tmp, orig, len_front) + len_front;
        tmp = strcpy(tmp, with) + len_with;
        orig += len_front + len_rep; // move to next "end of rep"
    }
    strcpy(tmp, orig);
    return result;
}

char_t* getCgiData(char_t* buf,unsigned int buf_size,int *nread)
{
    ReadFile(hIn,buf,buf_size,(LPDWORD)nread,NULL);
    return buf;
}

char_t* getCgiDataWithMalloc(int *len)
{
    char *ptr=NULL,*ppos=NULL;
    int nRead,nLeft;
    nLeft = *len = GetFileSize(hIn,NULL);
    DBG(L"file size =%d\n",*len);

    if(len > 0)
        ppos = ptr = (char*)malloc(*len);
    else
        return NULL;
    do {
        ReadFile(hIn, ppos, nLeft,(LPDWORD)&nRead,NULL);

        ppos +=nRead;
        nLeft -= nRead;
        DBG(L"has read %d,left %d\n",nRead,nLeft);
        //write_data(L"nRead=%d",nRead);

        if(nRead <=0 || nLeft <= 0)
            break;
    } while(1);

    return (char_t*)ptr;
}

void freeCgiDataWithMalloc(char_t *pData)
{
    if(pData)
        free(pData);
}
//ok:return value;else return null
char_t* gogetenv(char_t *key,char_t *value)
{
    char_t* pos=NULL;
    char_t buf[256];
    int bSuccess;
    int len;

    if (fenv) {
        fseek(fenv,0,SEEK_SET);
        while (fgetws((wchar_t*)buf, 256, fenv)) { // Key & Value
            pos = wcschr(buf, L'=');
            if (!pos) {
                continue;
            }
            len = buf + wcslen(buf) - 1 - pos;
            //write_data(L"%s,len=%d\n",buf,len);
            *pos = L'\0'; // Erase the '='
            if (0 != _wcsicmp(buf, key)) { // Key
                continue;
            }
            pos++; // Point to the value
            bSuccess = SUCCEEDED(StringCchCopy(value,len,pos));
            //hexdump(value,len);
            //write_data(L"value=%s\n",value);
            if(bSuccess) {
                return value;
            }
        }
    }
    return NULL;
}
wchar_t STD_HEADER[] = L"Connection: close\r\n"\
                       L"Server: goagent\r\n"\
                       L"Cache-Control: no-store, no-cache, must-revalidate, pre-check=0, post-check=0, max-age=0\r\n" \
                       L"Pragma: no-cache\r\n" \
                       L"Expires: Mon, 3 Jan 2017 12:34:56 GMT\r\n";

int init(int argc, _TCHAR* argv[])
{
    //char_t buf[256];
    hOut = CreateFile(argv[argc-1], GENERIC_WRITE, FILE_SHARE_WRITE, NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL, NULL);
    hIn = CreateFile(argv[argc-2], GENERIC_READ, FILE_SHARE_READ, NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL, NULL);
    if(hOut == INVALID_HANDLE_VALUE || hIn == INVALID_HANDLE_VALUE)
        return -1;
    write_data(L"Content-type:text/html;charset=gbk\r\n");
    write_data(STD_HEADER);
    write_data(L"\r\n");
    fenv = _wfopen(argv[argc-3], L"rb");
    return 1;
}
void destroy()
{
    CloseHandle(hOut);
    CloseHandle(hIn);
    fclose(fenv);
}

int write_data(const char_t *fmt,...)
{
    va_list		args;
    char_t		buf[256];
    int need_write,written;

    va_start(args, fmt);
    need_write = vswprintf(buf,fmt,args);
    va_end(args);
    WriteFile(hOut,buf,2*need_write,(LPDWORD)&written,NULL) ;
#ifdef D_DEBUG
    wprintf(L"%s| sizeof: %d need write:%d written: %d wcslen: %d\n",buf,sizeof(buf),need_write,written,wcslen(buf));
#endif
    return written;
}

char* UnicodeToAnsi( const wchar_t* szStr )
{
    int nLen = WideCharToMultiByte( CP_ACP, 0, szStr, -1, NULL, 0, NULL, NULL );
    if (nLen == 0) {
        return NULL;
    }
    char* pResult = new char[nLen];
    WideCharToMultiByte( CP_ACP, 0, szStr, -1, pResult, nLen, NULL, NULL );
    return pResult;
}


/*
 * AnsiToUnicode converts the ANSI string pszA to a Unicode string
 * and returns the Unicode string through ppszW. Space for the
 * the converted string is allocated by AnsiToUnicode.
 */

HRESULT __fastcall AnsiToUnicode(LPCSTR pszA, wchar_t** ppszW)
{

    ULONG cCharacters;
    DWORD dwError;

    // If input is null then just return the same.
    if (NULL == pszA) {
        *ppszW = NULL;
        return NOERROR;
    }

    // Determine number of wide characters to be allocated for the
    // Unicode string.
    cCharacters =  strlen(pszA)+1;

    // Use of the OLE allocator is required if the resultant Unicode
    // string will be passed to another COM component and if that
    // component will free it. Otherwise you can use your own allocator.
    *ppszW = (wchar_t*)malloc(cCharacters*2);
    if (NULL == *ppszW)
        return E_OUTOFMEMORY;

    // Covert to Unicode.
    if (0 == MultiByteToWideChar(CP_ACP, 0, pszA, cCharacters,
                                 *ppszW, cCharacters)) {
        dwError = GetLastError();
        free(*ppszW);
        *ppszW = NULL;
        return HRESULT_FROM_WIN32(dwError);
    }

    return NOERROR;
}
/*
 * UnicodeToAnsi converts the Unicode string pszW to an ANSI string
 * and returns the ANSI string through ppszA. Space for the
 * the converted string is allocated by UnicodeToAnsi.
 */

HRESULT __fastcall UnicodeToAnsi2(wchar_t* pszW, LPSTR* ppszA)
{

    ULONG cbAnsi, cCharacters;
    DWORD dwError;

    // If input is null then just return the same.
    if (pszW == NULL) {
        *ppszA = NULL;
        return NOERROR;
    }

    cCharacters = wcslen(pszW)+1;
    // Determine number of bytes to be allocated for ANSI string. An
    // ANSI string can have at most 2 bytes per character (for Double
    // Byte Character Strings.)
    cbAnsi = cCharacters*2;

    // Use of the OLE allocator is not required because the resultant
    // ANSI  string will never be passed to another COM component. You
    // can use your own allocator.
    *ppszA = (LPSTR)malloc(cbAnsi);
    if (NULL == *ppszA)
        return E_OUTOFMEMORY;

    // Convert to ANSI.
    if (0 == WideCharToMultiByte(CP_ACP, 0, pszW, cCharacters, *ppszA,
                                 cbAnsi, NULL, NULL)) {
        dwError = GetLastError();
        free(*ppszA);
        *ppszA = NULL;
        return HRESULT_FROM_WIN32(dwError);
    }
    return NOERROR;

}

void UTF8ToGBK(const char *szOut, char **out )
{
    unsigned short *wszGBK;
    char *szGBK;
//闀垮害
    int len = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)szOut, -1, NULL, 0);
    wszGBK = new unsigned short[len+1];
    memset(wszGBK, 0, len * 2 + 2);
    MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)szOut, -1, (LPWSTR)wszGBK, len);
//闀垮害
    len = WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, NULL, 0, NULL, NULL);
    *out = new char[len+1];
    szGBK = *out;
    memset(szGBK, 0, len + 1);
    WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, szGBK, len, NULL, NULL);
//szOut = szGBK; //杩欐牱寰楀埌鐨剆zOut涓嶆纭紝鍥犱负姝ゅ彞鎰忎箟鏄皢szGBK鐨勯鍦板潃璧嬬粰szOut锛屽綋delete []szGBK鎵ц鍚巗zGBK鐨勫唴
    //瀛樼┖闂村皢琚噴鏀撅紝姝ゆ椂灏嗗緱涓嶅埌szOut鐨勫唴瀹?
    delete []wszGBK;
}

void GBKToUTF8(char* &szOut)
{
    char* strGBK = szOut;
    int len=MultiByteToWideChar(CP_ACP, 0, (LPCSTR)strGBK, -1, NULL,0);
    unsigned short * wszUtf8 = new unsigned short[len+1];
    memset(wszUtf8, 0, len * 2 + 2);
    MultiByteToWideChar(CP_ACP, 0, (LPCSTR)strGBK, -1, (LPWSTR)wszUtf8, len);
    len = WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)wszUtf8, -1, NULL, 0, NULL, NULL);
    char *szUtf8=new char[len + 1];
    memset(szUtf8, 0, len + 1);
    WideCharToMultiByte (CP_UTF8, 0, (LPWSTR)wszUtf8, -1, szUtf8, len, NULL,NULL);
//szOut = szUtf8;
    memset(szOut,'\0',strlen(szUtf8)+1);
    memcpy(szOut,szUtf8,strlen(szUtf8));
    delete[] szUtf8;
    delete[] wszUtf8;
}

//百分号编码
//http://zh.wikipedia.org/zh-cn/%E7%99%BE%E5%88%86%E5%8F%B7%E7%BC%96%E7%A0%81

BOOL UrlEncode(const char* szSrc, char* pBuf, int cbBufLen, BOOL bUpperCase)
{
    if(szSrc == NULL || pBuf == NULL || cbBufLen <= 0)
        return FALSE;

    size_t len_ascii = strlen(szSrc);
    if(len_ascii == 0)
    {
        pBuf[0] = 0;
        return TRUE;
    }
    
    //先转换到UTF-8
    char baseChar = bUpperCase ? 'A' : 'a';
    int cchWideChar = MultiByteToWideChar(CP_ACP, 0, szSrc, len_ascii, NULL, 0);
    LPWSTR pUnicode = (LPWSTR)malloc((cchWideChar + 1) * sizeof(WCHAR));
    if(pUnicode == NULL)
        return FALSE;
    MultiByteToWideChar(CP_ACP, 0, szSrc, len_ascii, pUnicode, cchWideChar + 1);

    int cbUTF8 = WideCharToMultiByte(CP_UTF8, 0, pUnicode, cchWideChar, NULL, 0, NULL, NULL);
    LPSTR pUTF8 = (LPSTR)malloc((cbUTF8 + 1) * sizeof(CHAR));
    if(pUTF8 == NULL)
    {
        free(pUnicode);
        return FALSE;
    }
    WideCharToMultiByte(CP_UTF8, 0, pUnicode, cchWideChar, pUTF8, cbUTF8 + 1, NULL, NULL);
    pUTF8[cbUTF8] = '\0';

    unsigned char c;
    int cbDest = 0; //累加
    unsigned char *pSrc = (unsigned char*)pUTF8;
    unsigned char *pDest = (unsigned char*)pBuf;
    while(*pSrc && cbDest < cbBufLen - 1)
    {
        c = *pSrc;
        if(isalpha(c) || isdigit(c) || c == '-' || c == '.' || c == '~')
        {
            *pDest = c;
            ++pDest;
            ++cbDest;
        }
        else if(c == ' ')
        {
            *pDest = '+';
            ++pDest;
            ++cbDest;
        }
        else
        {
            //检查缓冲区大小是否够用？
            if(cbDest + 3 > cbBufLen - 1)
                break;
            pDest[0] = '%';
            pDest[1] = (c >= 0xA0) ? ((c >> 4) - 10 + baseChar) : ((c >> 4) + '0');
            pDest[2] = ((c & 0xF) >= 0xA)? ((c & 0xF) - 10 + baseChar) : ((c & 0xF) + '0');
            pDest += 3;
            cbDest += 3;
        }
        ++pSrc;
    }
    //null-terminator
    *pDest = '\0';
    free(pUnicode);
    free(pUTF8);
    return TRUE;
}

//解码后是utf-8编码
BOOL UrlDecode(const char* szSrc, char* pBuf, int cbBufLen)
{
    if(szSrc == NULL || pBuf == NULL || cbBufLen <= 0)
        return FALSE;

    size_t len_ascii = strlen(szSrc);
    if(len_ascii == 0)
    {
        pBuf[0] = 0;
        return TRUE;
    }
    
    char *pUTF8 = (char*)malloc(len_ascii + 1);
    if(pUTF8 == NULL)
        return FALSE;

    int cbDest = 0; //累加
    unsigned char *pSrc = (unsigned char*)szSrc;
    unsigned char *pDest = (unsigned char*)pUTF8;
    while(*pSrc)
    {
        if(*pSrc == '%')
        {
            *pDest = 0;
            //高位
            if(pSrc[1] >= 'A' && pSrc[1] <= 'F')
                *pDest += (pSrc[1] - 'A' + 10) * 0x10;
            else if(pSrc[1] >= 'a' && pSrc[1] <= 'f')
                *pDest += (pSrc[1] - 'a' + 10) * 0x10;
            else
                *pDest += (pSrc[1] - '0') * 0x10;

            //低位
            if(pSrc[2] >= 'A' && pSrc[2] <= 'F')
                *pDest += (pSrc[2] - 'A' + 10);
            else if(pSrc[2] >= 'a' && pSrc[2] <= 'f')
                *pDest += (pSrc[2] - 'a' + 10);
            else
                *pDest += (pSrc[2] - '0');

            pSrc += 3;
        }
        else if(*pSrc == '+')
        {
            *pDest = ' ';
            ++pSrc;
        }
        else
        {
            *pDest = *pSrc;
            ++pSrc;
        }
        ++pDest;
        ++cbDest;
    }
    //null-terminator
    *pDest = '\0';
    ++cbDest;

    int cchWideChar = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)pUTF8, cbDest, NULL, 0);
    LPWSTR pUnicode = (LPWSTR)malloc(cchWideChar * sizeof(WCHAR));
    if(pUnicode == NULL)
    {
        free(pUTF8);
        return FALSE;
    }
    MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)pUTF8, cbDest, pUnicode, cchWideChar);
    WideCharToMultiByte(CP_ACP, 0, pUnicode, cchWideChar, pBuf, cbBufLen, NULL, NULL);
    free(pUTF8);
    free(pUnicode);
    return TRUE;
}

