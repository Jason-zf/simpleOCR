#ifndef CVTCHAR
#define CVTCHAR


#include<Windows.h>
#include<wchar.h>


class CvtChar
{
public:
	CvtChar();
	~CvtChar();
public:
	char* WideByte2MultiByte(const wchar_t * src);
	wchar_t* MultiByte2WideByte(const  char * src);

private:

};





#endif