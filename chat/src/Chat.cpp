// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "Chat.h"
#include "ChatGetline.h"
#include <iomanip>
#include <iostream>
#include <cstring>
#if defined (__linux__)
#include <sys/utsname.h>
#endif


Chat::Chat() : _log("Chat"), _userOnline(false),
	_socketDescriptor(0), _port(0), _connection(0), _isServer(false) {
	std::string logMessage = "Вызван конструктор";
	_log.write(logMessage);
	// Печатаем в консоль информации о операционной системы, в которой запущен чат.
	showConfig();
	// Настраиваем сеть.
	netConfig();
	// Подключаемся к базе данных.
	logMessage = "Подключаемся к базе данных";
	_log.write(logMessage);
	std::string host = "localhost";
	std::string user = "root";
	std::string password = "123456";
	std::string name = "chatdb";
	_dataBase.connect(host, user, password, name);
	// Для пустой базы данных добавляем администратора.
	if (!_dataBase.find_user("root")) {
		logMessage = "Добавляем пользователя с правами администратора";
		_log.write(logMessage);
		_dataBase.add_user("root", "root", "123");
	}
}

Chat::~Chat() {
	std::string logMessage = "Вызван деструктор";
	_log.write(logMessage);
#ifdef __linux__
	logMessage = "Закрываем сокет, завершаем соединение";
	_log.write(logMessage);
	close(_socketDescriptor);
#endif
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


void Chat::getNetMessage() {
#ifdef __linux__
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
#endif
}

void Chat::sendNetMessage(const std::string& message) {
#ifdef __linux__
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
		logMessage = "Сообщение успешно отправлено";
		_log.write(logMessage);
	}
#endif

}

void Chat::showConfig() {
	std::string logMessage;
	// ОС Windows
#if defined (_WIN32) || defined (_WIN64)
	logMessage = "OS name: Windows";
	_log.write(logMessage);
#endif
	// ОС Linux
#if defined(__linux__)
	struct utsname _utsname;
	uname(&_utsname);
	logMessage = "OS name: " + _utsname.sysname;
	_log.write(logMessage);
	logMessage = "OS release: " + _utsname.release;
	_log.write(logMessage);
	logMessage = "OS version: " + _utsname.version;
	_log.write(logMessage);
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
	// Отправляем сообщение в сокет.
	sendNetMessage(message);
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

void Chat::netConfig() {
#ifdef __linux__
	// Создаем сокет для обмена сообщениями по сети с помощью протокола TCP.
	_socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
	if (_socketDescriptor == -1) {
		logMessage = "Ошибка при создании сокета!";
		_log.write(logMessage);
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
	int status = connect(_socketDescriptor, (sockaddr*)&_socketAddress, sizeof(_socketAddress));
	if (status == -1) {
		logMessage = "Сервер не найден! Приложение будет настроено как сервер";
		_log.write(logMessage);
		_isServer = true;
		// Настраиваем приложение как сервер,
		//  принимающий запросы с любых IP.
		_socketAddress.sin_addr.s_addr = htonl(INADDR_ANY);
		// Привязываем сокет сервера к IP-адресу и номеру порта.
		status = bind(_socketDescriptor, (struct sockaddr*)&_socketAddress, sizeof(_socketAddress));
		if (status == -1) {
			logMessage = "Ошибка при привязке сокета!";
			_log.write(logMessage);
			// Чат продолжит свою работу локально.
			return;
		}
		// Переводим сокет в режим ожидания. Глубина очереди - 5 соединений.
		status = listen(_socketDescriptor, 5);
		if (status == -1) {
			logMessage = "Ошибка при переводе сокета в режим ожидания!";
			_log.write(logMessage);
			// Чат продолжит свою работу локально.
			return;
		}
		else {
			logMessage = "Сокет переведен в режим ожидания";
			_log.write(logMessage);
		}
		sockaddr_in client;
		socklen_t lenght = sizeof(client);
		logMessage = "Ждем первого клиента...";
		_log.write(logMessage);
		_connection = accept(_socketDescriptor, (sockaddr*)&client, &lenght);
		if (_connection == -1) {
			logMessage = "Ошибка! Сервер не может получать сообщения";
			_log.write(logMessage);
			// Чат продолжит свою работу локально.
			return;
		}
		logMessage = "Первый клиент подключен";
		_log.write(logMessage);
	}
	else {
		logMessage = "Установлено соединение с сервером";
		_log.write(logMessage);
	}
#endif
}
