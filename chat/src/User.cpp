// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "User.h"
#include <iostream>


User::User() : _password(nullptr), _isOnline(false) {
}

User::User(const User& other) {
	_name = other._name;
	_login = other._login;
	_fpassword = other._fpassword;
	_password = new uint[SHA1HASHLENGTHUINTS];
	memcpy(_password, other._password, SHA1HASHLENGTHBYTES);
	_isOnline = other._isOnline;
}


User::User(const std::string& name, const std::string& login, const std::string& password) :
	_name(name), _login(login), _fpassword(password), _isOnline(false) {
	char* c_password = new char[password.size() + 1];
	for (size_t i = 0; i < password.size(); ++i) {
		c_password[i] = password.at(i);
	}
	uint c_size = static_cast <uint> (sizeof(char) * (password.size() + 1));
	_password = sha1(c_password, c_size);
	delete[] c_password;
}


User& User::operator = (const User& other) {
	// Выполняем проверку на присвоение объекта самому себе.
	if (this == &other) {
		return *this;
	}
	_name = other._name;
	_login = other._login;
	_fpassword = other._fpassword;
	delete[] _password;
	_password = new uint[SHA1HASHLENGTHUINTS];
	memcpy(_password, other._password, SHA1HASHLENGTHBYTES);
	return *this;
}

void User::setName(const std::string& name) {
	_name = name;
}

void User::setLogin(const std::string& login) {
	_login = login;
}

void User::setFPassword(const std::string& fpassword) {
	_fpassword = fpassword;
}

void User::setPassword() {
	char* c_password = new char[_fpassword.size() + 1];
	for (size_t i = 0; i < _fpassword.size(); ++i) {
		c_password[i] = _fpassword.at(i);
	}
	uint c_size = static_cast <uint> (sizeof(char) * (_fpassword.size() + 1));
	_password = sha1(c_password, c_size);
	delete[] c_password;
}


User::~User() {
	delete[] _password;
}


std::string User::getName() const {
	return _name;
}


std::string User::getLogin() const {
	return _login;
}

std::string User::getFPassword() const {
	return _fpassword;
}


uint* User::getPassword() const {
	return _password;
}

bool User::getStatus() const {
	return _isOnline;
}

std::ostream& operator << (std::ostream& output, const User& object) {
	output << "Состояние объекта класса User: имя - " << object._name <<
	", пароль - " << object._password << std::endl;
	return output;
}
