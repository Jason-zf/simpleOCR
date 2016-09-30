#include"stdafx.h"
#include"CvtChar.h"



//存在问题：由于char*开辟内存后没有释放所以不太适合用于循环中（如果要用就要释放内存）
char*CvtChar::WideByte2MultiByte(const wchar_t * src)
{
	//getbuffer 到底是个什么东西
	//掌握这个api
	char*dest;
	int len = WideCharToMultiByte(CP_ACP, 0, src, lstrlenW(src), NULL, 0, NULL, NULL);
	dest = new char[len + 1];
	memset(dest, 0, len + 1);
	WideCharToMultiByte(CP_ACP, NULL, (LPCWSTR)src, -1, (LPSTR)dest, len, NULL, FALSE);
	dest[len] = '\0';
	return dest;
}
wchar_t* CvtChar::MultiByte2WideByte(const  char * src)
{
	int len = MultiByteToWideChar(CP_ACP, 0, src, strlen(src), NULL, 0);
	wchar_t* dest = new wchar_t[len + 1];
	memset(dest, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, src, -1, dest, len);
	dest[len] = '\0';
	return dest;
}
CvtChar::CvtChar()
{
}

CvtChar::~CvtChar()
{
}
