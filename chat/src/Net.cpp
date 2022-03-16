// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "Net.h"
#include <cstring>

Net::Net() : _socketDescriptor(0), _port(0), _connection(0), _isServer(false),
	_log("Net") {
}

Net::~Net() {
	std::string logMessage = "Закрываем сокет, завершаем соединение";
	_log.write(logMessage);
#ifdef __linux__
	close(_socketDescriptor);
#endif
}

void Net::config(const std::string& IP, const uint16_t port) {
	_IP = IP;
	_port = port;
#ifdef __linux__
	// Создаем сокет для обмена сообщениями по сети с помощью протокола TCP.
	_socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
	if (_socketDescriptor == -1) {
		std::string logMessage = "Ошибка при создании сокета!";
		_log.write(logMessage);
		// Чат продолжит свою работу локально.
		return;
	}
	// По умолчанию запускается клиентская версия приложения и 
	//  пытается установить соединение с приложением-сервером.
	// Заполняем струкруту с параметрами связи с сервером.
	sockaddr_in _socketAddress;
	_socketAddress.sin_addr.s_addr = inet_addr(_IP.c_str());;
	_socketAddress.sin_port = htons(_port);
	_socketAddress.sin_family = AF_INET;
	// Пытаемся установить связь с сервером.
	int status = connect(_socketDescriptor, (sockaddr*)&_socketAddress, sizeof(_socketAddress));
	if (status == -1) {
		std::string logMessage = "Сервер не найден! Приложение будет настроено как сервер";
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
		std::string logMessage = "Установлено соединение с сервером";
		_log.write(logMessage);
	}
#endif
}

std::string Net::getMessage() {
	std::string logMessage = "Читаем сообщение из сокета";
	_log.write(logMessage);
	char c_message[netMessageLen];
#ifdef __linux__
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
#endif
	std::string message = c_message;
	logMessage = "Текст полученного сообщения: " + message;
	_log.write(logMessage);
	return message;
}

void Net::sendMessage(const std::string& message) {
	std::string logMessage = "Отправляем сообщение в сокет: " + message;
	_log.write(logMessage);
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
	else {
		logMessage = "Ошибка при отправке сообщения";
		_log.write(logMessage);
}
#endif
}
