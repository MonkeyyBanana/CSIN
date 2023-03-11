#pragma once

#include <Windows.h>
#include <iostream>

namespace Utils {
	char* randomCharStr(int size) {
		char* out = (char*)malloc(sizeof(char) * size);
		static const char pool[] = { "ABCDEFGHIJKLMNOPQRSTUVWXYabcdefghijklmnopqrstuvwxyz1234567890!@#$%^&*()-_=+[{]};:,<.>/?`~ " };
		srand((unsigned)time(NULL) * 7);
		for (int i = 0; i < size; ++i)
			out[i] = pool[rand() % (sizeof(pool) - 1)];
		return out;
	}
}