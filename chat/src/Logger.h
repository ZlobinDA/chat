/**
*
* @file
* @brief Класса для работы с журналом сообщений.
*
* Сообщения записываются в указанный файл.
* Сообщения могут быть записаны на русском или английском языках.
* Перед сообщением может быть добавлен префик - метка объекта, который записываает сообщение.
*/

#pragma once

#include "Language.h"

#include <fstream>
#include <mutex>
#include <string>

class Logger {
private:
	std::string _path;		/**< путь до файла */
	std::fstream _log;		/**< объект класса ofstream, с которым связан файл журнала */
	Language _language;		/**< язык, используемые для вывода сообщений в журнал */
	std::mutex _mutex;		/**< мьютекс для защиты записи в файл */

	/** Открытие файла с журналом сообщений. */
	void open();
	/** Закрытие файла с журналом сообщений. */
	void close();
public:
	/**
	* Конструктор с параметрами. Для создания объекта с привязкой к файлу.
	* @param path - путь до файла;
	* @param language - язык сообщений.
	*/
	Logger(const std::string& path, const Language& language);
	/** Деструктор */
	~Logger();

	/**
	* Запись сообщений в журнал.
	* @param message - сообщение для записи в файл.
	*/
	void operator << (const std::string& message);

	/**
	* Чтение строки из журнала.
	* @return - строка из файла.
	*/
	std::string operator >> (const std::string& message);
};