#pragma once
#include <stdio.h>
#include <iostream>
#include <fstream>

#include "ChatUser.h"
#include "ChatConstants.h"

#define FILEUSRLISTPREFIX "DB_users"
#define FILEUSRLISTPOSTFIX ".dat"

/*
Класс, содержащий список всех зарегистрированных в чате пользователей и основные методы для работы с ними.
*/

class ChatUsersList
{
public:
	ChatUsersList(std::string rootPassword); // По параметру "rootPassword" создаётся суперпользователь и выделяется память.
	~ChatUsersList();

	USRNGRPIDTYPE getUsersCount() const;
	bool isEmpty() const;
	bool isFull() const;

	std::string getNickname(USRNGRPIDTYPE id);
	std::string getFullname(USRNGRPIDTYPE id);
	bool addUser(ChatUser* newUser);								// Добавление зарегистрированного пользователя
	bool deleteUserById(USRNGRPIDTYPE id);
	USRNGRPIDTYPE findUserByNickname(std::string nickname) const;
	bool deleteUserByNickname(std::string nickname);
	void clearList();												// Удаляем всех, кроме root
	bool checkPassword(USRNGRPIDTYPE id, std::string password) const;

	bool saveToFile();												// Сохранение базы в текстовый файл
	bool loadFromFile();											// Чтение базы из текстового файла
	// TODO: перейти на бинарные файлы

private:
	ChatUser** _usersList;
	USRNGRPIDTYPE _usersCount;

};

