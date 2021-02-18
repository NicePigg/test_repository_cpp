// mystring.h: 标准系统包含文件的包含文件

// 或项目特定的包含文件。



#ifndef __MYSTRING__

#define __MYSTRING__

class String

{

public:

	String(const char* cstr = 0);

	String(const String& str);// 拷贝构造

	String& operator=(const String& str);// 拷贝赋值

	~String();// 析构函数

	char* get_c_str() const { return m_data; }

private:

	char* m_data;

};



#include <cstring>



inline 

String::String(const char* cstr)

{

	if (cstr) {// 非空字符串

		m_data = new char[strlen(cstr) + 1];

		strcpy(m_data, cstr);

	}

	else {// 空字符串

		m_data = new char[1];   

		*m_data = '\0';

	}

}



inline

String::~String()

{

	delete[] m_data;

}



inline

String& String::operator=(const String& str) // & 引用 原变量的别名

{

	if (this == &str)//自我赋值 1.效率高 2. 保证正确性  & 取地址 得到指针

		return *this;

	delete[] m_data;

	m_data = new char[strlen(str.m_data) + 1];

	strcpy(m_data, str.m_data);

	return *this;  // 返回*this 返回值String&

}

 

inline

String::String(const String& str)

{

	m_data = new char[strlen(str.m_data) + 1];

	strcpy(m_data, str.m_data);

}



#include <iostream>
using namespace std;

ostream& operator<<(ostream& os, const String& str)

{

	os << str.get_c_str();

	return os;

}



#endif
