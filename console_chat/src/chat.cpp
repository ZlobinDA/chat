// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "chat.h"
#include <iomanip>
#include <iostream>

Chat::Chat() {
	// ��������� �������� ������������ - �������������� ����.
	addUser("root", "123");
}

Chat::~Chat() {
	std::cout << "������ ���������� ����" << std::endl;
}

void Chat::addUser(const std::string& login, const std::string& password, const std::string& name) {
	std::cout << "��������� ������������ � ���: " << "����� - " << login << ", ��� - " << name <<
		", ������ - " << password << std::endl;
	// ������� ������ ������������.
	UserData newUser(name, password);
	// ��������� ������ ������������ � ����.
	_dataBase[login] = newUser;
}

bool Chat::checkPassword(const std::string& login, const std::string& password) {
	std::cout << "��������� ���� ������������ � ���: " << "����� - " << login <<
		", ������ - " << password << std::endl;
	// ��������� ��� �� ����������� ������.
	std::cout << "��������� ������:" << std::endl;
	char* c_password = new char[password.size() + 1];
	for (size_t i = 0; i < password.size(); ++i) {
		c_password[i] = password.at(i);
	}
	uint c_size = static_cast <uint> (sizeof(char) * (password.size() + 1));
	uint* digest = sha1(c_password, c_size);
	printPassword(digest);
	// ��������� ����������� ��� ����������� ������ ������������.
	std::cout << "������������������ ������:" << std::endl;
	auto search = _dataBase.find(login);
	printPassword(search->second._password);
	// ���������� ��� � ���, ��� ��� �������� �� ������ ��� �����������.
	if (digest[0] == search->second._password[0] &&
		digest[1] == search->second._password[1] &&
		digest[2] == search->second._password[2] &&
		digest[3] == search->second._password[3] &&
		digest[4] == search->second._password[4]) {
		std::cout << "�������� ���� ������������ � ���" << std::endl;
		delete[] digest;
		return true;
	}
	else {
		std::cout << "������! ������������ ������" << std::endl;
		delete[] digest;
		return false;
	}
}

size_t Chat::getUsersCount() const {
	return _dataBase.size();
}

bool Chat::findLogin(const std::string& login) const {
	return (_dataBase.find(login) != _dataBase.end());
}

std::string Chat::getName(const std::string& login) const {
	auto search = _dataBase.find(login);
	if (search != _dataBase.end()) {
		return search->second._name;
	}
	else {
		return "Error";
	}
}

std::vector<std::string> Chat::getUsersList() const {
	std::vector <std::string> array;
	for (auto &it : _dataBase) {
		// �������� �������������� �� ������ �������������.
		if (it.first != "root") array.push_back(it.first);
	}
	return array;
}

void Chat::printPassword(const uint* password) {
	for (int i = 0; i < 5; i++) {
		std::cout << std::hex << std::setw(8) << std::setfill('0') << password[i] << " ";
	}
	std::cout << std::endl;
}

Chat::UserData::UserData() : _name(""), _password(0) {}

Chat::UserData::UserData(const UserData& other) {
	_name = other._name;
	_password = other._password;
}

Chat::UserData::UserData(const std::string& name, const std::string& password) :
	_name(name) {
	char* c_password = new char[password.size() + 1];
	for (size_t i = 0; i < password.size(); ++i) {
		c_password[i] = password.at(i);
	}
	uint c_size = static_cast <uint> (sizeof(char) * (password.size() + 1));
	_password = sha1(c_password, c_size);
	delete[] c_password;
}

Chat::UserData::~UserData() {
	delete[] _password;
}

Chat::UserData& Chat::UserData::operator = (const UserData& other) {
	// ��������� �������� �� ���������� ������� ������ ����.
	if (this == &other) {
		return *this;
	}
	_name = other._name;
	delete[] _password;
	_password = new uint[SHA1HASHLENGTHUINTS];
	memcpy(_password, other._password, SHA1HASHLENGTHBYTES);
	return *this;
}
