#pragma once

namespace Util
{
	void toLowerString(std::string& str);
	std::string toLowerStringRet(std::string str);

	std::wstring to_wstring(std::string str);
	std::string to_string(std::wstring str);
}