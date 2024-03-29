#pragma once

#include "Logger.h"

#if defined __linux__
#include <mysql/mysql.h>
#elif defined (_WIN32) || defined (_WIN64)
#include <mysql.h>
#endif
#include <fstream>
#include <string>
#include <vector>

/**
* ����� ��� ������ � ����� ������ ����.
*/
class DataBase {
private:
	MYSQL _descriptor;		// ���������� ���������� � ����� ������.
	std::string _host;		// ��� ����� ��� IP ����� ���� ������
	std::string _user;		// ��� ������������ ���� ������
	std::string _password;	// ������ ������������ dbUser
	std::string _name;		// ��� ���� ������
	bool _isConnected;		// ������ ����� � ����� ������: false - ���� ������ ����������, true - ��������.

	/** ������ � �������� ���������. */
	Logger* _log;			/**< ������ ��� ������ � �������� */

	// �������� ������� ���� ������.
	bool check_database();
	// ����������� � ������������ ���� ������.
	bool make_connection();
	// �������� ������� ������� � ������� � �������������.
	void check_usersTable();
	// �������� ������� ������� � ����������� �������������.
	void check_messagesTable();
	// ������ ������ �� �������.
	std::vector <std::string> get_data(const std::string& query);
	// ������ id ������������ �� ��� ������.
	std::string get_id(const std::string& login);
public:
	DataBase(Logger* log);
	~DataBase();

	// ����������� � ��.
	void connect(const std::string& host, const std::string& user, const std::string& password, const std::string& name);

	/** ������ ��� ������ � ��������������. */
	// ���������� ������������ � ��.
	void add_user(const std::string& name, const std::string& login, const std::string& password);
	// ����� ������������ � �� �� ������.
	bool find_user(const std::string& login);
	// ����� ������������� � ��.
	int users_count();
	// ������ ������������� �� ��.
	std::vector <std::string> get_users();
	// �������� ������������ ������ ������������ ��� ������.
	bool check_password(std::string& login, std::string& password);
	// ������ ����� ������������ �� ��� ������.
	std::string get_name(const std::string& login);

	/** ������ ��� ������ � �����������. */
	// ���������� ��������� � ���� ������.
	void add_message(const std::string& sender, const std::string& reciever, const std::string& message);
	// ���������� ��������� �� ���� ������.
	std::vector <std::vector<std::string>> get_messages();
};

