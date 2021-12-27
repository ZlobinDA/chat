#pragma once
#include <stdio.h>
#include <iostream>
#include <fstream>

#include "ChatUser.h"
#include "ChatConstants.h"

constexpr char fileUserListPrefix[] = "DB_users";
constexpr char fileUserListPostfix[] = ".dat";

/*
Класс, содержащий список всех зарегистрированных в чате пользователей и основные методы для работы с ними.
*/

class ChatUsersList
{
public:
	ChatUsersList(const std::string& rootPassword); // По параметру "rootPassword" создаётся суперпользователь и выделяется память.
	~ChatUsersList();

	userNGroupIdType getUsersCount() const;
	bool isEmpty() const;
	bool isFull() const;

	std::string getNickname(const userNGroupIdType& id);
	std::string getFullname(const userNGroupIdType& id);
	bool addUser(ChatUser* newUser);								// Добавление зарегистрированного пользователя
	bool deleteUserById(const userNGroupIdType& id);
	userNGroupIdType findUserByNickname(const std::string& nickname) const;
	bool deleteUserByNickname(const std::string& nickname);
	void clearList();												// Удаляем всех, кроме root
	bool checkPassword(const userNGroupIdType& id, const std::string& password) const;

	bool saveToFile();												// Сохранение базы в текстовый файл
	bool loadFromFile();											// Чтение базы из текстового файла
	// TODO: перейти на бинарные файлы

private:
	ChatUser** _usersList;
	userNGroupIdType _usersCount;

};

