// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "User.h"
#include <iostream>


User::User() : _isOnline(false) {
}

User::User(const User& other) {
	_name = other._name;
	_login = other._login;
	_password = other._password;
	_isOnline = other._isOnline;
}


User::User(const std::string& name, const std::string& login, const std::string& password) :
	_name(name), _login(login), _password(password), _isOnline(false) {}


User& User::operator = (const User& other) {
	// Выполняем проверку на присвоение объекта самому себе.
	if (this == &other) {
		return *this;
	}
	_name = other._name;
	_login = other._login;
	_password = other._password;
	return *this;
}

void User::setName(const std::string& name) {
	_name = name;
}

void User::setLogin(const std::string& login) {
	_login = login;
}

void User::setPassword(const std::string& password) {
	_password = password;
}

std::string User::getName() const {
	return _name;
}


std::string User::getLogin() const {
	return _login;
}

std::string User::getPassword() const {
	return _password;
}

bool User::getStatus() const {
	return _isOnline;
}
