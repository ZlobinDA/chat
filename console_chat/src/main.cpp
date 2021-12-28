// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
 Задание
 Создайте чат со следующим функционалом:
 - консольная программа;
 - регистрация пользователей - логин, пароль, имя;
 - вход в чат по логину/паролю;
 - отправка сообщений конкретному пользователю;
 - обмен сообщениями между всеми пользователями чата одновременно.

 Обязательным условием является использование классов.
 Дополнительно можно реализовать обработку исключений и использование шаблонов.
*/

#include "chat.h"
#include "chat_exception.h" // chat_exception, iostream, string
#include "chat_getline.h"
#include <iostream>

auto main()->int {

#if defined (_WIN64) || (_WIN32)
	// ОС Windows (x32 и x64). Задаем в консоле кодировку cp1251 для корректного отображения русских букв.
	setlocale(LC_ALL, "");
#endif

	std::cout << std::endl;
	std::cout << "Добро пожаловать в локальный консольный чат!" << std::endl;
	Chat consoleChat;

	// Признак наличия пользователя в чате.
	static std::string currentUser;
	static bool userOnline = false;

	// Работа чата организована в виде бесконечного цикла.
	for (;;) {
		std::cout << std::endl;
		std::cout << "Выберите действие:" << std::endl;
		std::cout << "\t 1. Я новый пользователь. Хочу зарегистрироваться" << std::endl;
		std::cout << "\t 2. Я зарегистрированный пользователь. Войти в чат" << std::endl;
		std::cout << "\t 3. Завершить работу приложения" << std::endl;
		// Возможность писать сообщение в чат появляется при наличии 1-ого зарегистрированного пользователя в чате.
		if (userOnline) {
			std::cout << "\t 4. Написать сообщение в общий чат" << std::endl;
		}
		// Возможность написать личное сообщение доступна при наличии:
		//	- одного пользователя онлайн;
		//	- более 2-х зарегистрированных пользователей в чате (один пользователь - администратор).
		if (userOnline && consoleChat.getUsersCount() > 2) {
			std::cout << "\t 5. Написать личное сообщение пользователю" << std::endl;
		}
		// Выводим список пользователей, зарегистрированных в чате.
		// Данные о наличии администратора не выводим.
		if (consoleChat.getUsersCount() > 1) {
			std::string Users = "Пользователи, зарегистрированные в чате:";
			std::vector <std::string> logins = consoleChat.getUsersList();
			// Данные о наличии администратора не выводим.
			for (size_t i = 0; i < logins.size(); ++i) {
				Users = Users + " " + logins[i];
			}
			std::cout << Users << std::endl;
		}
		else {
			std::cout << "В чате еще никто не зарегистрировался" << std::endl;
		}

		int consoleInput = chat_getline<int>();
		switch (consoleInput) {
		case 1: // Регистрация нового пользователя.
		{
			if (consoleChat.getUsersCount() >= maxUsers) {
				std::cout << "Вы не можете зарегистрироваться! Пользовательская база данных переполнена" << std::endl;
				break;
			}

			std::cout << "При вводе личной информации используйте буквы английского алфавита и цифры. " <<
				"Буквы русского алфавита и дополнительные символы запрещены!" << std::endl;
			std::cout << "Введите свое имя:" << std::endl;
			std::string name = chat_getline<std::string>();

			std::cout << "Придумайте логин:" << std::endl;
			std::string login = chat_getline();
			// Логин должен быть уникальным. Если логин уже существует, повторный запрос логина.
			if (consoleChat.findLogin(login)) {
				std::cout << "Вы не можете зарегистрироваться! Пользователь [" << login << "] уже зарегистрирован" << std::endl;
				break;
			}

			std::cout << "Введите свой пароль:" << std::endl;
			std::string password = chat_getline();

			// Добавляем нового пользователя в базу пользователей чата.
			consoleChat.addUser(login, password, name);

			break;
		}
		case 2: // Вход в чат.
		{
			// Администратор не может входить в чат и писать сообщения.
			if (consoleChat.getUsersCount() < 1) {
				std::cout << "В чате еще нет зарегистрированных пользователей!" << std::endl;
				break;
			}

			std::cout << "Введите свой логин:" << std::endl;
			std::string login = chat_getline();;

			// Проверка наличия данного логина в базе.
			if (!consoleChat.findLogin(login)) {
				std::cout << "Пользователь с данным логином не зарегистрирован!" << std::endl;
				break;
			}

			std::cout << "Введите свой пароль:" << std::endl;
			std::string password = chat_getline();;

			// Проверка соответствия пароля в базе и введенного пароля.
			if (consoleChat.checkPassword(login, password)) {
				std::cout << "Добро пожаловать, " << consoleChat.getName(login) << "!" << std::endl;
				currentUser = std::move(login);
				userOnline = true;
			}
			else {
				std::cout << "Пароль введен не верно!" << std::endl;
			}
			break;
		}
		case 3: // Завершение работы приложения.
		{
			std::cout << "Введите пароль администратора:" << std::endl;
			std::string password = chat_getline();

			if (consoleChat.checkPassword("root", password)) {
				std::cout << "Работа чата завершена" << std::endl;
				return 0;
			}
			else {
				std::cout << "Неправильный пароль!" << std::endl;
				break;
			}
		}
		case 4: // Написать в общий чат.
		{
			std::cout << "Наберите текст сообщения. Для отправки нажмите клавишу Enter" << std::endl;
			std::string message = chat_getline();;
			std::cout << "[" << currentUser << "]: " << message << std::endl;
		}
		break;
		case 5: // Напичать личное сообщение.
		{
			std::cout << "Выберите пользователя, которому хотите написать личное сообщение" << std::endl;
			std::vector <std::string> logins = consoleChat.getUsersList();
			for (size_t i = 0; i < logins.size(); ++i) {
				// Пользователь не может отправлять личные сообщения себе
				if (logins[i] != currentUser) {
					std::cout << "\t " << i << ". Написать сообщение пользователю " << logins[i] << std::endl;
				}
			}
			size_t number = chat_getline<size_t>();;
			// Ищем введенный номер среди пользователей.

			std::cout << "Наберите текст сообщения для пользователя " << logins.at(number) << ". Для отправки нажмите клавишу Enter" << std::endl;
			std::string message = chat_getline();
			std::cout << "[" << logins.at(number) << "] у вас личное сообщение от пользователя " <<
				"[" << currentUser << "]: " << message << std::endl;
			break;
		}
		default:
			break;
		}

	}

	return 0;
}