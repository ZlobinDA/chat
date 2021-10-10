#pragma once

#include <exception>
#include <string>
using namespace std;

/** ���������������� ����� ���������� ��� ��������� ����������, ��������� ������������� � ������� �������. */
class chat_exception : public exception {
private:
	string _message;
public:
	chat_exception(string message) : _message(message) {}
	const char* what() const noexcept override {
		static string message = "";
		message = _message;
		return message.c_str();
	}
};
