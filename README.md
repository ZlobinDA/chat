
	========
	Описание
	========

	После запуска приложения выводится стартовое меню:
		1. Я новый пользователь. Хочу зарегистрироваться
		2. Я зарегистрированный пользователь. Войти в чат
		3. Завершить работу приложения
	Для того, чтобы у пользователя появилась возможность написать сообщение в чат, он должен зарегистрироваться (п. 1 меню)
	 и далее войти в чат (п. 2 меню). Имена пользователей могут совпадать, логины должны быть уникальными.
	Для смены пользователя необходимо использовать п.2 и войти в чат другим пользователем. При этом выходить из чата (разлогиниваться)
	 предыдущем пользователем не нужно.
	Писать сообщение можно либо в общий чат, либо конкретному пользователя.
	Общий чат, личные сообщения и проверка почты (см ниже, п. 4, 5, 6 меню) становятся доступен при входе пользователя в чат:
		1. Я новый пользователь. Хочу зарегистрироваться
		2. Я зарегистрированный пользователь. Войти в чат
		3. Завершить работу приложения
		4. Написать сообщение в общий чат
		5. Написать личное сообщение пользователю
	    	6. Проверить почту
	Чат поддерживает возможность писать сообщения самому себе.
	Каждое сообщение содержит имя пользователя, который его написал.
	Выбор имени пользователя для отправки личного сообщения активируется после выбора пункта меню 5.
	
	При попытке завершить работу приложения запрашивается пароль администратора. Пароль администратора (123) сохранен в коде программы.
	
	==========
	Обновления
	==========
	
	[27.01.2022]
	Добавлена запись данных о зарегистрированных пользователях и сообщениях в чате в файл.
	Данные восстанавливаются при перезапуске приложения.
	
	[09.02.2022]
	Добавлена поддержка работы с сетью. Работа с сетью реализована на примере архитектуры клиент-сервер.
	При запуске экземпляра приложения производится попытка соединится с сервером, если сервер недоступен,
	экземпляр приложения конфигурируется как сервер. Если сервер найден, то экземлятр приложения конфигурируется как клиент.
	Для проверки и вывод в консоль сообщения, отправленного по сети, необходимо использовать п. 6 "Проверить почту".
	
	[16.03.2022]
	Добавлена поддержка базы данных. Информация о пользователях и история сообщений хранится в базе данных.
	
	[30.03.2022]
	Добавлен класс Logger для записи и чтения лог-сообщений в многопоточном режиме.
	По умолчанию используются следующие логи: main.log - журнал с информацией о работе чата,
	message.log - журнал с сообщениями пользователей чата.
	Пример работы класса Logger в многопоточном режиме приведен в конструкторе класса Chat.

	==============
	Важные моменты
	==============
	Задание параметров сети и базы данных производится в функции Main.cpp.
	Работа с сетью доступна только для ОС Linux.
	
	
