// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "DataBase.h"

#include <iostream>

bool DataBase::check_database() {
	std::string logMessage;
	// Подключаемся к серверу. Для этого устанавливаем соединение с базой данных по умолчанию.
	if (!mysql_real_connect(&_descriptor,
		_host.c_str(), _user.c_str(), _password.c_str(), NULL, 0, NULL, 0)) {
		std::string error = mysql_error(&_descriptor);
		logMessage = "Ошибка при подключении к серверу с базой данных: " + error;
		*_log << logMessage;
		return false;
	}
	// Пытаемся создать базу данных. Если база данных существует, она не будет перезаписана.
	logMessage = "Создаем базу данных " + _name + ", если она не существует";
	*_log << logMessage;
	std::string query = "create database if not exists " + _name;
	int result = mysql_query(&_descriptor, query.c_str());
	if (result == 0) {
		logMessage = "База данных успешно создана или обнаружена";
		*_log << logMessage;
		return true;
	}
	return false;
}

bool DataBase::make_connection() {
	std::string logMessage;
	// Повторно иницилизируем дискриптор, т.к. по предыдущему значению дескриптора
	//  не получится повторно подключиться.
	mysql_init(&_descriptor);
	logMessage = "Подключаемся к базе данных " + _name;
	*_log << logMessage;
	if (!mysql_real_connect(&_descriptor,
		_host.c_str(), _user.c_str(), _password.c_str(), _name.c_str(), 0, NULL, 0)) {
		std::string error = mysql_error(&_descriptor);
		logMessage = "Ошибка при подключении к базе данных: " + error;
		*_log << logMessage;
		return false;
	}
	else {
		logMessage = "Выполнено подключение к базе данных " + _name;
		*_log << logMessage;
		return true;
	}

}

DataBase::DataBase(Logger* log) : _isConnected(false), _log(log) {
	std::string logMessage;
	logMessage = "Инициализируем дескриптор соединения с базой данных";
	*_log << logMessage;
	mysql_init(&_descriptor);
	if (&_descriptor == nullptr) {
		std::string error = mysql_error(&_descriptor);
		logMessage = "Ошибка при инициализации дескриптора соединения с базой данных: " + error;
		*_log << logMessage;
		return;
	}
}

DataBase::~DataBase() {
	std::string logMessage = "Закрываем соединение с базой данных " + _name;
	*_log << logMessage;
	mysql_close(&_descriptor);
}

void DataBase::connect(const std::string& host, const std::string& user,
	const std::string& password, const std::string& name) {
	_host = host;
	_user = user;
	_password = password;
	_name = name;
	// Проверяем существует ли база данных.
	if (!check_database()) {
		_isConnected = false;
		return;
	}
	// Подключаемся к базе данных.
	if (!make_connection()) {
		_isConnected = false;
		return;
	}
	// Задаем набор символов.
	mysql_set_character_set(&_descriptor, "utf8mb4");
	// Проверяем наличие таблицы с данными о пользователях. Если таблица отсутствует, создаем ее.
	check_usersTable();
	// Проверяем наличие таблицы с сообщениями пользователей. Если таблица отсутствует, создаем ее. 
	check_messagesTable();
	// После выполнения всех проверок, считаем, что соединение с базой данных установлено.
	_isConnected = true;
}

void DataBase::check_usersTable() {
	std::string logMessage;
	mysql_query(&_descriptor, "show tables like 'users'");
	MYSQL_RES* res;
	MYSQL_ROW row;
	if ((res = mysql_store_result(&_descriptor))) {
		if ((row = mysql_fetch_row(res))) {
			logMessage = "Найдена таблица users с данными пользователей";
			*_log << logMessage;
		}
		else {
			logMessage = "Создаем таблицу users с данными пользователей";
			*_log << logMessage;
			/**
			* Таблица "Пользователи" содержит следующие данные:
			*  - id пользователя. id является первичным ключом;
			*  - имя пользователя;
			*  - логин пользователя;
			*  - пароль пользователя. Пароль хранится в незашифрованом виде.
			*/
			int result = mysql_query(&_descriptor, "create table users(id int auto_increment primary key, "
				"name varchar(255), "
				"login varchar(255) not null, "
				"password varchar(255) not null)");
			if (result == 0) {
				logMessage = "Таблица users успешно создана";
				*_log << logMessage;
			}
			else {
				logMessage = "Ошибка при создании таблицы users";
				*_log << logMessage;
				_isConnected = false;
			}
		}
	}
}

void DataBase::check_messagesTable() {
	std::string logMessage;
	mysql_query(&_descriptor, "show tables like 'messages'");
	MYSQL_RES* res;
	MYSQL_ROW row;
	if ((res = mysql_store_result(&_descriptor))) {
		if ((row = mysql_fetch_row(res))) {
			logMessage = "Найдена таблица messages с сообщениями пользователей";
			*_log << logMessage;
		}
		else {
			logMessage = "Создаем таблицу messages с сообщениями пользователей";
			*_log << logMessage;
			/**
			* Таблица "Сообщения" содержит следующие данные:
			*  - id сообщения. id является первичным ключом;
			*  - id отправителя. Внешний ключ - соответствует первичному ключу id в таблице users;
			*  - id получателя. Внешний ключ - соответствует первичному ключу id в таблице users;
			*  - текст сообщения.
			*/
			mysql_query(&_descriptor, "create table messages(id int auto_increment primary key,"
				"sender_id int references users(id), "
				"reciever_id int references users(id), "
				"message varchar(1024))");
		}
	}
}

