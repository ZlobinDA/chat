#pragma once

#include "chat_exception.h" // chat_exception, iostream, string, std
#include <type_traits> // is_same_v
#include <iostream>

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
			std::string consoleInput;
			getline(std::cin, consoleInput);

			if constexpr (std::is_same_v<T, int>) {
				// ��������� �� ������� ���� � ������
				// ��� ��������� ������� ����� ���� �� �������, 
				//  �� ������������� ������������ ����������� ��� ������������� static_cast
				for (size_t i{ 0 }; i < consoleInput.size(); ++i) {
					if ((consoleInput.at(i) >= 'A' && consoleInput.at(i) <= 'Z') ||
						(consoleInput.at(i) >= 'a' && consoleInput.at(i) <= 'z')) {
						throw chat_exception("������ �������������� �����");
					}
				}
				_consoleInput = static_cast <int> (stoi(consoleInput));
			}
			else if constexpr (std::is_same_v<T, size_t>) {
				// ��������� �� ������� ���� � ������
				// ��� ��������� ������� ����� ���� �� �������, 
				//  �� ������������� ������������ ����������� ��� ������������� static_cast
				for (size_t i{ 0 }; i < consoleInput.size(); ++i) {
					if ((consoleInput.at(i) >= 'A' && consoleInput.at(i) <= 'Z') ||
						(consoleInput.at(i) >= 'a' && consoleInput.at(i) <= 'z')) {
						throw chat_exception("������ �������������� �����");
					}
				}
				_consoleInput = static_cast <size_t> (stoi(consoleInput));
			}
			else if constexpr (std::is_same_v<T, std::string>) {
				// ��������� �� ������� ������� ���� � ������.
				// ������ ������ ������ ������ ��������� � ��������� A-Z ��� a-z.
				for (size_t i{ 0 }; i < consoleInput.size(); ++i) {
					if ( !(consoleInput.at(i) >= 'A' && consoleInput.at(i) <= 'Z') && 
						!(consoleInput.at(i) >= 'a' && consoleInput.at(i) <= 'z')) {
						throw chat_exception("������ �������������� ���������� �����");
					}
				}
				_consoleInput = static_cast <std::string> (consoleInput);
			}
			break;
		}
		catch (chat_exception& e) {
			std::cout << "������� ������ ������������� �������: " << e.what() << std::endl;
			std::cout << "��������� ���� ������:" << std::endl;
			continue;
		}
		catch (...) {
			std::cout << "������� ������ ������������� �������!" << std::endl;
			std::cout << "��������� ���� ������:" << std::endl;
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
std::string chat_getline() {
	std::string consoleInput;
	// ��������� ����� ������������ ����������� � ���� ������������ �����: 
	//	���� ����� ������������� ���� ������������ �� ������ ���������� ����������.
	for (;;) {
		try {
			getline(std::cin, consoleInput);
			break;
		}
		catch (...) {
			std::cout << "������� ������ ������������� �������!" << std::endl;
			std::cout << "��������� ���� ������:" << std::endl;
			continue;
		}
	}

	return consoleInput;
}