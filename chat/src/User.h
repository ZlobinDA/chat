#pragma once

#include <fstream>
#include <cstring>
#include <string>

/**
* Класс пользователя чата.
*/
class User final {
private:
	std::string _name;		/** имя пользователя */
	std::string _login;		/** логин пользователя */
	std::string _password;	/** пароль пользователя */
	bool _isOnline;			/** статус пользователя: true - online, false - offline */
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
