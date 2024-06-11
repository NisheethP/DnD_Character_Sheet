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

Util::RNG::RNG()
{
	static auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	rng.seed(seed);
	rng.discard(10000);
}

int Util::RNG::rollUniform(int min, int max)
{
	std::uniform_int_distribution<int> dice(min, max);
	return dice(rng);
}
