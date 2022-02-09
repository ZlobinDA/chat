#pragma once

#include "DataFile.h"
#include "Message.h"
#include "User.h"

#include <string>
#include <vector>
#include <unordered_map>
#ifdef __linux__
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif // __linux__

// Константы цикла по управлению работой чата
constexpr int registrationState = 1;
constexpr int enterState = 2;
constexpr int terminateState = 3;
constexpr int writePublicState = 4;
constexpr int writePrivateState = 5;
constexpr int checkMailState = 6;

// Максимальное число пользователей, которые могут зарегистрироваться в чате.
// Используем маленькое значение, чтобы протестировать работу чата при переполнении базы пользователей.
// Проверяем работу чата для 3 пользователей, т.к. 1 пользователь - администратор.
constexpr int maxUsers = 4;

// Максимально допустимая длина сетевого сообщения.
constexpr int netMessageLen = 256;

class Chat final {
private:
	std::string _currentUser;	// Имя текущего пользователя, находящегося в чате.
	bool _userOnline;			// Признак наличия пользователя в чате.

	/** Работа с сетью. */
	int _socketDescriptor;	// дескриптор сокета
	uint16_t _port;			// номер порта
	std::string _IP;		// IP адресс
	int _connection;		// дескриптор присоединенного сокета
	bool _isServer;			// признак того, что приложение является сервером
	// Настройка приложения.
	void netConfig();
	// Получить сообщение из сокета.
	void getNetMessage();
	// Отправить сообщение в сокет.
	void sendNetMessage(const std::string& message);
	
	/** База данных сообщений чата. */
	std::vector <Message> _messageDataBase;
	DataFile <std::vector <Message>>* _messageDataBaseFile;
	void setMessage(const std::string& from, const std::string& to, const std::string& message);

	/** База данных пользователей чата. */
	// Используем ассоциативный контейнер unordered_map для хранения базы данных пользователей:
	//	- логин пользователя уникальный, поэтому храним данные в виде пары
	//	 ключ (логин) - значение (структура с данными о пользователе);
	//	- данные о пользователях можно хранить неупорядоченно;
	//	- быстрый поиск, вставка и удаление пользователя.
	std::unordered_map <std::string, User> _dataBase;
	std::vector <User> _userDataBase;
	DataFile <std::vector <User>>* _userDataBaseFile;

	/** Механика чата. */
	/** Вывод в консоль названия и номера версии операционной системы, в которой запущен чат. */
	void showConfig();
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

	/** Добавление нового пользователя в базу пользователей чата. */
	void add(const std::string& name, const std::string& login, const std::string& password, bool _readUserDB = false);

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

public:
	Chat();
	~Chat();

	/** Метод для запуска чата. Работа чата организована в виде бесконечного цикла. */
	int run();
};