/*

Создайте чат со следующим функционалом:
- консольная программа;
- регистрация пользователей - логин, пароль, имя;
- вход в чат по логину/паролю;
- отправка сообщений конкретному пользователю;
- обмен сообщениями между всеми пользователями чата одновременно.

Обязательным условием является использование классов.
Дополнительно можно реализовать обработку исключений и использование шаблонов.

*/

#include <iostream>
#include <string>
using namespace std;

auto main()->int {

	// Задаем в консоле кодировку cp1251 для корректного отображения русских букв.
	system("chcp 1251");

	cout << endl;
	cout << "Добро пожаловать в локальный консольный чат!" << endl;

	// Прототип. Чат из двух пользователей. Для каждого пользователя хранится имя, логин и пароль. Все данные на английском языке.
	// Прототип. Максимальное число пользователей в базе данных. Если больше, отказ от регистрации.
	constexpr int maxUsers = 2;
	// Расшифровка индексов массива userData.
	constexpr int userName = 0;			// имя пользователя
	constexpr int userLogin = 1;		// логин
	constexpr int userPassword = 2;		// пароль
	constexpr int userStatus = 3;		// статус: в чате или нет
	string userData[maxUsers] [4];
	// Текущее число пользователей в чате. 
	static int userCount = 0;

	// Работа чата организована в виде бесконечного цикла.
	// Завершить работу приложения может пользователь с правами администратора.
	// @todo Создать пользователя администратора.
	for (;;) {
		for (;;) {
			cout << endl;
			cout << "Выберите действие:" << endl;
			int operationNumber = 1;
			cout << "\t " << operationNumber << ". Я новый пользователь. Хочу зарегистрироваться" << endl;
			++operationNumber;
			cout << "\t " << operationNumber << ". Я зарегистрированный пользователь. Войти в чат" << endl;
			++operationNumber;
			cout << "\t " << operationNumber << ". Завершить работу приложения" << endl;
			// Возможность написать личное сообщение доступна при наличии более 2-х пользователей в чате.
			if (userCount > 1) {
				for (auto i = 0; i < maxUsers; ++i) {
					if (userData[i][userStatus] == "online") {
						++operationNumber;
						cout << "\t " << operationNumber << ". Написать сообщение пользователю " << userData[i][userName] << endl;

					}
				}
			}
			// Возможность писать сообщение в чат появляется при наличии 1-ого пользователя в чате.
			if (userCount > 0) {
				for (auto i = 0; i < maxUsers; ++i) {
					if (userData[i][userStatus] == "online") {
						++operationNumber;
						cout << "\t " << operationNumber << ". Написать сообщение в общий чат " << endl;
						break;
					}
				}
			}

			// Выводим список пользователей в чате.
			if (userCount > 0) {
				string onlineUsers = "Пользователи в чате:";
				for (auto i = 0; i < maxUsers; ++i) {
					if (userData[i][userStatus] == "online") {
						onlineUsers = onlineUsers + " " + userData[i][userName];
					}
				}
				cout << onlineUsers << endl;
			}
			else {
				cout << "В чате никого нет" << endl;
			}


			string consoleInput;
			// @todo Обработка исключений, связанных с вводом: вместо цифры с 1-4 вводится слово.
			getline(cin, consoleInput);
			switch (stoi(consoleInput)) {
			case 1:
			{
				// Порядковый номер регистрируемого пользователя.
				// Использую отдельную переменную, что не путаться с userCount
				static int number = -1;
				++number;
				if (number > maxUsers - 1) {
					--number;
					cout << "Вы не можете зарегистрироваться! Пользовательская база данных переполнена" << endl;
					break;
				}

				cout << "Введите свое имя:" << endl;
				// @todo Обработка исключений, связанных с вводом.
				string name;
				getline(cin, name);
				userData[number][userName] = name;

				cout << "Придумайте логин:" << endl;
				// @todo Обработка исключений, связанных с вводом.
				string login;
				getline(cin, login);
				userData[number][userLogin] = login;
				// @todo Логин должен быть уникальным. Если логин уже существует, повторный запрос логина.

				cout << "Введите свой пароль:" << endl;
				// @todo Обработка исключений, связанных с вводом.
				string password;
				getline(cin, password);
				userData[number][userPassword] = password;

				break;
			}
			case 2:
			{
				cout << "Введите свой логин:" << endl;
				// @todo Обработка исключений, связанных с вводом.
				string login;
				getline(cin, login);
				// Проверка наличия данного логина в базе.
				bool isUser = false;
				int number = -1; // номер пользователя в базе, используется при проверки пароля
				for (auto i = 0; i < maxUsers; ++i) {
					if (login == userData[i][userLogin]) {
						isUser = true;
						number = i;
						break;
					}
				}

				// Проверили всю базу и не нашли пользователя в базе.
				if (!isUser) {
					cout << "Пользователь с данным логином не зарегистрирован!" << endl;
					break;
				}

				cout << "Введите свой пароль:" << endl;
				// @todo Обработка исключений, связанных с вводом.
				string password;
				getline(cin, password);
				// Проверка соответствия пароля в базе и введенного пароля.
				if (password == userData[number][userPassword]) {
					userData[number][userStatus] = "online";
					cout << "Добро пожаловать, " << userData[number] [userName] << "!" << endl;
					++userCount;
				}
				else {
					cout << "Пароль введен не верно!" << endl;
				}
				break;
			}
			case 3:
			{
				cout << "Введите пароль администратора:" << endl;
				string password;
				getline(cin, password);
				// @todo Выполнить проверку пароля администратора
				bool shutdown = true;
				if (shutdown) {
					cout << "Работа чата завершена" << endl;
					return 0;
				}
				else {
					cout << "Неправильный пароль!" << endl;
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