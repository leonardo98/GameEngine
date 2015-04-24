#include "shared.h"

std::string IntToStr(int i)
{
	char buff[10];
	sprintf(buff, "%i", i);
	return buff;
}

int round(float a) {
	int b = static_cast<int>(a);
	return (a - b) >= 0.5f ? b + 1 : b;
} 