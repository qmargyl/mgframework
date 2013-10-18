#include "mgcomponent.h"
#include <sstream>
#include <iostream>

int MGComponent::m_IDGenerator = 0;
size_t MGComponent::m_allocatedMemory = 0;


bool MGComponent::detectCollisionRectangle(int x1, int y1, int x2, int y2, int a1, int b1, int a2, int b2)
{
	if(detectCollisionPointRectangle(x1, y1, a1, b1, a2, b2))
		return true;
	if(detectCollisionPointRectangle(x2, y2, a1, b1, a2, b2))
		return true;
	if(detectCollisionPointRectangle(a1, b1, x1, y1, x2, y2))
		return true;
	if(detectCollisionPointRectangle(a2, b2, x1, y1, x2, y2))
		return true;
	return false;
}

bool MGComponent::detectCollisionPointRectangle(int px, int py, int x1, int y1, int x2, int y2)
{ 
	if(x1 >= px && px >=x2 && y1 >= py && py >= y2)
		return true;
	if(x2 >= px && px >=x1 && y2 >= py && py >= y1)
		return true;
	return false;
}

std::string MGComponent::toString(int number)
{
	std::stringstream ss;	//create a stringstream
	ss << number;		//add number to the stream
	return ss.str();	//return a string with the contents of the stream
}

std::vector<std::string> MGComponent::split(std::string str, char c)
{
	std::string strTempString;
	std::vector<int> splitIndices;
	std::vector<std::string> splitLine;
    int nCharIndex = 0;
    int nLineSize = (int)str.size();

    // find indices
    for(int i = 0; i < nLineSize; i++)
    {
        if(str[i] == c)
		{
            splitIndices.push_back(i);
		}
    }
    splitIndices.push_back(nLineSize); // end index

    // fill split lines
    for(int i = 0; i < (int)splitIndices.size(); i++)
    {
        strTempString = str.substr(nCharIndex, (splitIndices[i] - nCharIndex));
        splitLine.push_back(strTempString);
        nCharIndex = splitIndices[i] + 1;
    }
	return splitLine;
}
