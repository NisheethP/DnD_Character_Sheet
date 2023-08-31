#include "Util.h"
#include <locale>
#include <codecvt>

using convert_t = std::codecvt_utf8<wchar_t>;
std::wstring_convert<convert_t, wchar_t> strconverter;


void Util::toLowerString(std::string& str)
{
	for (auto it = str.begin(); it != str.end(); ++it)
		*it = std::tolower(*it);
}

std::wstring Util::to_wstring(std::string str)
{

	return strconverter.from_bytes(str);
}

std::string Util::to_string(std::wstring str)
{
	return strconverter.to_bytes(str);
}
