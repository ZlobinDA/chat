#include "ChatUser.h"

ChatUser::ChatUser(std::string nickname, std::string fullname, std::string password) : _nickname(nickname), _fullname(fullname), _password(password),
                                                                                        _id(USRWRONGID), _registered(false), _groupsCount(0)
{
    _groupsIndexes = new USRNGRPIDTYPE * [GRPMAXCOUNT];
#ifdef _DEBUG
    std::cout << "[  OK  ] Created user " << fullname << " a.k.a " << nickname << std::endl;
#endif            
}

ChatUser::ChatUser(std::string rootPassword) : _nickname("root"), _fullname("root"), _password(rootPassword),
                                                _id(0), _registered(true), _groupsCount(0)
{
}

bool ChatUser::verifyRegistration() const
{
    if(_nickname.compare("root") != 0)
        return true;

    // TODO: проверка по списку/файлу других недопустимых лексем
    return false;

}

bool ChatUser::registerUser(USRNGRPIDTYPE id)
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

bool ChatUser::checkPassword(std::string password) const
{
    return password == _password;
}

USRNGRPIDTYPE ChatUser::getId() const
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
    // TODO: запилить вывод _groupsIndexes
    return retVal;
}


std::ostream& operator<<(std::ostream& output, const ChatUser* chatUser)
{
    output << chatUser->getInfo();
    return output;
}
