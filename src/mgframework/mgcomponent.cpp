

#include <sstream>
#include <string>
#include <cstring>
#include <iostream>
#include "mgcomponent.h"

int MGComponent::m_IDGenerator = 0;

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
	std::stringstream ss;
	ss << number;
	return ss.str();
}

std::string MGComponent::toString(unsigned int number)
{
	std::stringstream ss;
	ss << number;
	return ss.str();
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

//TODO: This function needs more self-explaining variable names
std::vector<std::string> MGComponent::symbols(char *str)
{
	std::vector<std::string> splitLine;
	std::string sym("");
	char c[2] = {0, 0};
	char s[3] = {0, 0, 0};
	bool insideString = false;
	bool lineSupportingParameters = false;

	for(unsigned int i = 0; i < strlen(str); ++i)
	{
		if(i < strlen(str) - 1)
		{
			s[0] = str[i];
			s[1] = str[i + 1];
		}
		else
		{
			s[0] = 0;
			s[1] = 0;
		}

		if(str[i] == '"')
		{
			insideString = !insideString;
		}
		else if(!insideString && std::string(s) == "//")
		{
			if(sym != std::string(""))
			{
				if(splitLine.empty() && 
				  (	sym == std::string("call") ||
					sym == std::string("add") ||
					sym == std::string("pe") ||
					sym == std::string("mo") ||
					sym == std::string("delete")))
				{
					lineSupportingParameters = true;
				}
				splitLine.push_back(sym);
				sym = std::string("");
			}
			break;
		}
		else if(!insideString && (str[i] == ' ' || str[i] == '\t'))
		{
			if(sym != std::string(""))
			{
				if(splitLine.empty() && 
				  ( sym == std::string("call") ||
					sym == std::string("add") ||
					sym == std::string("pe") ||
					sym == std::string("mo") ||
					sym == std::string("delete")))
				{
					lineSupportingParameters = true;
				}
				splitLine.push_back(sym);
				sym = std::string("");
			}
		}
		else if(!insideString && (std::string(s) == "==" || std::string(s) == "<=" || std::string(s) == ">=" || 
				std::string(s) == "++" || std::string(s) == "--" || std::string(s) == "!=" || 
				std::string(s) == ">>" || std::string(s) == "<<"))
		{
			if(sym != std::string(""))
			{
				splitLine.push_back(sym);
				sym = std::string("");
			}
			splitLine.push_back(std::string(s));
			i++;
		}
		else if(!insideString && 
				!lineSupportingParameters &&
				(str[i] == '=' || str[i] == '(' || str[i] == ')' || str[i] == '+' || str[i] == '-' || 
				 str[i] == '*' || str[i] == '/' || str[i] == '|' || str[i] == '&' || str[i] == '<' || 
				 str[i] == '>'))
		{
			if(sym!=std::string(""))
			{
				splitLine.push_back(sym);
				sym = std::string("");
			}
			c[0] = str[i];
			splitLine.push_back(std::string(c));
		}
		else if(str[i] < 32)
		{
			// Ignore "special" characters..
		}
		else
		{
			c[0] = str[i];
			sym += std::string(c);
		}
	}
	if(sym != std::string(""))
	{
		// XXX: Is this really necessary?
		if(splitLine.empty() && 
		  ( sym == std::string("call") ||
			sym == std::string("add") ||
			sym == std::string("pe") ||
			sym == std::string("mo") ||
			sym == std::string("delete")))
		{
			lineSupportingParameters = true;
		}
		splitLine.push_back(sym);
	}

	return splitLine;
}