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

#include "ChatUser.h"
#include "ChatUsersList.h"
#include "chat_exception.h" // chat_exception, iostream, string, std
#include "chat_getline.h"

auto main()->int {

	// ������ � ������� ��������� cp1251 ��� ����������� ����������� ������� ����.
	setlocale(LC_ALL, "");

	cout << endl;
	cout << "����� ���������� � ��������� ���������� ���!" << endl;

	// ������������ ����� ������������� � ���� ������. ���� ������, ����� �� �����������.
	// ����� ����� ���� �����, ��� �������� ���������� ������� ������ ��������� �����.
	constexpr int maxUsers = 3;

	// ������ ��������������
	string rootPassword = "123";

	// userList - ���� ������ �������������.
	ChatUsersList* userList = new ChatUsersList(rootPassword);
	userList->loadFromFile();

	// ������� ������� ������������ � ����.
	static bool userOnline = false;

	// ������ ���� ������������ � ���� ������������ �����.
	static string currentUserLogin = ""; // ����� �������� ��������� ������������ � ����
										// pam: ��������� �������� �� ������������� currentUserID
	for (;;) {
		cout << endl;
		cout << "�������� ��������:" << endl;
		cout << "\t 1. � ����� ������������. ���� ������������������" << endl;
		cout << "\t 2. � ������������������ ������������. ����� � ���" << endl;
		cout << "\t 3. ��������� ������ ����������" << endl;
		// ����������� ������ ��������� � ��� ���������� ��� ������� 1-��� ������������������� ������������ � ����.
		if (userOnline) {
			cout << "\t 4. �������� ��������� � ����� ���" << endl;
		}

		// ����������� �������� ������ ��������� �������� ��� ������� ����� 2-� ������������������ ������������� � ����.
		if (userOnline && userList->getUsersCount() >= 2) {
			cout << "\t 5. �������� ������ ��������� ������������" << endl;
		}
		// ������� ������ �������������, ������������������ � ����.
		if (userList->getUsersCount() >= 1) {

			string Users = "������������, ������������������ � ����:";
			for (auto i = 1; i <= userList->getUsersCount(); ++i) {
				Users = Users + " " + userList->getNickname(i);
			}
			cout << Users << endl;
		}
		else {
			cout << "� ���� ��� ����� �� �����������������" << endl;
		}

		int consoleInput = chat_getline<int>();
		switch (consoleInput) {
		case 1:
		{
			// ���������� ����� ��������������� ������������.
			// ��������� ��������� ����������, ��� �� �������� � userOnline
			// pam: ������� static int number �� userList->getUsersCount()
			if (userList->getUsersCount() >= maxUsers) {
				cout << "�� �� ������ ������������������! ���������������� ���� ������ �����������" << endl;
				break;
			}

			cout << "��� ����� ������ ���������� ����������� ����� ����������� �������� � �����. " <<
				"����� �������� �������� � �������������� ������� ���������!" << endl;
			cout << "������� ���� ���:" << endl;
			string name = chat_getline<string>();

			cout << "���������� �����:" << endl;
			string login = chat_getline();
			// @todo ����� ������ ���� ����������. ���� ����� ��� ����������, ��������� ������ ������.

			cout << "������� ���� ������:" << endl;
			string password = chat_getline();

			// ������� ������ ������������. ������ �������� � �������: ����� - ������ ��� - ������.
			ChatUser* newUser = new ChatUser(login, name, password);
			// 
			if (!newUser->registerUser(userList->getUsersCount() + 1))
			{
				// pam: ���� ������, ���� verifyRegistration() �� �������. ���� ��� ��������� ������ login=root
				cout << "�� �� ������ ������������������! �������� ������� ����������� �����" << endl;
				break;
			}

			// ��������� ������ ������������ � ���� ������. ������� ����� ������������ ������������ ����� ����� getUsersCount.
			userList->addUser(newUser);

			break;
		}
		case 2:
		{
			if (userList->getUsersCount() < 1) { // root'� ���� ������� ��� ��������� ��������� ������
				cout << "� ���� ��� ��� ������������������ �������������!" << endl;
				break;
			}

			cout << "������� ���� �����:" << endl;
			string login = chat_getline();;

			// �������� ������� ������� ������ � ����.
			USRNGRPIDTYPE number = userList->findUserByNickname(login);

			// ��������� ��� ���� � �� ����� ������������ � ����.
			if (number == USRWRONGID) {
				cout << "������������ � ������ ������� �� ���������������!" << endl;
				break;
			}

			cout << "������� ���� ������:" << endl;
			string password = chat_getline();;

			// �������� ������������ ������ � ���� � ���������� ������.
			if (userList->checkPassword(number, password)) {
				cout << "����� ����������, " << userList->getFullname(number) << "!" << endl;
				currentUserLogin = userList->getNickname(number);
				userOnline = true;
			}
			else {
				cout << "������ ������ �� �����!" << endl;
			}
			break;
		}
		case 3:
		{
			cout << "������� ������ ��������������:" << endl;
			string password = chat_getline();

			//if (password == rootPassword) {
			if (userList->checkPassword(0, password))
			{
				cout << "������ ���� ���������" << endl;
				userList->saveToFile();
				return 0;
			}
			else {
				cout << "������������ ������!" << endl;
				break;
			}
		}
		case 4:
		{
			cout << "�������� ����� ���������. ��� �������� ������� ������� Enter" << endl;
			string message = chat_getline();;
			cout << "[" << currentUserLogin << "]: " << message << endl;
		}
		break;
		case 5:
		{
			cout << "�������� ������������, �������� ������ �������� ������ ���������" << endl;
			for (auto i = 1; i <= userList->getUsersCount(); ++i) {
				// ������������ �� ����� ���������� ������ ��������� ����
				if ( (userList -> getNickname(i).compare(currentUserLogin)) != 0 ) {
					cout << "\t " << i << ". �������� ��������� ������������ " << userList->getNickname(i) << endl;
				}
			}
			int number = chat_getline<int>();;
			// ���� ��������� ����� ����� �������������.
			for (auto i{ 1 }; i <= userList->getUsersCount(); ++i) {
				if ( ( (number) == i )&&(userList->getNickname( number ).compare(currentUserLogin) ) ) {
					cout << "�������� ����� ��������� ��� ������������ " << userList->getNickname(i) << ". ��� �������� ������� ������� Enter" << endl;
					string message = chat_getline();
					cout << "[" << userList->getNickname(i) << "] � ��� ������ ��������� �� ������������ " <<
						"[" << currentUserLogin << "]: " << message << endl;
				}
			}
			break;
		}
		default:
			break;
		}

	}

	return 0;
}