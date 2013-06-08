#ifndef __LIBCGI_H__
#define __LIBCGI_H__

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <winbase.h>
#include <commctrl.h>
#define char_t wchar_t



#ifdef __cplusplus
extern "C" {
#endif
extern HANDLE hIn;
int init(int argc, _TCHAR* argv[]);
void destroy();
int write_data(const char_t *fmt,...);
char_t* gogetenv(char_t *key,char_t *value);
char_t* getCgiData(char_t* buf,unsigned int buf_size,int *nread);
void hexdump(void *_data, unsigned len);
#ifdef __cplusplus
}
#endif
#endif