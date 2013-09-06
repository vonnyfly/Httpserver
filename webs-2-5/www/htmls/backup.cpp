#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <winbase.h>
#define char_t unsigned short

//#define D_DEBUG

HANDLE hOut;
FILE* fin;

//ok:return value;else return null
char_t* gogetenv(char_t *key,char_t *value)
{
    char_t* pos=NULL;
    char_t buf[256];
    int bSuccess;

    if (fin) {
        fseek(fin,0,SEEK_SET);
        while (fgetws((wchar_t*)buf, 256, fin)) { // Key & Value
            pos = wcschr(buf, L'=');
            if (!pos) {
                continue;
            }
            *pos = L'\0'; // Erase the '='
            if (0 != _wcsicmp(buf, key)) { // Key
                continue;
            }
            pos++; // Point to the value
            bSuccess = SUCCEEDED(StringCchCopy(value,256,pos));
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
    if(hOut == INVALID_HANDLE_VALUE)
        return -1;
    write_data(L"Content-type:text/html;charset=gbk\r\n\r\n");
    write_data(L"<TITLE>CGI</TITLE><H3>CGI environment</H3>");

    fin = _wfopen(argv[argc-2], L"rb");
    return 1;
}
void destroy()
{
    CloseHandle(hOut);
    fclose(fin);
}

int write_data(const char_t *fmt,...)
{
    va_list		args;
    char_t		buf[256];
    int need_write,written;

    va_start(args, fmt);
    need_write = vswprintf(buf,fmt,args);
    va_end(args);
    WriteFile(hOut,buf,2*need_write,&written,NULL) ;
#ifdef D_DEBUG
    wprintf(L"%s| sizeof: %d need write:%d written: %d wcslen: %d\n",buf,sizeof(buf),need_write,written,wcslen(buf));
#endif
    return written;
}
/*------------------------------begin COM --------*/
/*
void ReportCommError(LPTSTR lpszMessage);
DWORD WINAPI CommReadThreadFunc(LPVOID lpParam);

HANDLE hCommPort = INVALID_HANDLE_VALUE;
void Listing9_1()
{
	  // set the timeouts to specify the behavior of
  // reads and writes.
  COMMTIMEOUTS ct;
    // Get the current communications parameters,
  // and configure baud rate
  DCB dcb;
HANDLE hCommReadThread;

  hCommPort = CreateFile (_T("COM1:"),
    GENERIC_READ | GENERIC_WRITE,
    0,        // COM port cannot be shared
    NULL,    // Always NULL for Windows CE
    OPEN_EXISTING,
    0,          // Non-overlapped operation only
    NULL);      // Always NULL for Windows CE
  if(hCommPort == INVALID_HANDLE_VALUE)
  {
  ReportCommError(_T("Opening Comms Port."));
    return;
  }

  ct.ReadIntervalTimeout = MAXDWORD;
  ct.ReadTotalTimeoutMultiplier = 0;
  ct.ReadTotalTimeoutConstant = 0;
  ct.WriteTotalTimeoutMultiplier = 10;
  ct.WriteTotalTimeoutConstant = 1000;
  if(!SetCommTimeouts(hCommPort, &ct))
  {
    ReportCommError(_T("Setting comm. timeouts."));
    return;
  }

  dcb.DCBlength = sizeof(DCB);
  if(!GetCommState(hCommPort, &dcb))
  {
    ReportCommError(_T("Getting Comms. State."));
    return;
  }
  dcb.BaudRate = CBR_19200; // set baud rate to 19,200
  dcb.fOutxCtsFlow = TRUE;
  dcb.fRtsControl        = RTS_CONTROL_HANDSHAKE;
  dcb.fDtrControl        = DTR_CONTROL_ENABLE;
  dcb.fOutxDsrFlow      = FALSE;
  dcb.fOutX              = FALSE; // no XON/XOFF control
  dcb.fInX              = FALSE;
  dcb.ByteSize          = 8;
  dcb.Parity            = NOPARITY;
  dcb.StopBits          = ONESTOPBIT;
  if(!SetCommState(hCommPort, &dcb))
  {
    ReportCommError(_T("Setting Comms. State."));
    return;
  }
  // now need to create the thread that will
  // be reading the comms port
  hCommReadThread = CreateThread(NULL, 0,
      CommReadThreadFunc, NULL, 0, NULL);
  if(hCommReadThread == NULL)
  {
    ReportCommError(_T("Creating Thread."));
    return;
  }
  else
    CloseHandle(hCommReadThread);
}
void ReportCommError(LPTSTR lpszMessage)
{
  TCHAR szBuffer[200];
  wsprintf(szBuffer,
    _T("Communications Error %d \r\n%s"),
    GetLastError(),
    lpszMessage);
}

DWORD WINAPI CommReadThreadFunc(LPVOID lpParam)
{
	INT rc;
	DWORD cBytes;
	BYTE ch = 'c';

	WriteFile(hCommPort,&ch,1,&cBytes,NULL);
	return 0;
}
*/

int _tmain(int argc, _TCHAR* argv[])
{
    int i=0;
    HANDLE hCom;
    char_t buf[256];
//初始化，必须调用
    if(init(argc, argv)<0) {
        wprintf(L"[-] cgi file open failed\n");
        exit(-1);
    }

    write_data(L"<html>\n");
    write_data(L"<head><title>CGI Output</title></head>\n");
    write_data(L"<body>\n");

    write_data(L"<H1>CGI Test Program</H1>\n");
    write_data(L"<P>This program displays the CGI Enviroment</P>\n");

    /*
     *	Print the CGI environment variables
     */
    write_data(L"<H2>Environment Variables<A NAME=\"env\"></A></H2>\n");
    write_data(L"<P>REQUEST METHOD = %s</P>\n", gogetenv(L"REQUEST_METHOD",buf));
    write_data(L"<P>QUERY STRING = %s</P>\n", gogetenv(L"QUERY_STRING",buf));
    write_data(L"<P>PATH TRANSLATED = %s</P>\n", gogetenv(L"PATH_TRANSLATED",buf));
    write_data(L"<P>GATEWAY INTERFACE = %s</P>\n", gogetenv(L"GATEWAY_INTERFACE",buf));
    write_data(L"<P>SCRIPT NAME = %s</P>\n", gogetenv(L"SCRIPT_NAME",buf));


    write_data(L"<P>PATH INFO = %s</P>\n", gogetenv(L"PATH_INFO",buf));

    write_data(L"<P>HTTP HOST = %s</P>\n", gogetenv(L"HTTP_HOST",buf));
    write_data(L"<P>HTTP ACCEPT = %s</P>\n", gogetenv(L"HTTP_ACCEPT",buf));
    write_data(L"<P>HTTP CONNECTION = %s</P>\n", gogetenv(L"HTTP_CONNECTION",buf));

    write_data(L"<P>REMOTE USER = %s</P>\n", gogetenv(L"REMOTE_USER",buf));
    write_data(L"<P>REMOTE HOST = %s</P>\n", gogetenv(L"REMOTE_HOST",buf));
    write_data(L"<P>REMOTE ADDR = %s</P>\n", gogetenv(L"REMOTE_ADDR",buf));

    write_data(L"<P>SERVER NAME = %s</P>\n", gogetenv(L"SERVER_NAME",buf));
    write_data(L"<P>SERVER PORT = %s</P>\n", gogetenv(L"SERVER_PORT",buf));
    write_data(L"<P>SERVER HOST = %s</P>\n", gogetenv(L"SERVER_HOST",buf));
    write_data(L"<P>SERVER PROTOCOL = %s</P>\n", gogetenv(L"SERVER_PROTOCOL",buf));
    write_data(L"<P>SERVER ADDR = %s</P>\n", gogetenv(L"SERVER_ADDR",buf));
    write_data(L"<P>SERVER URL = %s</P>\n", gogetenv(L"SERVER_URL",buf));
    write_data(L"<P>SERVER SOFTWARE = %s</P>\n", gogetenv(L"SERVER_SOFTWARE",buf));

//销毁资源
    destroy();
    return 0;
}


//example: CGI env
//PATH_TRANSLATED=/www/cgi-bin/cgitest.exe
//SCRIPT_NAME=cgi-bin/cgitest.exe
//REMOTE_USER=(null)
//AUTH_TYPE=(null)
//SERVER_HOST=Windo68.
//REQUEST_METHOD=GET
//HTTP_ACCEPT_LANGUAGE=zh-CN
//QUERY_STRING=m=1&n=1
//HTTP_ACCEPT_ENCODING=gzip, deflate
//SERVER_PORT=80
//SERVER_NAME=Windo68.
//GATEWAY_INTERFACE=CGI/1.1
//HTTP_ACCEPT=text/html, application/xhtml+xml, */*
//HTTP_CONNECTION=Keep-Alive
//SERVER_SOFTWARE=GoAhead-Webs/2.5.0
//PATH_INFO=/cgi-bin/cgitest.exe
//SERVER_PROTOCOL=HTTP/1.1
//REMOTE_ADDR=192.168
//HTTP_DNT=1
//HTTP_USER_AGENT=Mozilla/5.0 (compatible; MSIE 10.0; Windows NT 6.1; WOW64; Trident/6.0)
//HTTP_HOST=192.168.212.129
//SERVER_ADDR=??????㈱9
//m=1
//n=1
//SERVER_URL=Windo68.

