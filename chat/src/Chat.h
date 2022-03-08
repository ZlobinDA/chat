#pragma once

#include "DataBase.h"
#include "Logger.h"

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

// Максимально допустимая длина сетевого сообщения.
constexpr int netMessageLen = 256;

class Chat final {
private:
	DataBase _dataBase;			/** база данных пользователей чата */
	Logger _log;				/** журнал */
	std::string _currentUser;	/** имя текущего пользователя, находящегося в чате */
	bool _userOnline;			/** признак наличия пользователя в чате */

	/** Работа с сетью. */
	int _socketDescriptor;	// дескриптор сокета
	uint16_t _port;			// номер порта
	std::string _IP;		// IP адресс
	int _connection;		// дескриптор присоединенного сокета
	bool _isServer;			// признак того, что приложение является сервером
	// Настройка сети.
	void netConfig();
	// Получить сообщение из сокета.
	void getNetMessage();
	// Отправить сообщение в сокет.
	void sendNetMessage(const std::string& message);

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

public:
	Chat();
	~Chat();

	/** Метод для запуска чата. Работа чата организована в виде бесконечного цикла. */
	int run();
};