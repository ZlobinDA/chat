// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Language.h"
#include "Logger.h"

#include <chrono>
//#include <ctime>   // localtime
#include <iomanip> // put_times
#include <iostream>

Logger::Logger(const std::string& path, const Language& language) : _path(path), _language(language) {
	open();
}

void Logger::open() {
	// Открываем файл для вывода информации.
	// Предыдущее содержимое файла будет удалено.
	_log.open(_path, std::ios::out | std::ios::in | std::ios::trunc);
	if (_log) {
		std::string logMessage;
		if (_language.isEnglish()) logMessage = "File is opened: " + _path;
		if (_language.isRussian()) logMessage = "Открыт файл: " + _path;
		*this << logMessage;
	}
	else {
#ifdef _DEBUG
		if (_language.isEnglish()) std::cout << "Error at file openning: " + _path;
		if (_language.isRussian()) std::cout << "Ошибка открытия файла: " + _path;
#endif // _DEBUG
	}
}

void Logger::close() {
	std::string logMessage;
	if (_language.isEnglish()) logMessage = "File is closed: " + _path;
	if (_language.isRussian()) logMessage = "Закрыт файл: " + _path;
	*this << logMessage;

	_log.close();
}

Logger::~Logger() {
	close();
}

void Logger::operator << (const std::string& message) {
	_mutex.lock();
	/** Проверяем, доступен ли файл */
	if (_log.is_open()) {
		/** Получаем текущее время в формате "год.месяц.дата часы:минуты:секунды". */
		auto timePoint = std::chrono::system_clock::now();
		auto timeT = std::chrono::system_clock::to_time_t(timePoint);
		auto timeObject = std::put_time(std::localtime(&timeT), "%Y-%m-%d %X");
		/** Печатаем сообщение с датой и временем в журнал. */
		_log << "[" << timeObject << "] - " << message << std::endl;
		_log.flush();
	}
	_mutex.unlock();
}

std::string Logger::operator>>(const std::string& message) {
	std::string line;
	_mutex.lock();
	/** Проверяем, доступен ли файл */
	if (_log.is_open()) {
		getline(_log, line);
	}
	_mutex.unlock();
	return line;
}
