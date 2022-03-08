#pragma once

#include "Logger.h"

#include <mysql.h>
#include <string>
#include <vector>

/**
* Класс для работы с базой данных чата.
*/
class DataBase {
private:
	MYSQL _descriptor;		// дескриптор соединения с базой данных.
	std::string _host;		// имя хоста или IP адрес базы данных
	std::string _user;		// имя пользователя базы данных
	std::string _password;	// пароль пользователя dbUser
	std::string _name;		// имя базы данных
	bool _isConnected;		// статус связи с базой данных: false - база данных недоступна, true - доступна.
	Logger _log;			// журнал

	// Проверка наличие базы данных.
	bool check_database();
	// Подключение к существующей базе данных.
	bool make_connection();
	// Проверка наличия таблицы с данными о пользователях.
	void check_usersTable();
	// Проверка наличия таблицы с сообщениями пользователей.
	void check_messagesTable();
	// Запрос данных из таблицы.
	std::vector <std::string> get_data(const std::string& query);
	// Запрос id пользователя по его логину.
	std::string get_id(const std::string& login);
public:
	DataBase();
	~DataBase();

	// Подключение к БД.
	void connect(const std::string& host, const std::string& user, const std::string& password, const std::string& name);

	/** Методы для работы с пользователями. */
	// Добавление пользователя в БД.
	void add_user(const std::string& name, const std::string& login, const std::string& password);
	// Поиск пользователя в БД по логину.
	bool find_user(const std::string& login);
	// Число пользователей в БД.
	int users_count();
	// Список пользователей из БД.
	std::vector <std::string> get_users();
	// Проверка соответствия пароля пользователя его логину.
	bool check_password(std::string& login, std::string& password);
	// Запрос имени пользователя по его логину.
	std::string get_name(const std::string& login);

	/** Методы для работы с сообщениями. */
	// Добавление сообщения в базу данных.
	void add_message(const std::string& sender, const std::string& reciever, const std::string& message);
	// Извлечение сообщения из базы данных.
	std::vector <std::string> get_messages();
};

