#pragma once
#include <iostream>
#include "ChatConstants.h"
#include <string>

//class ChatGroup;

class ChatUser								// Пользователь для включения в состав ChatUsersList
{
public:
	ChatUser(std::string nickname, std::string fullname, std::string password);
	bool verifyRegistration() const;		// Проверка имени, ника, пароля пользователя на допустимость
	bool registerUser(USRNGRPIDTYPE id);	// Регистрация по списку в ChatUsersList

	bool isRegistered() const;
	std::string getNickname() const;
	std::string getFullname() const;
	std::string getPassword() const;
	USRNGRPIDTYPE getId() const;
	std::string getInfo() const;
	friend std::ostream& operator<<(std::ostream& output, const ChatUser* chatUser);

private:
	std::string _nickname;
	std::string _fullname;
	std::string _password;
	USRNGRPIDTYPE _id;
	bool _registered;
	
	USRNGRPIDTYPE** _groupsIndexes;			// Список групп, в которые включён пользователь ->
	USRNGRPIDTYPE _groupsCount;				//			-> функционал не реализован! ->
											// TODO:	-> добавить методы addToGroup(id) и т.п.
};

