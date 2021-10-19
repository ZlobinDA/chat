#include "ChatUsersList.h"

ChatUsersList::ChatUsersList(std::string rootPassword) // Программу запускает администратор, делая себя нулевым пользователем
{
	_usersList = new ChatUser * [USRMAXCOUNT];
	ChatUser* rootUser = new ChatUser(rootPassword);
	_usersCount = 0;
	_usersList[0] = rootUser;
}

ChatUsersList::~ChatUsersList()
{
	for (USRNGRPIDTYPE i = 0; i <= _usersCount; i++) // удаляем всех, включая администратора
	{
		delete _usersList[i];
	}
	delete[] _usersList;
#ifdef _DEBUG
	std::cout << "[  OK  ] Users list is empty" << std::endl;
#endif
}

USRNGRPIDTYPE ChatUsersList::getUsersCount() const
{
	return _usersCount;
}

bool ChatUsersList::isEmpty() const
{
	return (_usersCount == 0);
}

bool ChatUsersList::isFull() const
{
	return (_usersCount == USRMAXCOUNT);
}

std::string ChatUsersList::getNickname(USRNGRPIDTYPE id)
{
	if (id <= _usersCount)
		return _usersList[id]->getNickname();
	else
		return std::string();
}

std::string ChatUsersList::getFullname(USRNGRPIDTYPE id)
{
	if (id <= _usersCount)
		return _usersList[id]->getFullname();
	else
		return std::string();
}

bool ChatUsersList::addUser(ChatUser* newUser)
{
	if (_usersCount < USRMAXCOUNT)
	{
		if (newUser->isRegistered())
		{
			_usersList[++_usersCount] = newUser;
#ifdef _DEBUG
			std::cout << "[  OK  ] Added " << newUser << " to position N" << int(_usersCount) << std::endl;
#endif
			return true;
		}
		else
		{
#ifdef _DEBUG
			std::cout << "[FAILED] User " << newUser << " is not registered!" << std::endl;
#endif
			return false;
		}
	}
	else
	{
#ifdef _DEBUG
		std::cout << "[FAILED] Failed to add " << newUser << " ! UsersCount = " << int(_usersCount) << std::endl;
#endif
		return false;
	}
}

bool ChatUsersList::deleteUserById(USRNGRPIDTYPE id)
{
	ChatUser* userToDelete;

	if (id == 0)
	{
#ifdef _DEBUG
		std::cout << "[FAILED] Root is god!" << std::endl;
#endif
		return false;
	}

	if ((id <= _usersCount) && (id > 0))
	{
		userToDelete = _usersList[id];
#ifdef _DEBUG
		std::cout << "[  OK  ] User " << id << ") " << userToDelete << " was deleted" << std::endl;
#endif
		for (USRNGRPIDTYPE i = id; i < _usersCount - 1; ++i)
		{
			_usersList[i] = _usersList[i + 1]; // Поочерёдно сдвигаем пользователей вниз по списку
			_usersList[i]--; // Уменьшаем идентификатор _id у сдвинутого вниз пользователя.
		}
		--_usersCount;
		delete userToDelete;
		return true;
	}
	else
	{
#ifdef _DEBUG
		std::cout << "[FAILED] Failed to delete user N " << id << " ! UsersCount = " << _usersCount << std::endl;
#endif
		return false;
	}

}

USRNGRPIDTYPE ChatUsersList::findUserByNickname(std::string nickname) const
{
	for (USRNGRPIDTYPE i = 1; i <= _usersCount; i++)
	{
		if (_usersList[i]->getNickname().compare(nickname) == 0)
		{
			return i;
		}
	}
	return USRWRONGID;
}

bool ChatUsersList::deleteUserByNickname(std::string nickname)
{
	USRNGRPIDTYPE id = this->findUserByNickname(nickname);
	return this->deleteUserById(id);
}

void ChatUsersList::clearList()
{
	for (USRNGRPIDTYPE i = 1; i < _usersCount; i++) // Удаляем всех, кроме root
	{
		delete _usersList[i];
	}
	_usersCount = 0;
}

