//CvtStringÍ·ÎÄ¼þ

#ifndef _CVT_STRING_
#define _CVT_STRING_

#include <sstream>
#include <iostream>
#include <string>
#include <cstdlib>
#include<atlstr.h>

using namespace std;

class CvtString
{
public:
	CvtString();
	~CvtString();
public:
	template<class in_T, class out_T>
	out_T convert(const in_T& in);
	template<class T>
	T cstring_to(const wstring& str);
	template<class T>
	wstring to_cstring(const T& in);

};

template<class in_T, class out_T>
out_T CvtString::convert(const in_T& in)
{
	stringstream stream;
	stream << in;
	out_T result;
	stream >> result;
	return result;
}

template<class T>
T CvtString::cstring_to(const wstring& str)
{
	wstringstream ostr;
	ostr << str;
	T result;
	ostr >> result;
	return result;
}

template<class T>
wstring CvtString::to_cstring(const T& in)
{
	/*wostringstream ostr;*/
	wstringstream ostr;
	ostr << in;
	return ostr.str();
}



#endif

