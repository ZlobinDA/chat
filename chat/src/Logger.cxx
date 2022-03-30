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
	// Открываем файл для чтения и записи сообщений.
	// Новые сообщения будут дописаны в конец файла.
	// Попытка 1. Предполагаем, что файл существует.
	_log.open(_path, std::ios::out | std::ios::in | std::ios::ate);
	if (_log) {
		std::string logMessage;
		if (_language.isEnglish()) logMessage = "File is opened: " + _path;
		if (_language.isRussian()) logMessage = "Открыт файл: " + _path;
		*this << logMessage;
	}
	else {
		// Файл не найден. Открываем новый файл.
		_log.open(_path, std::ios::out);
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
		_log.clear();;
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

void Logger::operator >> (std::string& line) {
	// Читаем сообщение из файла, после чего запоминаем положение каретки.
	static std::streamoff position = 0;
	line.clear();
	_mutex.lock();
	if (_log.is_open() && position != -1) {
		_log.seekg(position);
		getline(_log, line);
		position = _log.tellg();
		_mutex.unlock();
		return;
	}
	// Тест. Считаем, что файл считывается от начала до конца.
	// Сбрасываем положение каретки, чтобы в файл можно было записывать инофрмацию.
	if (_log.is_open() && position == -1) {
		_log.seekg(0);
	}
	_mutex.unlock();
}
