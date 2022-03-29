// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "Chat.h"
#include "ChatGetline.h"

#include <cstring>
#include <iostream>
#include <thread>

#if defined (__linux__)
#include <sys/utsname.h>
#endif

Chat::Chat(const std::string& rootLogin, const std::string& rootPassword,
	const std::string& net_IP, const uint16_t net_port,
	const std::string& dataBase_host, const std::string& dataBase_user, const std::string& dataBase_password, const std::string& dataBase_name) :
	_userOnline(false),
	_language(LanguageType::russian), _mainLog("main.log", _language), _messageLog("message.log", _language),
	_rootLogin(rootLogin), _rootPassword(rootPassword),
	_net(&_mainLog), _net_IP(net_IP), _net_port(net_port),
	_dataBase(&_mainLog), _dataBase_host(dataBase_host), _dataBase_user(dataBase_user), _dataBase_password(dataBase_password), _dataBase_name(dataBase_name) {
	// Печатаем информацию об операционной системе, в которой запущен чат.
	show_config();
	// Подключаемся к базе данных.
	std::thread thread_db(&Chat::connect_db, this);
	// Подключаемся к серверу.
	std::thread thread_net(&Chat::connect_net, this);

	if (thread_db.joinable()) {
		thread_db.join();
	}
	if (thread_net.joinable()) {
		thread_net.join();
	}
}

Chat::~Chat() {
}

int Chat::run() {
	std::cout << std::endl;
	std::cout << "Добро пожаловать в локальный консольный чат!" << std::endl;
	for (;;) {
		std::cout << std::endl;
		std::cout << "Выберите действие:" << std::endl;
		std::cout << "\t 1. Я новый пользователь. Хочу зарегистрироваться" << std::endl;
		std::cout << "\t 2. Я зарегистрированный пользователь. Войти в чат" << std::endl;
		std::cout << "\t 3. Завершить работу приложения" << std::endl;
		// Возможность писать сообщение в чат появляется при наличии 1-ого пользователя в чате.
		if (_userOnline) {
			std::cout << "\t 4. Написать сообщение в общий чат" << std::endl;
			std::cout << "\t 5. Написать личное сообщение пользователю" << std::endl;
			std::cout << "\t 6. Проверить почту" << std::endl;
		}
		// Выводим список пользователей, зарегистрированных в чате. Данные о наличии администратора не выводим.
		if (_dataBase.users_count() > 1) {
			std::string Users = "Пользователи, зарегистрированные в чате: ";
			std::vector <std::string> logins = _dataBase.get_users();
			// Данные о наличии администратора не выводим.
			for (size_t i = 0; i < logins.size(); ++i) {
				Users = Users + " " + logins[i];
			}
			std::cout << Users << std::endl;
		}
		else {
			std::cout << "В чате еще никто не зарегистрировался" << std::endl;
		}

		switch (chatGetline<int>()) {
		case registrationState:
			// Регистрация нового пользователя.
			registration();
			break;
		case enterState:
			// Вход в чат.
			enter();
			break;
		case writePublicState:
			// Написать в общий чат.
			writePublic();
			break;
		case writePrivateState:
			// Напичать личное сообщение.
			writePrivate();
			break;
		case checkMailState:
			checkMail();
			break;
		case terminateState:
			// Завершение работы приложения.
			if (terminate()) return 1;
			break;
		default:
			break;
		}
	}
}

void Chat::show_config() {
	std::string logMessage;
	// ОС Windows
#if defined (_WIN32) || defined (_WIN64)
	logMessage = "OS name: Windows";
	_mainLog << logMessage;
#endif
	// ОС Linux
#if defined(__linux__)
	struct utsname _utsname;
	uname(&_utsname);
	std::string property = _utsname.sysname;
	logMessage = "OS name: " + property;
	_mainLog << logMessage;
	property = _utsname.release;
	logMessage = "OS release: " + property;
	_mainLog << logMessage;
	property = _utsname.version;
	logMessage = "OS version: " + property;
	_mainLog << logMessage;
#endif
}

void Chat::registration() {
	std::cout << "При вводе личной информации используйте буквы английского алфавита и цифры. "
		"Буквы русского алфавита и дополнительные символы запрещены!" << std::endl;

	std::cout << "Введите свое имя:" << std::endl;
	std::string name = chatGetline<std::string>();

	std::cout << "Придумайте логин:" << std::endl;
	std::string login = chatGetline();
	// Логин должен быть уникальным. Если логин уже существует, повторный запрос логина.
	if (_dataBase.find_user(login)) {
		std::cout << "Вы не можете зарегистрироваться! Пользователь [" << login
			<< "] уже зарегистрирован" << std::endl;
		return;
	}

	std::cout << "Введите свой пароль:" << std::endl;
	std::string password = chatGetline();

	// Добавляем нового пользователя в базу пользователей чата.
	_dataBase.add_user(name, login, password);
}

