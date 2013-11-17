#pragma once
#include <iostream>
#include <string>
#include <Windows.h>

void Error(std::string output)
{
	HANDLE h = GetStdHandle(STD_ERROR_HANDLE);
	SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
	std::cerr << output << std::endl;
	SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
}