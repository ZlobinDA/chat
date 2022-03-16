#pragma once

#include <string>

/** 
*  ласс дл€ вывода сообщений в журнал.
*  ¬ качестве журнала используетс€ консоль.
*  ¬ последующих верси€х планируетс€ добавление хранение журнала в файле.
*/
class Logger {
private:
	std::string _name;		/** название журанала. »спользуетс€ как префикс дл€ каждого сообщени€. */
	std::string _message;	/** сообщение дл€ записи в журнал. */
public:
	Logger() = delete;
	Logger(const std::string& logName);
	~Logger() = default;

	// ¬ывод сообщени€ в журнал.
	void write(const std::string& message);
};

