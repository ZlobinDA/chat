#pragma once

#include "sha1.h"
#include <fstream>
#include <string>

/**
*  ласс пользовател€ чата.
*/
class User final {
private:
	std::string _name;	/** им€ пользовател€ */
	std::string _login;	/** логин пользовател€ */
	std::string _fpassword; /** “ест! —охранение данных о пользователе в файл. */
	uint* _password;	/** пароль пользовател€ */
	bool _isOnline;		/** статус пользовател€: true - online, false - offline */
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
