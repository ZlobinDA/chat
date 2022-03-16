#pragma once

#include "Logger.h"

#include <string>
#ifdef __linux__
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif // __linux__

// Максимально допустимая длина сетевого сообщения.
constexpr int netMessageLen = 1024;

class Net final {
private:
	/** Параметры соединения */
	int _socketDescriptor;	/** дескриптор сокета */
	uint16_t _port;		/** номер порта */
	std::string _IP;	/** IP адресс */
	int _connection;	/** дескриптор присоединенного сокета */
	bool _isServer;		/** признак того, что приложение является сервером */

	/** Журнал */
	Logger _log;

public:

	Net();
	~Net();

	// Настройка сети.
	void config(const std::string& IP, const uint16_t port);
	// Получить сообщение из сокета.
	std::string getMessage();
	// Отправить сообщение в сокет.
	void sendMessage(const std::string& message);
};