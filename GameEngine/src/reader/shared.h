#pragma once
#include "oxygine-framework.h"
using namespace oxygine;
//extern Resources res;

std::string IntToStr(int i);

template <class T>
T random(T min, T max)
{
	return static_cast<T>(rand() / 65536.f * (max - min)) + min;
};

template <class T>
T lerp(T min, T max, float f)
{
	return static_cast<T>((max - min) * f) + min;
};

int round(float a);

template <class T>
T abs(T v)
{
	if (v > 0)
	{
		return v;
	}
	return -v;
};
