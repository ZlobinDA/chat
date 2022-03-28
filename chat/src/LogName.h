#pragma once

#include <string>

class LogName
{
private:
	std::string _name;	/**< название журнала. Используется как префикс для каждого сообщения */
public:
	LogName(const std::string& name) : _name(name) {}
	~LogName() = default;

	std::string get_logName(const std::string& message) {
		return (_name + ": " + message);
	}
};

