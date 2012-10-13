#include "functions.h"

/////////////////////////////
long char2long(char* charVar)
{
	long longVar;
	std::istringstream in(charVar);
	in >> longVar;
	return  longVar;
}
/////////////////////////////
std::string long2str(long longVar)
{
	std::ostringstream out;
	out << longVar;
	return  out.str();
}
/////////////////////////////
char* long2char(long longVar)
{
	std::string strVar = long2str(longVar);
	char* charVar = const_cast<char*>(strVar.c_str());
	return charVar;
}
/////////////////////////////
char* str2char(std::string strVar)
{
	char* charVar = const_cast<char*>(strVar.c_str());
	return charVar;
}
/////////////////////////////
int bool2int(bool boolVar)
{
	int intVar;
	if (boolVar) {
		intVar = 1;
	}
	else {
		intVar = 0;
	}
	return intVar;
}

