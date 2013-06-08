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
        fprintf(stderr," %02x %c", *data,
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

int init(int argc, _TCHAR* argv[])
{
    char_t buf[256];
    hOut = CreateFile(argv[argc-1], GENERIC_WRITE, FILE_SHARE_WRITE, NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL, NULL);
	hIn = CreateFile(argv[argc-2], GENERIC_READ, FILE_SHARE_READ, NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL, NULL);
    if(hOut == INVALID_HANDLE_VALUE || hIn == INVALID_HANDLE_VALUE)
        return -1;
    write_data(L"Content-type:text/html;charset=gbk\r\n\r\n");
    //write_data(L"<TITLE>CGI</TITLE><H3>CGI environment</H3>");

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