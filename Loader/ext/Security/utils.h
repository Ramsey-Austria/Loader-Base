#pragma once
#include "../Includes/Includes.h"

namespace utils
{
	std::string randomstring(int length)
	{
		std::string result = { };
		std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

		srand((unsigned)time(0) * 5);

		for (unsigned int i = 0; i < length; i++)
			result += alphabet[rand() % (alphabet.length() - 1)];

		return result;
	}
}