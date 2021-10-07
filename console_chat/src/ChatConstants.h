#pragma once


//===================================
// Тип счётчика пользователей и групп
// Чтобы не случилось, как в Telegram ( https://habr.com/ru/news/t/578872/ ):
#define USRNGRPIDTYPE int16_t 
// Для начала - до ~32766 пользователей и групп
// ! тип ДОЛЖЕН быть ЗНАКОВЫМ !
// id>=0 - id пользователей, id=0 - суперпользователь, id<0 - id групп
// 
// Максимальное число десятичных знаков для записи переменной типа USRNGRPIDTYPE
#define IDTYPEDIGITS10 3
// TODO: перевести на constexpr?

// Максимально допустимое число пользователей: root(id=0), id=1, id=2,...max-2
#define USRMAXCOUNT std::numeric_limits<USRNGRPIDTYPE>::max()-1
// Недопустимый индекс пользователя
#define USRWRONGID std::numeric_limits<USRNGRPIDTYPE>::max()
// Максимально допустимое число групп: id=min+1, id=min+2, id=min+3, ...-1
#define GRPMAXCOUNT std::numeric_limits<USRNGRPIDTYPE>::max()-1
// Недопустимый индекс группы
#define GRPWRONGID std::numeric_limits<USRNGRPIDTYPE>::min()


//=======================
// Тип счётчика сообщений
#define MESSAGEIDTYPE uint16_t
// Для экономии, пусть будет беззнаковым

// Максимально допустимое число сообщений
#define MSGMAXCOUNT std::numeric_limits<MESSAGEIDTYPE>::max()-1
// Недопустимый индекс сообщения
#define MSGWRONGID std::numeric_limits<MESSAGEIDTYPE>::max()
