// mystring.cpp: 定义应用程序的入口点。
//
#include <iostream>
#include "mystring.h"

using namespace std;

int main()
{
	String s1("hello");
	String s2("world");
	String s3(s2);
	cout << s3 << endl;
	s3 = s1;
	cout << s3 << endl;
	cout << s2 << endl;
	cout << s1 << endl;
	return 0;
}
