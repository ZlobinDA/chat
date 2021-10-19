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

#include "ChatUser.h"
#include "ChatUsersList.h"
#include "chat_exception.h" // chat_exception, iostream, string, std
#include "chat_getline.h"

auto main()->int {

	// Задаем в консоле кодировку cp1251 для корректного отображения русских букв.
	setlocale(LC_ALL, "");

	cout << endl;
	cout << "Добро пожаловать в локальный консольный чат!" << endl;

	// Максимальное число пользователей в базе данных. Если больше, отказ от регистрации.
	// Число может быть любым, для проверки достижения границы задано маленькое число.
	constexpr int maxUsers = 3;

	// Пароль администратора
	string rootPassword = "123";

	// userList - база данных пользователей.
	ChatUsersList* userList = new ChatUsersList(rootPassword);
	userList->loadFromFile();

	// Признак наличия пользователя в чате.
	static bool userOnline = false;

	// Работа чата организована в виде бесконечного цикла.
	static string currentUserLogin = ""; // логин текущего активного пользователя в чате
										// pam: предлагаю заменить на целочисленный currentUserID
	for (;;) {
		cout << endl;
		cout << "Выберите действие:" << endl;
		cout << "\t 1. Я новый пользователь. Хочу зарегистрироваться" << endl;
		cout << "\t 2. Я зарегистрированный пользователь. Войти в чат" << endl;
		cout << "\t 3. Завершить работу приложения" << endl;
		// Возможность писать сообщение в чат появляется при наличии 1-ого зарегистрированного пользователя в чате.
		if (userOnline) {
			cout << "\t 4. Написать сообщение в общий чат" << endl;
		}

		// Возможность написать личное сообщение доступна при наличии более 2-х зарегистрированных пользователей в чате.
		if (userOnline && userList->getUsersCount() >= 2) {
			cout << "\t 5. Написать личное сообщение пользователю" << endl;
		}
		// Выводим список пользователей, зарегистрированных в чате.
		if (userList->getUsersCount() >= 1) {

			string Users = "Пользователи, зарегистрированные в чате:";
			for (auto i = 1; i <= userList->getUsersCount(); ++i) {
				Users = Users + " " + userList->getNickname(i);
			}
			cout << Users << endl;
		}
		else {
			cout << "В чате еще никто не зарегистрировался" << endl;
		}

		int consoleInput = chat_getline<int>();
		switch (consoleInput) {
		case 1:
		{
			// Порядковый номер регистрируемого пользователя.
			// Использую отдельную переменную, что не путаться с userOnline
			// pam: заменил static int number на userList->getUsersCount()
			if (userList->getUsersCount() >= maxUsers) {
				cout << "Вы не можете зарегистрироваться! Пользовательская база данных переполнена" << endl;
				break;
			}

			cout << "При вводе личной информации используйте буквы английского алфавита и цифры. " <<
				"Буквы русского алфавита и дополнительные символы запрещены!" << endl;
			cout << "Введите свое имя:" << endl;
			string name = chat_getline<string>();

			cout << "Придумайте логин:" << endl;
			string login = chat_getline();
			// @todo Логин должен быть уникальным. Если логин уже существует, повторный запрос логина.

			cout << "Введите свой пароль:" << endl;
			string password = chat_getline();

			// Создаем нового пользователя. Данные вводятся в формате: логин - полное имя - пароль.
			ChatUser* newUser = new ChatUser(login, name, password);
			// 
			if (!newUser->registerUser(userList->getUsersCount() + 1))
			{
				// pam: сюда попадём, если verifyRegistration() не удалась. Пока она проверяет только login=root
				cout << "Вы не можете зарегистрироваться! Прочтите правила пользования чатом" << endl;
				break;
			}

			// Добавляем нового пользователя в базу данных. Подсчет числа пользователя производится через метод getUsersCount.
			userList->addUser(newUser);

			break;
		}
		case 2:
		{
			if (userList->getUsersCount() < 1) { // root'у пока входить для написания сообщений нельзя
				cout << "В чате еще нет зарегистрированных пользователей!" << endl;
				break;
			}

			cout << "Введите свой логин:" << endl;
			string login = chat_getline();;

			// Проверка наличия данного логина в базе.
			USRNGRPIDTYPE number = userList->findUserByNickname(login);

			// Проверили всю базу и не нашли пользователя в базе.
			if (number == USRWRONGID) {
				cout << "Пользователь с данным логином не зарегистрирован!" << endl;
				break;
			}

			cout << "Введите свой пароль:" << endl;
			string password = chat_getline();;

			// Проверка соответствия пароля в базе и введенного пароля.
			if (userList->checkPassword(number, password)) {
				cout << "Добро пожаловать, " << userList->getFullname(number) << "!" << endl;
				currentUserLogin = userList->getNickname(number);
				userOnline = true;
			}
			else {
				cout << "Пароль введен не верно!" << endl;
			}
			break;
		}
		case 3:
		{
			cout << "Введите пароль администратора:" << endl;
			string password = chat_getline();

			//if (password == rootPassword) {
			if (userList->checkPassword(0, password))
			{
				cout << "Работа чата завершена" << endl;
				userList->saveToFile();
				return 0;
			}
			else {
				cout << "Неправильный пароль!" << endl;
				break;
			}
		}
		case 4:
		{
			cout << "Наберите текст сообщения. Для отправки нажмите клавишу Enter" << endl;
			string message = chat_getline();;
			cout << "[" << currentUserLogin << "]: " << message << endl;
		}
		break;
		case 5:
		{
			cout << "Выберите пользователя, которому хотите написать личное сообщение" << endl;
			for (auto i = 1; i <= userList->getUsersCount(); ++i) {
				// Пользователь не может отправлять личные сообщения себе
				if ( (userList -> getNickname(i).compare(currentUserLogin)) != 0 ) {
					cout << "\t " << i << ". Написать сообщение пользователю " << userList->getNickname(i) << endl;
				}
			}
			int number = chat_getline<int>();;
			// Ищем введенный номер среди пользователей.
			for (auto i{ 1 }; i <= userList->getUsersCount(); ++i) {
				if ( ( (number) == i )&&(userList->getNickname( number ).compare(currentUserLogin) ) ) {
					cout << "Наберите текст сообщения для пользователя " << userList->getNickname(i) << ". Для отправки нажмите клавишу Enter" << endl;
					string message = chat_getline();
					cout << "[" << userList->getNickname(i) << "] у вас личное сообщение от пользователя " <<
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