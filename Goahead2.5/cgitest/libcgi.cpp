#include "libcgi.h"

HANDLE hOut,hIn;
FILE* fenv;

void hexdump(void *_data, unsigned len)
{
    unsigned char *data = (unsigned char *)_data;
    unsigned count;

    for (count = 0; count < len; count++) {
        if ((count & 15) == 0)
            write_data(L"%04x:", count);
        write_data(L" %02x %c", *data,
                (*data < 32) || (*data > 126) ? '.' : *data);
        data++;
        if ((count & 15) == 15)
            write_data(L"\n");
    }
    if ((count & 15) != 0)
        write_data(L"\n");
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
	}while(1);

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
	if (nLen == 0)
	{
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
    if (NULL == pszA)
    {
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
                  *ppszW, cCharacters))
    {
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
    if (pszW == NULL)
    {
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
                  cbAnsi, NULL, NULL))
    {
        dwError = GetLastError();
        free(*ppszA);
        *ppszA = NULL;
        return HRESULT_FROM_WIN32(dwError);
    }
    return NOERROR;

}
				