#pragma once

#include <exception>
#include <string>

/** Пользовательский класс исключений при обработке информации, введенной пользователем с помощью консоли. */
class chat_exception : public std::exception {
private:
	std::string _message;
public:
	chat_exception(std::string message) : _message(message) {}
	const char* what() const noexcept override {
		static std::string message = "";
		message = _message;
		return message.c_str();
	}
};
