#include "../cgitest/libcgi.h"

char_t DATA_DIR[] = L"\\data";
char_t CONFIG_FILE[] = L"config.xml";

#define strncpy wcsncpy
#define strlen wcslen
#define strcmp wcscmp
#define strncat wcsncat
#define print wprintf

//action=list
void gListFiles()
{
}

//action=edit
void gEditFile()
{
}

//action=new
void gNewFile()
{
}

//action=open
void gOpenFile()
{
}

//action=copy
void gCopyFile()
{
}

//action=print
void gPrintFile()
{
}

//action=settings
void gSettings()
{

}

//action=home
void gHome()
{
	write_data(L"<!DOCTYPE html PUBLIC '-//W3C//DTD XHTML 1.0 Transitional//EN' 'http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd'>");
	write_data(L"<html xmlns='http://www.w3.org/1999/xhtml'>");
	write_data(L"<head>");
	write_data(L"	<meta http-equiv='Content-Type' content='text/html; charset=utf-8' />");
	write_data(L"	<title>÷˜“≥</title>");
	write_data(L"</head>");
	write_data(L"<body>");
	write_data(L"	<form action='cgi-bin/test' method='get'>");
	write_data(L"		<p align='center'>");
	write_data(L"			<input name='textFilename' type='text' value='µ±«∞Œƒº˛√˚' />");
	write_data(L"		</p>");
	write_data(L"		<p align='center'>");
	write_data(L"			<input name='preview' type='image' value='‘§¿¿' src='../preview.jpg' />");
	write_data(L"		</p>");
	write_data(L"		<p align='center'>");
	write_data(L"			<input name='bPrint' type='button' value='¥Ú”°' onclick='window.location.href=print.html'/>");
	write_data(L"			<input name='bManage' type='button' value='π‹¿Ì' />");
	write_data(L"			<input name='bModify' type='button' value='±‡º≠' onclick='window.location.href=modify.html'/>");
	write_data(L"		</p>");
	write_data(L"	</form>");
	write_data(L"");
	write_data(L"</body>");
	write_data(L"</html>");
}

static void ListCurDir(char_t *dir)
{
    WIN32_FIND_DATA FindFileData;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    wchar_t DirSpec[MAX_PATH + 1];
    DWORD dwError;

    strncpy (DirSpec, dir, strlen(dir) + 1);
    strncat (DirSpec, L"/*", 3);

    hFind = FindFirstFile(DirSpec, &FindFileData);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        write_data(L"Invalid file handle. Error is %u ", GetLastError());
        return ;
    }
    else
    {
        if (FindFileData.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY )
        {
            write_data(L"	%s ", FindFileData.cFileName);   
        }
        /*
        else if(FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY
            && strcmp(FindFileData.cFileName, ".") != 0
            && strcmp(FindFileData.cFileName, "..") != 0)
        {		
            wchar_t Dir[MAX_PATH + 1];
            strcpy(Dir, pFilePath);
            strncat(Dir, "/", 2);
            strcat(Dir, FindFileData.cFileName);

            FindFile(Dir);
        }
		*/
        while (FindNextFile(hFind, &FindFileData) != 0)
        {
            if (FindFileData.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
            {		
                write_data(L"	%s ", FindFileData.cFileName);
            }
            /*
            else if(FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY
                && strcmp(FindFileData.cFileName, ".") != 0
                && strcmp(FindFileData.cFileName, "..") != 0)
            { 
                wchar_t Dir[MAX_PATH + 1];
                strcpy(Dir, pFilePath);
                strncat(Dir, "/", 2);
                strcat(Dir, FindFileData.cFileName);
                FindFile(Dir);
            }
            */

        }

        dwError = GetLastError();
        FindClose(hFind);
        if (dwError != ERROR_NO_MORE_FILES)
        {
            write_data(L"FindNextFile error. Error is %u ", dwError);
            return;
        }
    }
}

static BOOL DirectoryExists(const char_t* dirName) {
  DWORD attribs = GetFileAttributes(dirName);
  if (attribs == INVALID_FILE_ATTRIBUTES) {
    return false;
  }
  return (attribs & FILE_ATTRIBUTE_DIRECTORY);
}

int _tmain(int argc, _TCHAR* argv[])
{
    char_t ops_buf[10],name_buf[256],buf[256],tmpBuf[256];
	HANDLE hXmlFile;
	BOOL ret;
	int nRead = 0,nWritten;

//¬≥√µ√ä¬º¬ª¬Ø¬£¬¨¬±√ò√ê√´¬µ√∑√ì√É
    if(init(argc, argv)<0) {
        wprintf(L"[-] cgi file open failed\n");
        exit(-1);
    }
	//create data dir
	if(!DirectoryExists(DATA_DIR)){
		CreateDirectory(DATA_DIR,NULL);
	}
	ListCurDir(DATA_DIR);
    gogetenv(L"ops",ops_buf);
    gogetenv(L"file",name_buf);
	swprintf(buf,L"%s\\%s",DATA_DIR,name_buf);
	write_data(L"%s,%s,%s",ops_buf,name_buf,buf);
	if(ops_buf){
		if(_wcsicmp(ops_buf,L"del")==0){
			write_data(L"del,%s",buf);
			DeleteFile(buf);
		}else{
			/*
			hXmlFile = CreateFile(buf, GENERIC_WRITE, FILE_SHARE_WRITE, NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL, NULL);
			//hXmlFile = CreateFile(L"\\data\\app.xml", GENERIC_WRITE, FILE_SHARE_WRITE, NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL, NULL);
			if(hXmlFile != INVALID_HANDLE_VALUE){
				write_data(L"Open sucess");
				do {
					ReadFile(hIn, tmpBuf, 256,(LPDWORD)&nRead,NULL);
					write_data(L"nRead=%d",nRead);
					if(nRead <=0)
						break;
					WriteFile(hXmlFile,tmpBuf,nRead,(LPDWORD)&nWritten,NULL);
				}while(1);
			}else{
				
				write_data(L"Open failed,%d",GetLastError());
			}
			
			CloseHandle(hXmlFile);
			*/
			ret = CopyFile(argv[argc-2],buf,FALSE);
			
		}
	}

  
//√è√∫¬ª√ô√ó√ä√î¬¥
    destroy();
    return 0;
}