void Chat::enter() {
	// Администратор не может входить в чат и писать сообщения.
	if (_dataBase.users_count() < 2) {
		std::cout << "В чате еще нет зарегистрированных пользователей!" << std::endl;
		return;
	}
	std::cout << "Введите свой логин:" << std::endl;
	std::string login = chatGetline();;
	// Проверка наличия данного логина в базе.
	if (!_dataBase.find_user(login)) {
		std::cout << "Пользователь с данным логином не зарегистрирован!" << std::endl;
		return;
	}
	std::cout << "Введите свой пароль:" << std::endl;
	std::string password = chatGetline();;
	// Проверка соответствия пароля в базе и введенного пароля.
	if (_dataBase.check_password(login, password)) {
		std::cout << "Добро пожаловать, " << _dataBase.get_name(login) << "!" << std::endl;
		_currentUser = std::move(login);
		_userOnline = true;
	}
	else {
		std::cout << "Пароль введен не верно!" << std::endl;
	}
}

void Chat::writePublic() {
	// Запрашиваем сообщение у пользователя.
	std::cout << "Наберите текст сообщения. Для отправки нажмите клавишу Enter" << std::endl;
	std::string message = chatGetline();
	// Сохраняем сообщение в БД.
	_dataBase.add_message(_currentUser, "all", message);
	// Выводим в консоль.
	message = "[" + _currentUser + "] написал всем: " + message;
	std::cout << message << std::endl;
	_messageLog << message;
	// Отправляем сообщение в сокет.
	_net.sendMessage(message);
}

void Chat::writePrivate() {
	std::cout << "Выберите пользователя, которому хотите написать личное сообщение" << std::endl;
	std::vector <std::string> logins = _dataBase.get_users();
	for (size_t i = 0; i < logins.size(); ++i) {
		std::cout << "\t " << i << ". Написать сообщение пользователю " << logins[i] << std::endl;
	}
	size_t number = chatGetline<size_t>();;
	// Ищем введенный номер среди пользователей.
	std::string receiver = logins.at(number);
	std::cout << "Наберите текст сообщения для пользователя " << receiver << ". Для отправки нажмите клавишу Enter" << std::endl;
	std::string message = chatGetline();
	// Сохраняем сообщение в БД.
	_dataBase.add_message(_currentUser, receiver, message);
	// Выводим сообщение в консоль.
	message = "[" + _currentUser + "] отправил сообщение " + "[" + receiver + "]: " + message;
	std::cout << message << std::endl;
	_messageLog << message;
	// Отправляем сообщение в сокет.
	_net.sendMessage(message);
}

void Chat::checkMail() {
	std::string logMessage = "Проверяем почту";
	_mainLog << logMessage;
	// Считываем сообщение из сокета
	std::string message = _net.getMessage();
	// Выводим сообщение в консоль
	std::cout << message << std::endl;
}


bool Chat::terminate() {
	std::cout << "Введите пароль администратора:" << std::endl;
	std::string password = chatGetline();
	std::string login = "root";
	if (_dataBase.check_password(login, password)) {
		std::cout << "Работа чата завершена" << std::endl;
		return true;
	}
	else {
		std::cout << "Неправильный пароль!" << std::endl;
		return false;
	}
}

void Chat::connect_net() {
	std::string logMessage;
	if (_language.isEnglish()) logMessage = "Connect to the server";
	if (_language.isRussian()) logMessage = "Подключаемся к cерверу";
	_mainLog << logMessage;

	_net.config(_net_IP, _net_port);
}

void Chat::connect_db() {
	std::string logMessage;
	if (_language.isEnglish()) logMessage = "Connect to data base";
	if (_language.isRussian()) logMessage = "Подключаемся к базе данных";
	_mainLog << logMessage;

	_dataBase.connect(_dataBase_host, _dataBase_user, _dataBase_password, _dataBase_name);
	// В пустой базе данных отсутствует пользователь с правами администратора.
	// Для пустой базы данных добавляем администратора.
	if (!_dataBase.find_user("root")) {
		logMessage = "Добавляем пользователя с правами администратора";
		_mainLog << logMessage;
		_dataBase.add_user(_rootLogin, _rootLogin, _rootPassword);
	}
	logMessage = "Считываем историю сообщений из базы данных";
	_mainLog << logMessage;
	// Собщения из БД передаются в виде массива, каждая строка которого
	//  имеет следующий формат:
	// 1 элемент: имя отправителя;
	// 2 элемент: имя получателя;
	// 3 элемент: текст сообщения.
	std::vector <std::vector<std::string>> messages = _dataBase.get_messages();
	for (size_t i = 0; i < messages.size(); ++i) {
		std::string consoleMessage;
		std::string sender = (messages[i])[0];
		std::string receiver = (messages[i])[1];
		std::string message = (messages[i])[2];
		if (receiver == "all") {
			consoleMessage = "[" + sender + "] написал всем: " + message;
		}
		else {
			consoleMessage = "[" + receiver + "] у вас личное сообщение от пользователя " +
				"[" + sender + "]: " + message;
		}
		std::cout << consoleMessage << std::endl;
	}


	std::string line;
	while (true) {
		_messageLog >> line;
	}
}
