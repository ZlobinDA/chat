// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
 �������
 �������� ��� �� ��������� ������������:
 - ���������� ���������;
 - ����������� ������������� - �����, ������, ���;
 - ���� � ��� �� ������/������;
 - �������� ��������� ����������� ������������;
 - ����� ����������� ����� ����� �������������� ���� ������������.

 ������������ �������� �������� ������������� �������.
 ������������� ����� ����������� ��������� ���������� � ������������� ��������.
*/

#include "chat.h"
#include "chat_exception.h" // chat_exception, iostream, string
#include "chat_getline.h"
#include <iostream>

auto main()->int {

#if defined (_WIN64) || (_WIN32)
	// �� Windows (x32 � x64). ������ � ������� ��������� cp1251 ��� ����������� ����������� ������� ����.
	setlocale(LC_ALL, "");
#endif

	std::cout << std::endl;
	std::cout << "����� ���������� � ��������� ���������� ���!" << std::endl;
	Chat consoleChat;

	// ������� ������� ������������ � ����.
	static std::string currentUser;
	static bool userOnline = false;

	// ������ ���� ������������ � ���� ������������ �����.
	for (;;) {
		std::cout << std::endl;
		std::cout << "�������� ��������:" << std::endl;
		std::cout << "\t 1. � ����� ������������. ���� ������������������" << std::endl;
		std::cout << "\t 2. � ������������������ ������������. ����� � ���" << std::endl;
		std::cout << "\t 3. ��������� ������ ����������" << std::endl;
		// ����������� ������ ��������� � ��� ���������� ��� ������� 1-��� ������������������� ������������ � ����.
		if (userOnline) {
			std::cout << "\t 4. �������� ��������� � ����� ���" << std::endl;
		}
		// ����������� �������� ������ ��������� �������� ��� �������:
		//	- ������ ������������ ������;
		//	- ����� 2-� ������������������ ������������� � ���� (���� ������������ - �������������).
		if (userOnline && consoleChat.getUsersCount() > 2) {
			std::cout << "\t 5. �������� ������ ��������� ������������" << std::endl;
		}
		// ������� ������ �������������, ������������������ � ����.
		// ������ � ������� �������������� �� �������.
		if (consoleChat.getUsersCount() > 1) {
			std::string Users = "������������, ������������������ � ����:";
			std::vector <std::string> logins = consoleChat.getUsersList();
			// ������ � ������� �������������� �� �������.
			for (size_t i = 0; i < logins.size(); ++i) {
				Users = Users + " " + logins[i];
			}
			std::cout << Users << std::endl;
		}
		else {
			std::cout << "� ���� ��� ����� �� �����������������" << std::endl;
		}

		int consoleInput = chat_getline<int>();
		switch (consoleInput) {
		case 1: // ����������� ������ ������������.
		{
			if (consoleChat.getUsersCount() >= maxUsers) {
				std::cout << "�� �� ������ ������������������! ���������������� ���� ������ �����������" << std::endl;
				break;
			}

			std::cout << "��� ����� ������ ���������� ����������� ����� ����������� �������� � �����. " <<
				"����� �������� �������� � �������������� ������� ���������!" << std::endl;
			std::cout << "������� ���� ���:" << std::endl;
			std::string name = chat_getline<std::string>();

			std::cout << "���������� �����:" << std::endl;
			std::string login = chat_getline();
			// ����� ������ ���� ����������. ���� ����� ��� ����������, ��������� ������ ������.
			if (consoleChat.findLogin(login)) {
				std::cout << "�� �� ������ ������������������! ������������ [" << login << "] ��� ���������������" << std::endl;
				break;
			}

			std::cout << "������� ���� ������:" << std::endl;
			std::string password = chat_getline();

			// ��������� ������ ������������ � ���� ������������� ����.
			consoleChat.addUser(login, password, name);

			break;
		}
		case 2: // ���� � ���.
		{
			// ������������� �� ����� ������� � ��� � ������ ���������.
			if (consoleChat.getUsersCount() < 1) {
				std::cout << "� ���� ��� ��� ������������������ �������������!" << std::endl;
				break;
			}

			std::cout << "������� ���� �����:" << std::endl;
			std::string login = chat_getline();;

			// �������� ������� ������� ������ � ����.
			if (!consoleChat.findLogin(login)) {
				std::cout << "������������ � ������ ������� �� ���������������!" << std::endl;
				break;
			}

			std::cout << "������� ���� ������:" << std::endl;
			std::string password = chat_getline();;

			// �������� ������������ ������ � ���� � ���������� ������.
			if (consoleChat.checkPassword(login, password)) {
				std::cout << "����� ����������, " << consoleChat.getName(login) << "!" << std::endl;
				currentUser = std::move(login);
				userOnline = true;
			}
			else {
				std::cout << "������ ������ �� �����!" << std::endl;
			}
			break;
		}
		case 3: // ���������� ������ ����������.
		{
			std::cout << "������� ������ ��������������:" << std::endl;
			std::string password = chat_getline();

			if (consoleChat.checkPassword("root", password)) {
				std::cout << "������ ���� ���������" << std::endl;
				return 0;
			}
			else {
				std::cout << "������������ ������!" << std::endl;
				break;
			}
		}
		case 4: // �������� � ����� ���.
		{
			std::cout << "�������� ����� ���������. ��� �������� ������� ������� Enter" << std::endl;
			std::string message = chat_getline();;
			std::cout << "[" << currentUser << "]: " << message << std::endl;
		}
		break;
		case 5: // �������� ������ ���������.
		{
			std::cout << "�������� ������������, �������� ������ �������� ������ ���������" << std::endl;
			std::vector <std::string> logins = consoleChat.getUsersList();
			for (size_t i = 0; i < logins.size(); ++i) {
				// ������������ �� ����� ���������� ������ ��������� ����
				if (logins[i] != currentUser) {
					std::cout << "\t " << i << ". �������� ��������� ������������ " << logins[i] << std::endl;
				}
			}
			size_t number = chat_getline<size_t>();;
			// ���� ��������� ����� ����� �������������.

			std::cout << "�������� ����� ��������� ��� ������������ " << logins.at(number) << ". ��� �������� ������� ������� Enter" << std::endl;
			std::string message = chat_getline();
			std::cout << "[" << logins.at(number) << "] � ��� ������ ��������� �� ������������ " <<
				"[" << currentUser << "]: " << message << std::endl;
			break;
		}
		default:
			break;
		}

	}

	return 0;
}