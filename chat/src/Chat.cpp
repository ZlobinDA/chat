// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "Chat.h"
#include "ChatGetline.h"
#include <iomanip>
#include <iostream>
#include <cstring>
#if defined (_WIN32) || defined (_WIN64)
#include <windows.h>
#include <stdio.h>
#pragma comment(lib, "user32.lib")
#endif
#if defined (__linux__)
#include <sys/utsname.h>
#endif


Chat::Chat() : _userOnline(false), _socketDescriptor(0), _port(0), _connection(0), _isServer(false) {
	std::cout << "Вызван конструктор чата" << std::endl;
	//
	showConfig();
	//
	netConfig();
	// Считываем базу сообщений чата.
	_messageDataBaseFile = new DataFile <std::vector <Message>>(&_messageDataBase, "messageData");
	// Считываем базу пользователей чата.
	_userDataBaseFile = new DataFile <std::vector <User>>(&_userDataBase, "userData");
	// Добавляем пользователей в чат.
	if (_userDataBase.size() > 0) {
		// Нашли пользовательскую базу данных. Считываем данные из базы.
		bool _readUserDB = true;	/** флаг чтения данных их базы данных. */
		for (size_t i{ 0 }; i < _userDataBase.size(); ++i) {
			User user = _userDataBase[i];
			add(user.getName(), user.getLogin(), user.getPassword(), _readUserDB);
		}
	}
	else {
		// Не нашли базу данных. Добавляем администратора.
		add("root", "root", "123");
	}
}

