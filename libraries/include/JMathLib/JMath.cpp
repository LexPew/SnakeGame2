#include "JMath.h"
#include <random>

int JMath::RandomInt(int p_Min, int p_Max)
{
	std::random_device rd;
	std::uniform_int_distribution<int> dist(p_Min, p_Max);
	return dist(rd);
}

float JMath::Lerp(float p_FirstValue, float p_FinalValue, float p_T)
{
	return (1 - p_T) * p_FirstValue + p_T * p_FinalValue;
}
