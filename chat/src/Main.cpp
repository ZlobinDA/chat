// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "Chat.h"
#include <iostream>

auto main()->int {
#if defined (_WIN64) || (_WIN32)
	setlocale(LC_ALL, ".UTF8");
	std::cout << "ОС Windows. Задаем в консоле кодировку UTF8" << std::endl;
#endif
	
	// Задаем конфигурацию чата.
	std::string rootLogin {"root"};
	std::string rootPassword {"123"};
	std::string net_IP {"127.0.0.1"};
	uint16_t net_port {49999};
	std::string dataBase_host {"127.0.0.1"};
	std::string dataBase_user {"root"};
	std::string dataBase_password {"123456"};
	std::string dataBase_name {"chatdb"};
	
	Chat chat(rootLogin, rootPassword, net_IP, net_port, dataBase_host, dataBase_user, dataBase_password, dataBase_name);
	
	return chat.run();
}
