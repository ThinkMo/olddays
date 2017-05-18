#ifndef _STRING_H
#define _STRING_H

#include <cstring>
#include <utility>

class String
{

	char * data;

public:
	String():data(new char[1])
	{
		*data = '\0';
	}

	String(const char *str):data(new char[ strlen(str) + 1 ])
	{
		strcpy(data, str);
	}

	String(const String & rhs):data(new char[ rhs.size() + 1 ])
	{
		strcpy(data, rhs.c_str());
	}

	~String()
	{ delete [] data; }

	String & operator=(String rhs)
	{
		swap(rhs);
		return *this;
	}

	//c++ 11
	String(String && rhs):data(rhs.data)
	{
		rhs.data = nullptr;
	}

	String& operator=(String && rhs)
	{

		swap(rhs);
		return *this;
	}


	size_t size() const
	{
		return strlen(data);
	}

	const char * c_str() const
	{ return data; }

	void swap(String& rhs)
	{
		std::swap(data,rhs.data);
	}

};


#endif
