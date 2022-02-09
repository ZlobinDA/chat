#pragma once

#include <fstream>
#include <cstring>
#include <string>

/**
* ����� ������������ ����.
*/
class User final {
private:
	std::string _name;		/** ��� ������������ */
	std::string _login;		/** ����� ������������ */
	std::string _password;	/** ������ ������������ */
	bool _isOnline;			/** ������ ������������: true - online, false - offline */
public:
	User();
	User(const User& other);
	User(const std::string& name, const std::string& login, const std::string& password);
	~User() = default;

	User& operator = (const User& other);

	void setName(const std::string& name);
	void setLogin(const std::string& login);
	void setPassword(const std::string& password);

	std::string getName() const;
	std::string getLogin() const;
	std::string getPassword() const;

	bool getStatus() const;
};
