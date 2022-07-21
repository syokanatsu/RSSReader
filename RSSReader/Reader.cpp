#include "Reader.h"
#include <tinyxml2.h>

bool Channel::Parse(const char* str)
{
	tinyxml2::XMLDocument xml;
	auto err = xml.Parse(str);


	return false;
}
