
//#define D_DEBUG
#include "libcgi.h"

#define CGI 1

#if 0
HBITMAP CopyScreenToBitmap(int* nWidth,int* nHeight);
int Save16BitmapToFile(HBITMAP hBitmap,LPCTSTR lpFileName);


HBITMAP CopyScreenToBitmap(int* nWidth,int* nHeight)
{

    // 屏幕和内存设备描述表
    HDC  hScrDC, hMemDC;
    // 位图句柄
    HBITMAP  hBitmap, hOldBitmap;
    // 屏幕分辨率
    int  xScrn, yScrn;

    //为屏幕创建设备描述表
    hScrDC = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
    //为屏幕设备描述表创建兼容的内存设备描述表
    hMemDC = CreateCompatibleDC(hScrDC);
    // 获得屏幕分辨率
    xScrn = GetDeviceCaps(hScrDC, HORZRES);

    yScrn= GetDeviceCaps(hScrDC, VERTRES);

    //存储屏幕的长宽
    *nWidth = xScrn;
    *nHeight = yScrn;

    // 创建一个与屏幕设备描述表兼容的位图
    hBitmap = CreateCompatibleBitmap(hScrDC, xScrn, yScrn);
    // 把新位图选到内存设备描述表中
    hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
    // 把屏幕设备描述表拷贝到内存设备描述表中
    BitBlt(hMemDC, 0, 0, xScrn,yScrn,hScrDC, 0, 0, SRCCOPY);
    //得到屏幕位图的句柄
    hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);
    //清除
    DeleteDC(hScrDC);
    DeleteDC(hMemDC);
    // 返回位图句柄
    return hBitmap;
}


int Save16BitmapToFile(HBITMAP hBitmap,LPCTSTR lpFileName)//将截屏所得保存为16位的图片
{


    HDC hDC=CreateDC(TEXT("DISPLAY"),NULL,NULL,NULL);
    HDC hOffDC=CreateCompatibleDC(hDC);
    SelectObject(hOffDC,hBitmap);

    BITMAP Bitmap;
    GetObject(hBitmap,sizeof(BITMAP),&Bitmap);

    HANDLE fh=CreateFile(lpFileName,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,
                         FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,NULL);
    if(fh == INVALID_HANDLE_VALUE )
        return FALSE;

    BITMAPFILEHEADER bfh;
    memset(&bfh,0,sizeof(bfh));
    bfh.bfType=0x4D42/*((WORD) ('M' << 8) | 'B')*/;
    bfh.bfSize= sizeof(bfh)+2*Bitmap.bmWidth*Bitmap.bmHeight+sizeof(BITMAPFILEHEADER);
    bfh.bfOffBits=sizeof(BITMAPINFOHEADER)+sizeof(BITMAPFILEHEADER);
    DWORD dwWritten=0;
    WriteFile(fh,&bfh,sizeof(bfh),&dwWritten,NULL);
    BITMAPINFOHEADER bih;
    memset(&bih,0,sizeof(bih));
    bih.biSize=sizeof(bih);
    bih.biWidth=Bitmap.bmWidth;
    bih.biHeight=Bitmap.bmHeight;
    bih.biPlanes=1;
    bih.biBitCount=16;

    if( !WriteFile(fh,&bih,sizeof(bih),&dwWritten,NULL) ) {
        return FALSE;
    }

    BITMAPINFO bitmapInfo;
    memset((void *)&bitmapInfo,0,sizeof(BITMAPINFO) );
    bitmapInfo.bmiHeader=bih;

    HDC hMemDC=CreateCompatibleDC(hDC);
    BYTE *m_lpBitBmp=new BYTE[bfh.bfSize-sizeof(BITMAPFILEHEADER)];
    HBITMAP hDibBitmap=CreateDIBSection(hMemDC,&bitmapInfo,DIB_RGB_COLORS,(void **)&m_lpBitBmp,
                                        NULL,0);
    if(hDibBitmap != 0) {
        ::SelectObject(hMemDC,hDibBitmap);
        BitBlt(hMemDC,0,0,Bitmap.bmWidth,Bitmap.bmHeight,hOffDC,0,0,SRCCOPY);
        WriteFile(fh,m_lpBitBmp,bfh.bfSize-sizeof(BITMAPFILEHEADER),&dwWritten,NULL);
    }

    DeleteObject(hDibBitmap);
    DeleteDC(hDC);
    DeleteDC(hMemDC);

    CloseHandle(fh);

    return 1;

}

#endif


int _tmain(int argc, _TCHAR* argv[])
{
#if CGI
    char_t buf[256];
    int nread = 0;
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
    write_data(L"<P>CGI_Data = %s</P>\n", getCgiData(buf,sizeof(buf),&nread));
    write_data(L"<P>CGI_Size = %d</P>\n", nread);
    write_data(L"<P>out = %s</P>\n", argv[argc-1]);
    write_data(L"<P>in = %s</P>\n", argv[argc-2]);
    write_data(L"<P>env = %s</P>\n", argv[argc-3]);

#endif

#if 0
    int 	nWidth,nHeight;
    HBITMAP MyhBitmap;

    MyhBitmap = CopyScreenToBitmap(&nWidth, &nHeight );
    wprintf(L"width=%d\t,height=%d\n",nWidth, nHeight);
    //Save16BitmapToFile( MyhBitmap, L"\\NandFlash\\go.bmp");
    Save16BitmapToFile( MyhBitmap, L"go.bmp");
    wprintf(L"xxxxxxxxxxx");

#endif
#if CGI
//销毁资源
    destroy();
#endif
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

