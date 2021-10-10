#pragma once
#include <stdio.h>
#include <iostream>
#include <fstream>

#include "ChatUser.h"
#include "ChatConstants.h"

#define FILEUSRLISTPREFIX "DB_users"
#define FILEUSRLISTPOSTFIX ".dat"

class ChatUsersList													// База зарегистрированных пользователей
{
public:
	ChatUsersList(std::string rootPassword);
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
	void clearList();
	bool checkPassword(USRNGRPIDTYPE id, std::string password) const;

	bool saveToFile();												// Сохранение базы в текстовый файл
	bool loadFromFile();											// Очистка и чтение базы из текстового файла
	// TODO: перейти на бинарные файлы

private:
	ChatUser** _usersList;
	USRNGRPIDTYPE _usersCount;

};