Chat::~Chat() {
	std::cout << "Закрываем сокет, завершаем соединение" << std::endl;
	close(_socketDescriptor);
	//
	delete _userDataBaseFile;
	//
	delete _messageDataBaseFile;
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
		if (getUsersCount() > 1) {
			std::string Users = "Пользователи, зарегистрированные в чате: ";
			std::vector <std::string> logins = getUsersList();
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


void Chat::getNetMessage() {
	char c_message[netMessageLen];
	bzero(c_message, sizeof(c_message));
	// Читаем сообщение из сокета.
	if (_isServer) {
		// Приложения является сервером.
		read(_connection, c_message, sizeof(c_message));
	}
	else {
		// Приложение является клиентом.
		read(_socketDescriptor, c_message, sizeof(c_message));
	}
	// Выводим сообщение в консоль.
	//  Внимание! Использую string исключительно ради удобства отладки
	std::string message = c_message;
	std::cout << message << std::endl;
}

void Chat::sendNetMessage(const std::string& message) {
	// Отправляем сообщение в сокет.
	ssize_t bytes = 0;
	if (_isServer) {
		// Приложения является сервером.
		bytes = write(_connection, message.c_str(), netMessageLen);
	}
	else {
		// Приложение является клиентом.
		bytes = write(_socketDescriptor, message.c_str(), netMessageLen);
	}
	if (bytes >= 0) {
		std::cout << "Сообщение успешно отправлено" << std::endl;
	}
}

void Chat::setMessage(const std::string& from, const std::string& to, const std::string& message) {
	Message newMessage;
	newMessage.setSender(from);
	newMessage.setReceiver(to);
	newMessage.setText(message);
	_messageDataBase.emplace_back(newMessage);
	_messageDataBaseFile->writeState();
}


void Chat::showConfig() {
	// ОС Windows
#if defined (_WIN32) || defined (_WIN64)

#endif
	// ОС Linux
#if defined(__linux__)
	struct utsname _utsname;
	uname(&_utsname);
	std::cout << "OS name: " << _utsname.sysname << std::endl;
	std::cout << "OS release: " << _utsname.release << std::endl;
	std::cout << "OS version: " << _utsname.version << std::endl;
#endif
}


void Chat::registration() {
	if (getUsersCount() >= maxUsers) {
		std::cout << "Вы не можете зарегистрироваться! Пользовательская база данных переполнена" << std::endl;
		return;
	}
	std::cout << "При вводе личной информации используйте буквы английского алфавита и цифры. " <<
		"Буквы русского алфавита и дополнительные символы запрещены!" << std::endl;

	std::cout << "Введите свое имя:" << std::endl;
	std::string name = chatGetline<std::string>();

	std::cout << "Придумайте логин:" << std::endl;
	std::string login = chatGetline();
	// Логин должен быть уникальным. Если логин уже существует, повторный запрос логина.
	if (findLogin(login)) {
		std::cout << "Вы не можете зарегистрироваться! Пользователь [" << login << "] уже зарегистрирован" << std::endl;
		return;
	}

	std::cout << "Введите свой пароль:" << std::endl;
	std::string password = chatGetline();

	// Добавляем нового пользователя в базу пользователей чата.
	add(name, login, password);
}


void Chat::enter() {
	// Администратор не может входить в чат и писать сообщения.
	if (getUsersCount() < 2) {
		std::cout << "В чате еще нет зарегистрированных пользователей!" << std::endl;
		return;
	}
	std::cout << "Введите свой логин:" << std::endl;
	std::string login = chatGetline();;
	// Проверка наличия данного логина в базе.
	if (!findLogin(login)) {
		std::cout << "Пользователь с данным логином не зарегистрирован!" << std::endl;
		return;
	}
	std::cout << "Введите свой пароль:" << std::endl;
	std::string password = chatGetline();;
	// Проверка соответствия пароля в базе и введенного пароля.
	if (checkPassword(login, password)) {
		std::cout << "Добро пожаловать, " << getName(login) << "!" << std::endl;
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
	// Сохраеняем сообщение в БД.
	setMessage(_currentUser, "all", message);
	// Выводим в консоль.
	message = "[" + _currentUser + "] написал всем: " + message;
	std::cout << message << std::endl;
	// Отправляем сообщение в сокет.
	sendNetMessage(message);
}


void Chat::writePrivate() {
	std::cout << "Выберите пользователя, которому хотите написать личное сообщение" << std::endl;
	std::vector <std::string> logins = getUsersList();
	for (size_t i = 0; i < logins.size(); ++i) {
		std::cout << "\t " << i << ". Написать сообщение пользователю " << logins[i] << std::endl;
	}
	size_t number = chatGetline<size_t>();;
	// Ищем введенный номер среди пользователей.
	std::string receiver = logins.at(number);
	std::cout << "Наберите текст сообщения для пользователя " << receiver << ". Для отправки нажмите клавишу Enter" << std::endl;
	std::string message = chatGetline();
	// Сохраняем сообщение в БД.
	setMessage(_currentUser, receiver, message);
	// Выводим сообщение в консоль.
	std::cout << "[" << receiver << "] у вас личное сообщение от пользователя " <<
		"[" << _currentUser << "]: " << message << std::endl;
	// Отправляем сообщение в сокет.
	message = "[" + _currentUser + "] отправил сообщение " + "[" + receiver + "]: " + message;
	sendNetMessage(message);
}

void Chat::checkMail() {
	getNetMessage();
}


bool Chat::terminate() {
	std::cout << "Введите пароль администратора:" << std::endl;
	std::string password = chatGetline();
	if (checkPassword("root", password)) {
		std::cout << "Работа чата завершена" << std::endl;
		return true;
	}
	else {
		std::cout << "Неправильный пароль!" << std::endl;
		return false;
	}
}



void Chat::add(const std::string& name, const std::string& login, const std::string& password, bool _readUserDB) {
	std::cout << "Добавляем пользователя в чат: " << "логин - " << login << ", имя - " << name <<
		", пароль - " << password << std::endl;
	// Создаем нового пользователя.
	User user(name, login, password);
	// Добавляем нового пользователя в базу.
	_dataBase[login] = user;

	if (!_readUserDB) {
		_userDataBase.emplace_back(user);
		_userDataBaseFile->writeState();
	}
}


bool Chat::checkPassword(const std::string& login, const std::string& password) {
	std::cout << "Выполняем вход пользователя в чат: " << "логин - " << login <<
		", пароль - " << password << std::endl;
	// Извлекаем сохраненный при регистрации пароль пользователя.
	std::cout << "Зарегистрированный пароль:" << std::endl;
	auto search = _dataBase.find(login);
	std::string _password = search->second.getPassword();
	// Сравниваем хеш с тем, что был вычислен от пароля при регистрации.
	if (_password == password) {
		std::cout << "Выполнен вход пользователя в чат" << std::endl;
		return true;
	}
	else {
		std::cout << "Ошибка! Неправильный пароль" << std::endl;
		return false;
	}
}

size_t Chat::getUsersCount() const {
	return _dataBase.size();
}

bool Chat::findLogin(const std::string& login) const {
	return (_dataBase.find(login) != _dataBase.end());
}

std::string Chat::getName(const std::string& login) const {
	auto search = _dataBase.find(login);
	if (search != _dataBase.end()) {
		return search->second.getName();
	}
	else {
		return "Error";
	}
}

std::vector<std::string> Chat::getUsersList() const {
	std::vector <std::string> array;
	for (auto& it : _dataBase) {
		// Скрываем администратора из списка пользователей.
		if (it.first != "root") array.push_back(it.first);
	}
	return array;
}

void Chat::netConfig() {
	// Создаем сокет для обмена сообщениями по сети с помощью протокола TCP.
	_socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
	if (_socketDescriptor == -1) {
		std::cout << "Ошибка при создании сокета!" << std::endl;
		// Чат продолжит свою работу локально.
		return;
	}
	// По умолчанию запускается клиентская версия приложения и 
	//  пытается установить соединение с приложением-сервером.
	// Заполняем струкруту с параметрами связи с сервером.
	_port = 49999;
	_IP = "127.0.0.1";
	sockaddr_in _socketAddress;
	_socketAddress.sin_addr.s_addr = inet_addr(_IP.c_str());;
	_socketAddress.sin_port = htons(_port);
	_socketAddress.sin_family = AF_INET;
	// Пытаемся установить связь с сервером.
	int status = connect(_socketDescriptor, (sockaddr *)& _socketAddress, sizeof(_socketAddress));
	if (status == -1) {
		std::cout << "Сервер не найден! Приложение будет настроено как сервер" << std::endl;
		_isServer = true;
		// Настраиваем приложение как сервер,
		//  принимающий запросы с любых IP.
		_socketAddress.sin_addr.s_addr = htonl(INADDR_ANY);
		// Привязываем сокет сервера к IP-адресу и номеру порта.
		status = bind(_socketDescriptor, (struct sockaddr*)&_socketAddress, sizeof(_socketAddress));
		if (status == -1) {
			std::cout << "Ошибка при привязке сокета!" << std::endl;
			// Чат продолжит свою работу локально.
			return;
		}
		// Переводим сокет в режим ожидания. Глубина очереди - 5 соединений.
		status = listen(_socketDescriptor, 5);
		if (status == -1) {
			std::cout << "Ошибка при переводе сокета в режим ожидания!" << std::endl;
			// Чат продолжит свою работу локально.
			return;
		}
		else {
			std::cout << "Сокет переведен в режим ожидания" << std::endl;
		}
		sockaddr_in client;
		socklen_t lenght = sizeof(client);
		std::cout << "Ждем первого клиента..." << std::endl;
		_connection = accept(_socketDescriptor, (sockaddr*)&client, &lenght);
		if (_connection == -1) {
			std::cout << "Ошибка! Сервер не может получать сообщения" << std::endl;
			// Чат продолжит свою работу локально.
			return;
		}
		std::cout << "Первый клиент подключен" << std::endl;
	}
	else {
		std::cout << "Установлено соединение с сервером" << std::endl;
	}
}
