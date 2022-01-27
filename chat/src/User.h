#pragma once

#include "sha1.h"
#include <fstream>
#include <string>

/**
* ����� ������������ ����.
*/
class User final {
private:
	std::string _name;	/** ��� ������������ */
	std::string _login;	/** ����� ������������ */
	std::string _fpassword; /** ����! ���������� ������ � ������������ � ����. */
	uint* _password;	/** ������ ������������ */
	bool _isOnline;		/** ������ ������������: true - online, false - offline */
public:
	User();
	User(const User& other);
	User(const std::string& name, const std::string& login, const std::string& password);
	~User();

	User& operator = (const User& other);

	void setName(const std::string& name);
	void setLogin(const std::string& login);
	void setFPassword(const std::string& fpassword);
	void setPassword();

	std::string getName() const;
	std::string getLogin() const;
	std::string getFPassword() const;
	uint* getPassword() const;

	bool getStatus() const;

	friend std::ostream& operator << (std::ostream& output, const User& object);
};
