//#define D_DEBUG
#include "../cgitest/libcgi.h"
#define CGI 1

int _tmain(int argc, _TCHAR* argv[])
{
    char_t buf[256];
	int nread = 0;
//初始化，必须调用
    if(init(argc, argv)<0) {
        wprintf(L"[-] cgi file open failed\n");
        exit(-1);
    }
	write_data(L"<!DOCTYPE html PUBLIC '-//W3C//DTD XHTML 1.0 Transitional//EN' 'http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd'>");
	write_data(L"<html xmlns='http://www.w3.org/1999/xhtml'>");
	write_data(L"<head>");
	write_data(L"	<meta http-equiv='Content-Type' content='text/html; charset=utf-8' />");
	write_data(L"	<title>主页</title>");
	write_data(L"</head>");
	write_data(L"<body>");
	write_data(L"	<form action='cgi-bin/test' method='get'>");
	write_data(L"		<p align='center'>");
	write_data(L"			<input name='textFilename' type='text' value='当前文件名' />");
	write_data(L"		</p>");
	write_data(L"		<p align='center'>");
	write_data(L"			<input name='preview' type='image' value='预览' src='../preview.jpg' />");
	write_data(L"		</p>");
	write_data(L"		<p align='center'>");
	write_data(L"			<input name='bPrint' type='button' value='打印' onclick='window.location.href=print.html'/>");
	write_data(L"			<input name='bManage' type='button' value='管理' />");
	write_data(L"			<input name='bModify' type='button' value='编辑' onclick='window.location.href=modify.html'/>");
	write_data(L"		</p>");
	write_data(L"	</form>");
	write_data(L"");
	write_data(L"</body>");
	write_data(L"</html>");


}