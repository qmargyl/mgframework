

#include <sstream>
#include <string>
//#include <stdio>
#include <iostream>

#include "mgcomponent.h"


int MGComponent::m_IDGenerator = 0;
size_t MGComponent::m_allocatedMemory = 0;


bool MGComponent::detectCollisionRectangle(int x1, int y1, int x2, int y2, int a1, int b1, int a2, int b2)
{
	if(detectCollisionPointRectangle(x1, y1, a1, b1, a2, b2))
	{
		return true;
	}
	if(detectCollisionPointRectangle(x2, y2, a1, b1, a2, b2))
	{
		return true;
	}
	if(detectCollisionPointRectangle(a1, b1, x1, y1, x2, y2))
	{
		return true;
	}
	if(detectCollisionPointRectangle(a2, b2, x1, y1, x2, y2))
	{
		return true;
	}
	return false;
}

bool MGComponent::detectCollisionPointRectangle(int px, int py, int x1, int y1, int x2, int y2)
{ 
	if(x1 >= px && px >=x2 && y1 >= py && py >= y2)
	{
		return true;
	}
	if(x2 >= px && px >=x1 && y2 >= py && py >= y1)
	{
		return true;
	}
	return false;
}

std::string MGComponent::toString(int number)
{
	std::stringstream ss;	//create a stringstream
	ss << number;		//add number to the stream
	return ss.str();	//return a string with the contents of the stream
}

std::vector<std::string> MGComponent::split(char *str, const char *c)
{
	// XXX: Add support for new data type, string
	//      loop through '"' characters
	//      between ith and (i+1)th, copy out the string and remove it from str (replace it with some marker?)
	//      merge strings with rest of the result somehow?
	std::vector<std::string> splitLine;
	char * pch;
	pch = strtok (str, c);
	while (pch != NULL)
	{
		splitLine.push_back(std::string(pch));
		pch = strtok (NULL, c);
	}
	return splitLine;
}
