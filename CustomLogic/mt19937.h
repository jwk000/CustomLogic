#pragma once
#include <random>
#include <algorithm>

class MT19937
{
	std::mt19937 generator;
public:
	void Seed(int seed) 
	{
		generator.seed(seed);
	}
	int RandomRange(int lo, int hi)
	{
		return (std::abs((int)generator() % (hi - lo + 1)) + lo);
	}
};
