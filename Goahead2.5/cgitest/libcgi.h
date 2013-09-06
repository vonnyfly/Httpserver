#ifndef __LIBCGI_H__
#define __LIBCGI_H__

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <winbase.h>
#include <commctrl.h>
#define char_t wchar_t

#ifdef DEBUG
#define DBG(...) do{\
	write_data(L"<!-- *** %s, %s(), %d ***: ",	\
	_T(__FILE__), _T(__FUNCTION__), __LINE__ ); write_data(__VA_ARGS__);\
	write_data(L"-->\n");\
	}while(0)
#else
#define DBG(...)
#endif

#ifdef __cplusplus
extern "C" {
#endif
extern HANDLE hIn;
extern int init(int argc, _TCHAR* argv[]);
extern void destroy();
extern int write_data(const char_t *fmt,...);
extern char_t* gogetenv(char_t *key,char_t *value);
extern char_t* getCgiData(char_t* buf,unsigned int buf_size,int *nread);

extern char_t* getCgiDataWithMalloc(int *len);
extern void freeCgiDataWithMalloc(char_t *pData);

extern  void hexdump(const void *_data, unsigned len);
extern char *str_replace(char *orig, char *rep, char *with);

extern HRESULT __fastcall AnsiToUnicode(LPCSTR pszA, wchar_t** ppszW);
extern HRESULT __fastcall UnicodeToAnsi2(wchar_t* pszW, LPSTR* ppszA);
extern char* UnicodeToAnsi( const wchar_t* szStr );

extern BOOL UrlEncode(const char* szSrc, char* pBuf, int cbBufLen, BOOL bUpperCase);
extern BOOL UrlDecode(const char* szSrc, char* pBuf, int cbBufLen);

extern void UTF8ToGBK(const char *szOut, char** out);
#ifdef __cplusplus
}
#endif
#endif