bool ChatUsersList::checkPassword(USRNGRPIDTYPE id, std::string password) const
{
	return _usersList[id]->checkPassword(password);
}

/*
Сохранение текущего списка пользователей в текстовый файл.
Файл создаётся, либо перезаписывается.
TODO: обработка исключений записи (кончилось место на диске и т.п.) 
*/
bool ChatUsersList::saveToFile()
{
	std::string fname = FILEUSRLISTPREFIX + std::string("") + FILEUSRLISTPOSTFIX;
	
	// Документация по fstream : https://www.cplusplus.com/doc/tutorial/files/
	std::fstream usersFile(fname, std::ios::out | std::ios::trunc);
	if(usersFile.is_open())
	{
#ifdef _DEBUG
		std::cout << "[  OK  ] Opened '" << fname << "' for writing" << std::endl;
#endif
		usersFile << _usersCount << std::endl; // В самом начале записываем число пользователей
		for (USRNGRPIDTYPE i = 1; i <= _usersCount; ++i)
		{
			usersFile << "==========================================" << std::endl; // Может быть любой разделитель в одну строку
			usersFile << _usersList[i]->getNickname() << std::endl;
			usersFile << _usersList[i]->getFullname() << std::endl;
			usersFile << _usersList[i]->getPassword() << std::endl;
			// TODO: сохранение _groupsCount и _groupsIndexes!
		}
		usersFile.close();
#ifdef _DEBUG
		std::cout << "[  OK  ] Closed '" << fname << "'" << std::endl;
#endif
		return true;
	}
	else
	{
#ifdef _DEBUG
		std::cout << "[FAILED] Can't open '" << fname << "' for writing" << std::endl;
#endif
		return false;
	}

	return false;
}


/*
Чтение списка пользователей из текстового файла.
Если файл не пустой, вначале текущее содержимое списка очищается.
TODO: обработка ошибок и исключений при чтении (неверный формат и т.п.)
*/
bool ChatUsersList::loadFromFile()
{
	std::string fname = FILEUSRLISTPREFIX + std::string("") + FILEUSRLISTPOSTFIX;
	
	ChatUser* newUser;
	std::string tmpBuffer, nickname, fullname, password;
	USRNGRPIDTYPE usersCount;

	std::fstream usersFile(fname, std::ios::in);
	if (usersFile.is_open())
	{
#ifdef _DEBUG
		std::cout << "[  OK  ] Opened '" << fname << "' for reading" << std::endl;
#endif

		usersFile >> usersCount;					// Читаем число пользователей в файле
		std::getline(usersFile, tmpBuffer);			// Перенос строки
		if (usersCount > 0)
			this->clearList();						// Чистим базу пользователей перед чтением новой из файла

		for (USRNGRPIDTYPE i = 1; i <= usersCount; ++i)
		{
			std::getline(usersFile, tmpBuffer);		// Пропускаем разделитель в одну строку
			//std::cout << tmpBuffer << std::endl;

			usersFile >> nickname;					// nickname
			std::getline(usersFile, tmpBuffer);		// Перенос строки
			//std::cout << "nickname:" << nickname << std::endl;

			std::getline(usersFile, fullname);		// fullname - читаем так, потому что может содержать пробел!
			//std::cout << "fullname:" << fullname << std::endl;

			usersFile >> password;					// password
			std::getline(usersFile, tmpBuffer);		// Перенос строки
			//std::cout << "password:" << password << std::endl;
			
			// TODO: чтение _groupsCount и _groupsIndexes!

#ifdef _DEBUG
			std::cout << "[  OK  ] Creating user '" << nickname << "-" << fullname << "-" << password << "' from file data" << std::endl;
#endif

			newUser = new ChatUser(nickname, fullname, password);
			newUser->registerUser(i);
			this->addUser(newUser);

		}
		usersFile.close();
#ifdef _DEBUG
		std::cout << "[  OK  ] Closed '" << fname << "'" << std::endl;
#endif
		return true;
	}
	else
	{
#ifdef _DEBUG
		std::cout << "[FAILED] Can't open '" << fname << "' for reading" << std::endl;
#endif
		return false;
	}
		
	return false;
}