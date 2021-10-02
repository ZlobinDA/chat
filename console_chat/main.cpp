/*

�������� ��� �� ��������� ������������:
- ���������� ���������;
- ����������� ������������� - �����, ������, ���;
- ���� � ��� �� ������/������;
- �������� ��������� ����������� ������������;
- ����� ����������� ����� ����� �������������� ���� ������������.

������������ �������� �������� ������������� �������.
������������� ����� ����������� ��������� ���������� � ������������� ��������.

*/

#include <iostream>
#include <string>
using namespace std;

auto main()->int {

	// ������ � ������� ��������� cp1251 ��� ����������� ����������� ������� ����.
	system("chcp 1251");

	cout << endl;
	cout << "����� ���������� � ��������� ���������� ���!" << endl;

	// ��������. ��� �� ���� �������������. ��� ������� ������������ �������� ���, ����� � ������. ��� ������ �� ���������� �����.
	// ��������. ������������ ����� ������������� � ���� ������. ���� ������, ����� �� �����������.
	constexpr int maxUsers = 2;
	// ����������� �������� ������� userData.
	constexpr int userName = 0;			// ��� ������������
	constexpr int userLogin = 1;		// �����
	constexpr int userPassword = 2;		// ������
	constexpr int userStatus = 3;		// ������: � ���� ��� ���
	string userData[maxUsers] [4];
	// ������� ����� ������������� � ����. 
	static int userCount = 0;

	// ������ ���� ������������ � ���� ������������ �����.
	// ��������� ������ ���������� ����� ������������ � ������� ��������������.
	// @todo ������� ������������ ��������������.
	for (;;) {
		for (;;) {
			cout << endl;
			cout << "�������� ��������:" << endl;
			int operationNumber = 1;
			cout << "\t " << operationNumber << ". � ����� ������������. ���� ������������������" << endl;
			++operationNumber;
			cout << "\t " << operationNumber << ". � ������������������ ������������. ����� � ���" << endl;
			++operationNumber;
			cout << "\t " << operationNumber << ". ��������� ������ ����������" << endl;
			// ����������� �������� ������ ��������� �������� ��� ������� ����� 2-� ������������� � ����.
			if (userCount > 1) {
				for (auto i = 0; i < maxUsers; ++i) {
					if (userData[i][userStatus] == "online") {
						++operationNumber;
						cout << "\t " << operationNumber << ". �������� ��������� ������������ " << userData[i][userName] << endl;

					}
				}
			}
			// ����������� ������ ��������� � ��� ���������� ��� ������� 1-��� ������������ � ����.
			if (userCount > 0) {
				for (auto i = 0; i < maxUsers; ++i) {
					if (userData[i][userStatus] == "online") {
						++operationNumber;
						cout << "\t " << operationNumber << ". �������� ��������� � ����� ��� " << endl;
						break;
					}
				}
			}

			// ������� ������ ������������� � ����.
			if (userCount > 0) {
				string onlineUsers = "������������ � ����:";
				for (auto i = 0; i < maxUsers; ++i) {
					if (userData[i][userStatus] == "online") {
						onlineUsers = onlineUsers + " " + userData[i][userName];
					}
				}
				cout << onlineUsers << endl;
			}
			else {
				cout << "� ���� ������ ���" << endl;
			}


			string consoleInput;
			// @todo ��������� ����������, ��������� � ������: ������ ����� � 1-4 �������� �����.
			getline(cin, consoleInput);
			switch (stoi(consoleInput)) {
			case 1:
			{
				// ���������� ����� ��������������� ������������.
				// ��������� ��������� ����������, ��� �� �������� � userCount
				static int number = -1;
				++number;
				if (number > maxUsers - 1) {
					--number;
					cout << "�� �� ������ ������������������! ���������������� ���� ������ �����������" << endl;
					break;
				}

				cout << "������� ���� ���:" << endl;
				// @todo ��������� ����������, ��������� � ������.
				string name;
				getline(cin, name);
				userData[number][userName] = name;

				cout << "���������� �����:" << endl;
				// @todo ��������� ����������, ��������� � ������.
				string login;
				getline(cin, login);
				userData[number][userLogin] = login;
				// @todo ����� ������ ���� ����������. ���� ����� ��� ����������, ��������� ������ ������.

				cout << "������� ���� ������:" << endl;
				// @todo ��������� ����������, ��������� � ������.
				string password;
				getline(cin, password);
				userData[number][userPassword] = password;

				break;
			}
			case 2:
			{
				cout << "������� ���� �����:" << endl;
				// @todo ��������� ����������, ��������� � ������.
				string login;
				getline(cin, login);
				// �������� ������� ������� ������ � ����.
				bool isUser = false;
				int number = -1; // ����� ������������ � ����, ������������ ��� �������� ������
				for (auto i = 0; i < maxUsers; ++i) {
					if (login == userData[i][userLogin]) {
						isUser = true;
						number = i;
						break;
					}
				}

				// ��������� ��� ���� � �� ����� ������������ � ����.
				if (!isUser) {
					cout << "������������ � ������ ������� �� ���������������!" << endl;
					break;
				}

				cout << "������� ���� ������:" << endl;
				// @todo ��������� ����������, ��������� � ������.
				string password;
				getline(cin, password);
				// �������� ������������ ������ � ���� � ���������� ������.
				if (password == userData[number][userPassword]) {
					userData[number][userStatus] = "online";
					cout << "����� ����������, " << userData[number] [userName] << "!" << endl;
					++userCount;
				}
				else {
					cout << "������ ������ �� �����!" << endl;
				}
				break;
			}
			case 3:
			{
				cout << "������� ������ ��������������:" << endl;
				string password;
				getline(cin, password);
				// @todo ��������� �������� ������ ��������������
				bool shutdown = true;
				if (shutdown) {
					cout << "������ ���� ���������" << endl;
					return 0;
				}
				else {
					cout << "������������ ������!" << endl;
				}
			}
				break;
			default:
				break;
			}

		}
	}

	return 0;
}