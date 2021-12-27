#include "ChatUser.h"

ChatUser::ChatUser(const std::string& nickname, const std::string& fullname, const std::string& password) : 
                    _nickname(nickname), _fullname(fullname), _password(password), _id(userWrongId), _registered(false)
{
//    _groupsIndexes = new userNGroupIdType * [groupsMaxCount]; // Пока работаем без групп
#ifdef _DEBUG
    std::cout << "[  OK  ] Created user " << fullname << " a.k.a " << nickname << std::endl;
#endif            
}

/*
Конструктор для пользователя с UID 0.
registerUser для него не требуется.
*/
ChatUser::ChatUser(const std::string& rootPassword) : _nickname("root"), _fullname("root"), _password(rootPassword),
                                                _id(0), _registered(true)
{
}

/*
private-функция для проверки допустимости введённых значений Имя-Ник-Пароль
Вынесена отдельно, т.к. должна содержать поиск по базе лексем
*/
bool ChatUser::verifyRegistration() const
{
    if(_nickname.compare("root") == 0)
        return false;
    // TODO: проверка по списку/файлу других недопустимых лексем
    
    if(_password.length() == 0)
        return false;
    
    return true;


}

/*
Функция регистрации пользователя.
Присваивает уникальный идентификатор пользователя при условии допустимости введённых значений
*/
bool ChatUser::registerUser(const userNGroupIdType& id)
{
    if (verifyRegistration())
    {
        _id = id;
        _registered = true;
#ifdef _DEBUG
        std::cout << "[  OK  ] " << "Registered User " << int(this->_id) << ": " << this->_fullname << " a.k.a " << this->_nickname << std::endl;
#endif
        return true;
    }
    else
    {
#ifdef _DEBUG
        std::cout << "[FAILED] " << "Can't register " << int(this->_id) << ": " << this->_fullname << " a.k.a " << this->_nickname << std::endl;
#endif
        return false;
    }
}

ChatUser ChatUser::operator--()
{
    _id--;
    return *this;
}

bool ChatUser::isRegistered() const
{
    return _registered;
}

std::string ChatUser::getNickname() const
{
    return _nickname;
}

std::string ChatUser::getFullname() const
{
    return _fullname;
}

std::string ChatUser::getPassword() const
{
    return _password;
}

bool ChatUser::checkPassword(const std::string& password) const
{
    return (_password.compare(password) == 0);
}

userNGroupIdType ChatUser::getId() const
{
    return _id;
}

std::string ChatUser::getInfo() const
{
    std::string retVal = "";
    retVal = retVal + "User " + std::to_string(this->_id) + ": " + this->_fullname + " a.k.a " + this->_nickname;
    if (this->_registered)
        retVal = retVal + ", registered";
    else
        retVal = retVal + ", not registered";
    // TODO: добавить вывод _groupsIndexes
    return retVal;
}

std::ostream& operator<<(std::ostream& output, const ChatUser* chatUser)
{
    output << chatUser->getInfo();
    return output;
}
