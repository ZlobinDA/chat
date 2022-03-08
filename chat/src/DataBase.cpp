// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "DataBase.h"
#include <iostream>

bool DataBase::check_database() {
	std::string logMessage;
	// Подключаемся к серверу. Для этого устанавливаем соединение с базой данных по умолчанию.
	if (!mysql_real_connect(&_descriptor,
		_host.c_str(), _user.c_str(), _password.c_str(), NULL, NULL, NULL, 0)) {
		std::string error = mysql_error(&_descriptor);
		logMessage = "Ошибка при подключении к серверу с базой данных: " + error;
		_log.write(logMessage);
		return false;
	}
	// Пытаемся создать базу данных. Если база данных существует, она не будет перезаписана.
	std::string query = "create database if not exists " + _name;
	int result = mysql_query(&_descriptor, query.c_str());
	return true;
}

bool DataBase::make_connection() {
	std::string logMessage;
	// Повторно иницилизируем дискриптор, т.к. по предыдущему значению дескриптора
	//  не получится повторно подключиться.
	mysql_init(&_descriptor);
	logMessage = "Подключаемся к базе данных " + _name;
	_log.write(logMessage);
	if (!mysql_real_connect(&_descriptor,
		_host.c_str(), _user.c_str(), _password.c_str(), _name.c_str(), NULL, NULL, 0)) {
		std::string error = mysql_error(&_descriptor);
		logMessage = "Ошибка при подключении к базе данных: " + error;
		_log.write(logMessage);
		return false;
	}
	else {
		logMessage = "Выполнено подключение к базе данных " + _name;
		_log.write(logMessage);
		return true;
	}

}

DataBase::DataBase() : _isConnected(false), _log("DataBase") {
	std::string logMessage;
	logMessage = "Вызван конструктор";
	_log.write(logMessage);
	logMessage = "Инициализируем дескриптор соединения с базой данных";
	_log.write(logMessage);
	mysql_init(&_descriptor);
	if (&_descriptor == nullptr) {
		std::string error = mysql_error(&_descriptor);
		logMessage = "Ошибка при инициализации дескриптора соединения с базой данных: " + error;
		_log.write(logMessage);
		return;
	}
}

DataBase::~DataBase() {
	std::string logMessage = "Закрываем соединение с базой данных " + _name;
	_log.write(logMessage);
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
		return;
	}
	// Подключаемся к базе данных.
	if (!make_connection()) {
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
	if (res = mysql_store_result(&_descriptor)) {
		if (row = mysql_fetch_row(res)) {
			logMessage = "Найдена таблица users с данными пользователей";
			_log.write(logMessage);
		}
		else {
			logMessage = "Создаем таблицу users с данными пользователей";
			_log.write(logMessage);
			/**
			* Таблица "Пользователи" содержит следующие данные:
			*  - id пользователя. id является первичным ключом;
			*  - имя пользователя;
			*  - логин пользователя;
			*  - пароль пользователя. Пароль хранится в незашифрованом виде.
			*/
			mysql_query(&_descriptor, "create table users(id int auto_increment primary key,"
				"name varchar(255),"
				"login varchar(255) not null, "
				"password varchar(255)) not null");
		}
	}
}

void DataBase::check_messagesTable() {
	std::string logMessage;
	mysql_query(&_descriptor, "show tables like 'messages'");
	MYSQL_RES* res;
	MYSQL_ROW row;
	if (res = mysql_store_result(&_descriptor)) {
		if (row = mysql_fetch_row(res)) {
			logMessage = "Найдена таблица messages с сообщениями пользователей";
			_log.write(logMessage);
		}
		else {
			logMessage = "Создаем таблицу messages с сообщениями пользователей";
			_log.write(logMessage);
			/**
			* Таблица "Сообщения" содержит следующие данные:
			*  - id сообщения. id является первичным ключом;
			*  - id отправителя;
			*  - id получателя;
			*  - текст сообщения.
			*/
			mysql_query(&_descriptor, "create table messages(id int auto_increment primary key,"
				"sender_id int, "
				"reciever_id int, "
				"message varchar(1024))");
		}
	}
}

std::vector <std::string> DataBase::get_data(const std::string& query) {
	mysql_query(&_descriptor, query.c_str());
	MYSQL_RES* res;
	MYSQL_ROW row;
	std::vector <std::string> result;
	if (res = mysql_store_result(&_descriptor)) {
		while (row = mysql_fetch_row(res)) {
			result.push_back(row[0]);
		}
	}
	return result;
}

std::string DataBase::get_id(const std::string& login) {
	std::string logMessage;
	logMessage = "Запрос id пользователя по его логину";
	_log.write(logMessage);
	std::string query = "select id from users where login = '" + login + "'";
	std::string id = get_data(query).at(0);
	return id;
}

void DataBase::add_user(const std::string& name, const std::string& login, const std::string& password) {
	std::string logMessage;
	std::string dbString = "insert into users(id, name, login, password) values(default, '" +
		name + "', '" + login + "', '" + password + "')";
	int result = mysql_query(&_descriptor, dbString.c_str());
	if (result == 0) {
		logMessage = "Пользователь " + name + "успешно добавлен в базу данных";
		_log.write(logMessage);
	}
	else {
		logMessage = "Ошибка! Пользователь " + name + "не был добавлен в базу данных";
		_log.write(logMessage);
	}
}

bool DataBase::find_user(const std::string& login) {
	std::string logMessage = "Ищем пользователя " + login + " в базе данных";
	_log.write(logMessage);
	std::string query = "select id from users where login='" + login + "'";
	if (get_data(query).size() == 0) {
		logMessage = "Пользователь не найден";
		_log.write(logMessage);
		return false;
	}
	else {
		logMessage = "Пользователь найден";
		_log.write(logMessage);
		return true;
	}
}

int DataBase::users_count() {
	// Считаем, что число пользователей в базе данных
	//  равно числу записей в таблице.
	std::string logMessage;
	logMessage = "Запрос числа пользователей в базе данных";
	_log.write(logMessage);
	std::string query = "select count(id) from users";
	int count = std::stoi(get_data(query).at(0));
	return count;
}

std::vector <std::string> DataBase::get_users() {
	std::string logMessage = "Запрос списка пользователей в базе данных";
	_log.write(logMessage);
	std::string query = "select login from users";
	return get_data(query);
}

bool DataBase::check_password(std::string& login, std::string& password) {
	std::string logMessage = "Проверяем пароль для пользователя с логином " + login;
	_log.write(logMessage);
	std::string query = "select password from users where login='" + login + "'";
	if (get_data(query).at(0) == password) {
		return true;
	}
	else {
		return false;
	}
}

std::string DataBase::get_name(const std::string& login) {
	std::string logMessage = "Запрос имени пользователя по его логину";
	_log.write(logMessage);
	std::string query = "select name from users where login='" + login + "'";
	return get_data(query).at(0);
}

void DataBase::add_message(const std::string& sender, const std::string& reciever, const std::string& message) {
	std::string logMessage = "Добавляем сообщение в базу данных";
	_log.write(logMessage);
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
		_log.write(logMessage);
	}
	else {
		std::string error = mysql_error(&_descriptor);
		logMessage = "Ошибка! Сообщение не было добавлен в базу данных: " + error;
		_log.write(logMessage);
	}
}

std::vector<std::string> DataBase::get_messages() {
	std::string logMessage = "Запрос сообщений из базы данных";
	_log.write(logMessage);
	std::string query = "select login from users";
	return get_data(query);
}
