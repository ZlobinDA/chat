// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

/**
*
* @file
* @brief Описание класса для работы с журналом сообщений.
*
*/

#pragma once

#include "Language.h"

#include <fstream>
#include <mutex>
#include <string>

class Logger {
private:
	std::ofstream _logFile;	/**< объект класса ofstream, с которым связан файл журнала */
	std::string _path;		/**< путь до файла журнала */
	bool _isOpen;			/**< статус файла журнала: true - открыт, false - закрыт */
	Language _language;		/**< язык, используемые для вывода сообщений в журнал */
	std::mutex _mutex;		/**<  */
	std::once_flag _flag;	/**<  */
public:
	/**
	* Конструктор с параметрами. Для создания объекта с привязкой к файлу.
	* @param path - путь до файла журнала.
	*/
	Logger(const std::string& logName);

	/**
	* Конструктор копирования запрещен.
	*/
	Logger(const Logger&) = delete;

	/**
	* Конструктор присваивания запрещен.
	*/
	Logger& operator=(const Logger&) = delete;

	/** Деструктор. Выполняет закрытие файла. */
	~Logger();

	/**
	* Открытие файла журнала.
	*/
	void open();

	/**
	* Запись сообщений в журнал. Сообщения могут печататься в файл и выводиться в консоль.
	* @param message - сообщение для записи в файл.
	*/
	void operator << (const std::string& message);

	/**
	* Состояние файла.
	* @param file - указатель на проверяемый файл.
	* @return true - файл открыт и доступен для записи, false - проблемы с файлом.
	*/
	static bool getStatus(Logger* file);

	/** Закрытие файла журнала. */
	void close();

};