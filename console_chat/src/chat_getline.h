#pragma once

#include "chat_exception.h" // chat_exception, iostream, string, std
#include <type_traits> // is_same_v

/**
	Шаблонная функция по получению данных от пользователя.
	Функция используется при вводе номера пункта меню и имени пользователя.
	Считаем, что имя пользователя не может содержать цифр.
	В зависимости от специализации возвращает либо строку, либо число.
	В функции реализована проверка исключений. При возникновении исключения пользователю предлагается
	 повторить ввод информации.
*/
template <typename T> T chat_getline() {
	T _consoleInput;
	// Обработка ввода пользователя реализована в виде бесконечного цикла: 
	//	ввод будет запрашиваться пока пользователь не введет корректную информацию.
	for (;;) {
		try {
			string consoleInput;
			getline(cin, consoleInput);

			if constexpr (is_same_v<T, int>) {
				// Проверяем на наличие букв в строке
				// Как проверять русские буквы пока не понятно, 
				//  их использование улавливается исключением при использовании static_cast
				for (auto i = 0; i < consoleInput.size(); ++i) {
					if ((consoleInput.at(i) >= 'A' && consoleInput.at(i) <= 'Z') ||
						(consoleInput.at(i) >= 'a' && consoleInput.at(i) <= 'z')) {
						throw chat_exception("должны использоваться цифры");
					}
				}
				_consoleInput = static_cast <int> (stoi(consoleInput));
			}
			else if constexpr (is_same_v<T, string>) {
				// Проверяем на наличие русских букв в строке.
				// Каждый символ строки должен находится в диапазоне A-Z или a-z.
				for (auto i = 0; i < consoleInput.size(); ++i) {
					if ( !(consoleInput.at(i) >= 'A' && consoleInput.at(i) <= 'Z') && 
						!(consoleInput.at(i) >= 'a' && consoleInput.at(i) <= 'z')) {
						throw chat_exception("должны использоваться английские буквы");
					}
				}
				_consoleInput = static_cast <string> (consoleInput);
			}
			break;
		}
		catch (chat_exception& e) {
			cout << "Введены данные неправильного формата: " << e.what() << endl;
			cout << "Повторите ввод данных:" << endl;
			continue;
		}
		catch (...) {
			cout << "Введены данные неправильного формата!" << endl;
			cout << "Повторите ввод данных:" << endl;
			continue;
		}
	}

	return _consoleInput;
}


/** 
	Обычная функция для реализации возможности ввода логина, пароля и сообщения пользователя,
	 состоящего из комбинации английских букв и цифр.
	В функции реализована проверка исключений. При возникновении исключения пользователю предлагается
	 повторить ввод информации.
*/
string chat_getline() {
	string consoleInput;
	// Обработка ввода пользователя реализована в виде бесконечного цикла: 
	//	ввод будет запрашиваться пока пользователь не введет корректную информацию.
	for (;;) {
		try {
			getline(cin, consoleInput);
			break;
		}
		catch (...) {
			cout << "Введены данные неправильного формата!" << endl;
			cout << "Повторите ввод данных:" << endl;
			continue;
		}
	}

	return consoleInput;
}