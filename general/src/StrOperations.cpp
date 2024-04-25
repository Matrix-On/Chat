#include "strOperations.h"
#include <stdarg.h>


bool StrOperations::ExplodeStrToParts(const std::string& str, const std::string& separator, const int& numPart, std::string* part1, ...)
{
	int strLen = str.length();
	int sepLen = separator.length();
	if (strLen == 0 || sepLen == 0)
	{
		return false;
	}

	va_list vapos;
	va_start(vapos, part1);

	std::string* currPart = part1;
	
	int numberCurrPart = 0;
	int beginPos = -1;
	int count = 0;
	bool haveSeparator = true;

	for (int i = 0; i < strLen; ++i)
	{
		if (separator == str.substr(i, sepLen))
		{  
			if (count == 0)
			{
				currPart->clear();
			}
			else
			{
				(*currPart) = str.substr((beginPos < 0) ? 0 : beginPos, count);
			}

			haveSeparator = true;
			i += sepLen - 1;
			beginPos = -1;
			count = 0;
			++numberCurrPart;

			if (numberCurrPart < numPart)
			{
				currPart = va_arg(vapos, std::string*);
			}
			else
			{ 
				break;
			}
		}
		else
		{ 
			if (haveSeparator)
			{
				beginPos = i;
			}

			if (beginPos != -1)
			{
				++count;
			}

			haveSeparator = false;
		}
	}

	if (numberCurrPart < numPart)
	{
		*currPart = (beginPos != -1) ? str.substr(beginPos, count) : "";
		currPart = va_arg(vapos, std::string*);
	}
	++numberCurrPart;

	while (numberCurrPart < numPart)
	{
		currPart->clear();
		++numberCurrPart;
		currPart = va_arg(vapos, std::string*);
	}
	va_end(vapos);

    return true;
}

