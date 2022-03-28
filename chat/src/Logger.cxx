// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Language.h"
#include "Logger.h"

#include <chrono>
//#include <ctime>   // localtime
#include <iomanip> // put_times
#include <iostream>

Logger::Logger(const std::string& logName) : _path("main.log") {
#ifdef _DEBUG
	std::string message;
	if (_language.isEnglish()) message = "Logger: constructor is called";
	if (_language.isRussian()) message = "Logger: Вызван конструктор";
	// Внимание! Объект класса еще не создан, доступен вывод сообщения в консоль.
	std::cout << message << std::endl;
#endif
	std::call_once(_flag, &Logger::open, this);
}

Logger::~Logger() {
#ifdef _DEBUG
	std::string message;
	if (_language.isEnglish()) message = "Logger: destructor is called";
	if (_language.isRussian()) message = "Logger: Вызван деструктор";
	*this << message;
#endif
	if (_isOpen) {
		std::string message;
		if (_language.isEnglish()) message = "File is closed: " + _path;
		if (_language.isRussian()) message = "Закрыт файл: " + _path;;
		*this << message;

		_logFile.close();
	}
}

void Logger::open() {

	_logFile.open(_path, std::ios::out | std::ios::trunc);

	if (_logFile) {
		_isOpen = true;
		std::string message;
		if (_language.isEnglish()) message = "File is opened: " + _path;
		if (_language.isRussian()) message = "Открыт файл: " + _path;
		*this << message;
	}
	else {
		_isOpen = false;
#ifdef _DEBUG
		if (_language.isEnglish()) std::cout << "Error at file openning: " + _path;
		if (_language.isRussian()) std::cout << "Ошибка открытия файла: " + _path;
#endif
	}
}

void Logger::operator << (const std::string& message) {
	_mutex.lock();
#ifdef _DEBUG
	std::cout << message << std::endl;
#endif
	/** Проверяем, доступен ли файл */
	if (!getStatus(this)) return;
	/** Получаем текущее время в формате "год.месяц.дата часы:минуты:секунды". */
	auto timePoint = std::chrono::system_clock::now();
	auto timeT = std::chrono::system_clock::to_time_t(timePoint);
	auto timeObject = std::put_time(std::localtime(&timeT), "%Y-%m-%d %X");
	/** Печатаем сообщение с датой и временем в журнал. */
	_logFile << "[" << timeObject << "] - " << message << std::endl;
	_logFile.flush();
	_mutex.unlock();
}

bool Logger::getStatus(Logger* file) {
	if (file == 0) {
		return false;
	}
	else {
		return file->_isOpen;
	}
}

void Logger::close() {
	std::string message;
	if (_language.isEnglish()) message = "File is closed: " + _path;
	if (_language.isRussian()) message = "Закрыт файл: " + _path;
	*this << message;

	_logFile.close();
}
