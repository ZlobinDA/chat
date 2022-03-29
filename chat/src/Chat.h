#pragma once

#include "DataBase.h"
#include "Net.h"
#include "Language.h"
#include "Logger.h"

#include <fstream>
#include <mutex>
#include <string>
#include <vector>

// Константы цикла по управлению работой чата
constexpr int registrationState = 1;
constexpr int enterState = 2;
constexpr int terminateState = 3;
constexpr int writePublicState = 4;
constexpr int writePrivateState = 5;
constexpr int checkMailState = 6;

class Chat final {
private:
	std::string _currentUser;	/** имя текущего пользователя, находящегося в чате */
	bool _userOnline;			/** признак наличия пользователя в чате */

	/** Работа с журналом сообщений. */
	Language _language;			/**< язык, используемые для вывода сообщений в журнал */
	Logger _mainLog;			/**< журнал основных событий */
	Logger _messageLog;			/**< журнал сообщений */

	/** Работа с сетью. */
	Net _net;					/** объект для работы с сетью */
	std::string _net_IP;		/** IP сервера */
	uint16_t _net_port;			/** номер порта сервера */
	// Подключение к серверу.
	void connect_net();

	/** Работа с базой данной. */
	std::string _rootLogin;			/** логин администратора */
	std::string _rootPassword;		/** пароль администратора */
	DataBase _dataBase;				/** объект БД */
	std::string _dataBase_host;		/** адрес БД */
	std::string _dataBase_user;		/** пользователь БД */
	std::string _dataBase_password;	/** пароль пользователя БД */
	std::string _dataBase_name;		/** имя БД */
	// Подключение к базе данных.
	void connect_db();

	/** Вывод в консоль названия и номера версии операционной системы, в которой запущен чат. */
	void show_config();

	/** Механика чата. */
	/** Регистрация нового пользователя в чате. */
	void registration();
	/** Вход пользователя в чат. */
	void enter();
	/** Написать в общий чат. */
	void writePublic();
	/** Написать сообщение пользователю. */
	void writePrivate();
	/** Проверить почту. */
	void checkMail();
	/** Завершение работы проиложения. */
	bool terminate();

public:
	Chat(const std::string& rootLogin, const std::string& rootPassword,
		const std::string& net_IP, const uint16_t net_port,
		const std::string& dataBase_host, const std::string& dataBase_user, const std::string& dataBase_password, const std::string& dataBase_name);
	~Chat();
	
	/** Метод для запуска чата. Работа чата организована в виде бесконечного цикла. */
	int run();
};