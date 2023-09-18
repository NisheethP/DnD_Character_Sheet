#include "PreCompiledHeader.h"

#include "Util.h"
#include <locale>



void Util::toLowerString(std::string& str)
{
	for (auto it = str.begin(); it != str.end(); ++it)
		*it = std::tolower(*it);
}

std::string Util::toLowerStringRet(std::string str)
{
	for (auto it = str.begin(); it != str.end(); ++it)
		*it = std::tolower(*it);

	return str;
}

std::wstring Util::to_wstring(std::string str)
{
	return std::wstring(str.begin(), str.end());
}
