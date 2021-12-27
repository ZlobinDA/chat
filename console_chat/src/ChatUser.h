#pragma once
#include <iostream>
#include "ChatConstants.h"
#include <string>


/*
Класс, содержащий данные об отдельном пользователе и основные методы для работы с ними.
Инициализация полей происходит в конструкторе.
Метод регистрации registerUser необходимо вызывать перед дальнейшим использованием в составе класса "ChatUsersList".
*/

class ChatUser								// Пользователь для включения в состав ChatUsersList
{
public:
	ChatUser(const std::string& nickname, const std::string& fullname, const std::string& password);
	ChatUser(const std::string& rootPassword);
	bool registerUser(const userNGroupIdType& id);	// Регистрация по списку в ChatUsersList
											// TODO: можно сделать void, если поменять интерфейсную часть
	ChatUser operator--();					// Уменьшаем идентификатор _id - нужен для сдвигов в ChatUsersList

	bool isRegistered() const;
	std::string getNickname() const;
	std::string getFullname() const;
	std::string getPassword() const;		// TODO: небезопасно, убрать после добавления хеширования
	userNGroupIdType getId() const;
	bool checkPassword(const std::string& password) const;
	std::string getInfo() const;
	friend std::ostream& operator<<(std::ostream& output, const ChatUser* chatUser); // Для отладки

private:
	std::string _nickname;
	std::string _fullname;
	std::string _password;
	userNGroupIdType _id; // Индентификатор, совпадающий с индексом в ChatUsersList 
						// TODO: решить, нужен ли он отдельно
	bool _registered; // Успех прохождения registerUser
	
	bool verifyRegistration() const;		// Проверка имени, ника, пароля пользователя на допустимость

	//userNGroupIdType** _groupsIndexes;			// Список групп, в которые включён пользователь ->
	//userNGroupIdType _groupsCount;				//			-> функционал не реализован! ->
											// TODO:	-> добавить методы addToGroup(id) и т.п.
};

