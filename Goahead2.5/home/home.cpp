//#define D_DEBUG
#include "../cgitest/libcgi.h"
#define CGI 1

int _tmain(int argc, _TCHAR* argv[])
{
    char_t buf[256];
	int nread = 0;
//��ʼ�����������
    if(init(argc, argv)<0) {
        wprintf(L"[-] cgi file open failed\n");
        exit(-1);
    }
	write_data(L"<!DOCTYPE html PUBLIC '-//W3C//DTD XHTML 1.0 Transitional//EN' 'http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd'>");
	write_data(L"<html xmlns='http://www.w3.org/1999/xhtml'>");
	write_data(L"<head>");
	write_data(L"	<meta http-equiv='Content-Type' content='text/html; charset=utf-8' />");
	write_data(L"	<title>��ҳ</title>");
	write_data(L"</head>");
	write_data(L"<body>");
	write_data(L"	<form action='cgi-bin/test' method='get'>");
	write_data(L"		<p align='center'>");
	write_data(L"			<input name='textFilename' type='text' value='��ǰ�ļ���' />");
	write_data(L"		</p>");
	write_data(L"		<p align='center'>");
	write_data(L"			<input name='preview' type='image' value='Ԥ��' src='../preview.jpg' />");
	write_data(L"		</p>");
	write_data(L"		<p align='center'>");
	write_data(L"			<input name='bPrint' type='button' value='��ӡ' onclick='window.location.href=print.html'/>");
	write_data(L"			<input name='bManage' type='button' value='����' />");
	write_data(L"			<input name='bModify' type='button' value='�༭' onclick='window.location.href=modify.html'/>");
	write_data(L"		</p>");
	write_data(L"	</form>");
	write_data(L"");
	write_data(L"</body>");
	write_data(L"</html>");


}