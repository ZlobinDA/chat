// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "Logger.h"
#include <iostream>

Logger::Logger(const std::string& logName) : _name(logName) {
	std::cout << "Logger: Вызван конструктор" << std::endl;
}

void Logger::write(const std::string& message) {
#ifdef _DEBUG
	std::cout << _name << ": " << message << std::endl;
#endif
}
