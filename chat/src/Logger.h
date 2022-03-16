#pragma once

#include <string>

/** 
* ����� ��� ������ ��������� � ������.
*  � �������� ������� ������������ �������.
*  � ����������� ������� ����������� ���������� �������� ������� � �����.
*/
class Logger {
private:
	std::string _name;		/** �������� ��������. ������������ ��� ������� ��� ������� ���������. */
	std::string _message;	/** ��������� ��� ������ � ������. */
public:
	Logger() = delete;
	Logger(const std::string& logName);
	~Logger() = default;

	// ����� ��������� � ������.
	void write(const std::string& message);
};

