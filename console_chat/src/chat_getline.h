#pragma once

#include "chat_exception.h" // chat_exception, iostream, string, std
#include <type_traits> // is_same_v

/**
	��������� ������� �� ��������� ������ �� ������������.
	������� ������������ ��� ����� ������ ������ ���� � ����� ������������.
	�������, ��� ��� ������������ �� ����� ��������� ����.
	� ����������� �� ������������� ���������� ���� ������, ���� �����.
	� ������� ����������� �������� ����������. ��� ������������� ���������� ������������ ������������
	 ��������� ���� ����������.
*/
template <typename T> T chat_getline() {
	T _consoleInput;
	// ��������� ����� ������������ ����������� � ���� ������������ �����: 
	//	���� ����� ������������� ���� ������������ �� ������ ���������� ����������.
	for (;;) {
		try {
			string consoleInput;
			getline(cin, consoleInput);

			if constexpr (is_same_v<T, int>) {
				// ��������� �� ������� ���� � ������
				// ��� ��������� ������� ����� ���� �� �������, 
				//  �� ������������� ������������ ����������� ��� ������������� static_cast
				for (auto i = 0; i < consoleInput.size(); ++i) {
					if ((consoleInput.at(i) >= 'A' && consoleInput.at(i) <= 'Z') ||
						(consoleInput.at(i) >= 'a' && consoleInput.at(i) <= 'z')) {
						throw chat_exception("������ �������������� �����");
					}
				}
				_consoleInput = static_cast <int> (stoi(consoleInput));
			}
			else if constexpr (is_same_v<T, string>) {
				// ��������� �� ������� ������� ���� � ������.
				// ������ ������ ������ ������ ��������� � ��������� A-Z ��� a-z.
				for (auto i = 0; i < consoleInput.size(); ++i) {
					if ( !(consoleInput.at(i) >= 'A' && consoleInput.at(i) <= 'Z') && 
						!(consoleInput.at(i) >= 'a' && consoleInput.at(i) <= 'z')) {
						throw chat_exception("������ �������������� ���������� �����");
					}
				}
				_consoleInput = static_cast <string> (consoleInput);
			}
			break;
		}
		catch (chat_exception& e) {
			cout << "������� ������ ������������� �������: " << e.what() << endl;
			cout << "��������� ���� ������:" << endl;
			continue;
		}
		catch (...) {
			cout << "������� ������ ������������� �������!" << endl;
			cout << "��������� ���� ������:" << endl;
			continue;
		}
	}

	return _consoleInput;
}


/** 
	������� ������� ��� ���������� ����������� ����� ������, ������ � ��������� ������������,
	 ���������� �� ���������� ���������� ���� � ����.
	� ������� ����������� �������� ����������. ��� ������������� ���������� ������������ ������������
	 ��������� ���� ����������.
*/
string chat_getline() {
	string consoleInput;
	// ��������� ����� ������������ ����������� � ���� ������������ �����: 
	//	���� ����� ������������� ���� ������������ �� ������ ���������� ����������.
	for (;;) {
		try {
			getline(cin, consoleInput);
			break;
		}
		catch (...) {
			cout << "������� ������ ������������� �������!" << endl;
			cout << "��������� ���� ������:" << endl;
			continue;
		}
	}

	return consoleInput;
}