std::vector <std::string> DataBase::get_data(const std::string& query) {
	if (!_isConnected) return {};
	mysql_query(&_descriptor, query.c_str());
	MYSQL_RES* res;
	MYSQL_ROW row;
	std::vector <std::string> result;
	if ((res = mysql_store_result(&_descriptor))) {
		while ((row = mysql_fetch_row(res))) {
			result.push_back(row[0]);
		}
	}
	return result;
}

std::string DataBase::get_id(const std::string& login) {
	if (!_isConnected) return {};
	std::string logMessage;
	logMessage = "Запрос id пользователя по его логину";
	*_log << logMessage;
	std::string query = "select id from users where login = '" + login + "'";
	std::vector<std::string> id = get_data(query);
	if (id.size() > 0) {
		return id[0];
	}
	else {
		return {};
	}
}

void DataBase::add_user(const std::string& name, const std::string& login, const std::string& password) {
	if (!_isConnected) return;
	std::string logMessage;
	std::string dbString = "insert into users(id, name, login, password) values(default, '" +
		name + "', '" + login + "', '" + password + "')";
	int result = mysql_query(&_descriptor, dbString.c_str());
	if (result == 0) {
		logMessage = "Пользователь " + name + " успешно добавлен в базу данных";
		*_log << logMessage;
	}
	else {
		logMessage = "Ошибка! Пользователь " + name + "не был добавлен в базу данных";
		*_log << logMessage;
	}
}

bool DataBase::find_user(const std::string& login) {
	if (!_isConnected) return false;
	std::string logMessage = "Ищем пользователя " + login + " в базе данных";
	*_log << logMessage;
	std::string query = "select id from users where login='" + login + "'";
	if (get_data(query).size() == 0) {
		logMessage = "Пользователь не найден";
		*_log << logMessage;
		return false;
	}
	else {
		logMessage = "Пользователь найден";
		*_log << logMessage;
		return true;
	}
}

int DataBase::users_count() {
	if (!_isConnected) return 0;
	// Считаем, что число пользователей в базе данных
	//  равно числу записей в таблице.
	std::string logMessage;
	logMessage = "Запрос числа пользователей в базе данных";
	*_log << logMessage;
	std::string query = "select count(id) from users";
	std::vector<std::string> id = get_data(query);
	if (id.size() > 0) {
		return std::stoi(id[0]);
	}
	else {
		return 0;
	}
}

std::vector <std::string> DataBase::get_users() {
	if (!_isConnected) return {};
	std::string logMessage = "Запрос списка пользователей в базе данных";
	*_log << logMessage;
	std::string query = "select login from users";
	return get_data(query);
}

bool DataBase::check_password(std::string& login, std::string& password) {
	if (!_isConnected) return false;
	std::string logMessage = "Проверяем пароль для пользователя с логином " + login;
	*_log << logMessage;
	std::string query = "select password from users where login='" + login + "'";
	std::vector<std::string> _currentPassword = get_data(query);
	if (_currentPassword.size() > 0) {
		if (_currentPassword[0] == password) {
			return true;
		}
	}
	return false;
}

std::string DataBase::get_name(const std::string& login) {
	if (!_isConnected) return {};
	std::string logMessage = "Запрос имени пользователя по его логину";
	*_log << logMessage;
	std::string query = "select name from users where login='" + login + "'";
	return get_data(query).at(0);
}

void DataBase::add_message(const std::string& sender, const std::string& reciever, const std::string& message) {
	if (!_isConnected) return;
	std::string logMessage = "Добавляем сообщение в базу данных";
	*_log << logMessage;
	std::string sender_id = get_id(sender);
	std::string reciever_id;
	if (reciever != "all") {
		reciever_id = get_id(reciever);
	}
	else {
		// Пользователь с id = -1 не существует. Используем это значение
		//  для сообщений, адресованных в общий чат.
		reciever_id = "-1";
	}
	std::string dbString = "insert into messages(id, sender_id, reciever_id, message) values(default, '" +
		sender_id + "', '" + reciever_id + "', '" + message + "')";
	int result = mysql_query(&_descriptor, dbString.c_str());
	if (result == 0) {
		logMessage = "Сообщение успешно добавлен в базу данных";
		*_log << logMessage;
	}
	else {
		std::string error = mysql_error(&_descriptor);
		logMessage = "Ошибка! Сообщение не было добавлен в базу данных: " + error;
		*_log << logMessage;
	}
}

std::vector <std::vector<std::string>> DataBase::get_messages() {
	if (!_isConnected) return {};
	std::string logMessage = "Запрос сообщений из базы данных";
	*_log << logMessage;
	std::string query = "select sender_id, reciever_id, message from messages";
	mysql_query(&_descriptor, query.c_str());
	MYSQL_RES* res;
	MYSQL_ROW row;
	std::vector <std::string> tableRow;
	std::vector <std::vector<std::string>> tableData;
	if ((res = mysql_store_result(&_descriptor))) {
		while ((row = mysql_fetch_row(res))) {
			for (size_t i = 0; i < mysql_num_fields(res); ++i) {
				std::string cell = row[i];
				if (i == 0 || i == 1) {
					if (cell == "-1") {
						cell.clear();
						cell = "all";
					}
					else {
						std::string query = "select login from users where id = " + cell;
						cell = get_data(query).at(0);
					}
				}
				tableRow.push_back(cell);
			}
			tableData.push_back(tableRow);
			tableRow.clear();
		}
	}
	return tableData;
}
