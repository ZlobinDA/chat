// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "Chat.h"
#include <iostream>

auto main()->int {
#if defined (_WIN64) || (_WIN32)
	setlocale(LC_ALL, ".UTF8");
	std::cout << "ОС Windows. Задаем в консоле кодировку UTF8" << std::endl;
#endif
	
	Chat chat;
	return chat.run();
}