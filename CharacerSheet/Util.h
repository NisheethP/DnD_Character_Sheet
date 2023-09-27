#pragma once

namespace Util
{
	void toLowerString(std::string& str);
	std::string toLowerStringRet(std::string str);

	std::wstring to_wstring(std::string str);

	class RNG
	{
		std::mt19937 rng;

	public:
		RNG();
		int rollUniform(int min, int max);

	};

	static RNG RandomGen;
}