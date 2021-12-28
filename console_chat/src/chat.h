#pragma once

#include "sha1.h" // uint*
#include <string> // string
#include <unordered_map> // unordered_map

// Максимальное число пользователей, которые могут зарегистрироваться в чате.
// Используем маленькое значение, чтобы протестировать работу чата при переполнении базы пользователей.
// Проверяем работу чата для 3 пользователей, т.к. 1 пользователь - администратор.
constexpr int maxUsers = 4;

class Chat {
private:
	// Структура для хранения данных пользователя.
	struct UserData {
		std::string _name;
		uint* _password;

		UserData();
		UserData(const UserData& other);
		UserData(const std::string& password, const std::string& name = "noName");
		~UserData();

		UserData& operator = (const UserData& other);
	};

	// Используем ассоциативный контейнер unordered_map для хранения базы данных пользователей:
	//	- логин пользователя уникальный, поэтому храним данные в виде пары
	//	 ключ (логин) - значение (структура с данными о пользователе);
	//	- данные о пользователях можно хранить неупорядоченно;
	//	- быстрый поиск, вставка и удаление пользователя.
	std::unordered_map <std::string, UserData> _dataBase;

	/** Печать зашифрованного пароля пользователя. Метод для вывода отладочной информации. */
	void printPassword(const uint*);
public:
	Chat();
	~Chat();

	/** Добавление нового пользователя в базу пользователей чата. */
	void addUser(const std::string& login, const std::string& password, const std::string& name = "noName");

	/**
	* Метод проверяет соответствие введеного пользователем пароля
	*  и пароля, сохраненного при регистрации пользователя.
	*/
	bool checkPassword(const std::string& login, const std::string& password);

	/** Метод возвращает число пользователей, зарегистрированных в чате. */
	size_t getUsersCount() const;

	/**
	* Метод проверяет, существует ли пользователь с данным логином в базе чата.
	* Если пользователь найден метод возвращает true, иначе false.
	*/
	bool findLogin(const std::string& login) const;

	/** Метод возвращает имя пользователя по его логину. */
	std::string getName(const std::string& login) const;

	/** Метод возвращает массив с логинами зарегистрированных пользователей. */
	std::vector <std::string> getUsersList() const;
};