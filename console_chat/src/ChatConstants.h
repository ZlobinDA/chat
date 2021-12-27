#pragma once


//===================================
// Тип счётчика пользователей и групп "userNGroupIdType"
// Чтобы не случилось, как в Telegram ( https://habr.com/ru/news/t/578872/ ):
//typedef int16_t userNGroupIdType;
using userNGroupIdType = int16_t;
// Для начала - до ~32766 пользователей и групп
// ! тип ДОЛЖЕН быть ЗНАКОВЫМ !
// id>=0 - id пользователей, id=0 - суперпользователь, id<0 - id групп

// Максимальное число десятичных знаков для записи переменной типа userNGroupIdType
constexpr int idTypeDigits = 3;
// TODO: перевести на constexpr?

// Максимально допустимое число пользователей: root(id=0), id=1, id=2,...max-2
constexpr userNGroupIdType userMaxCount = std::numeric_limits<userNGroupIdType>::max() - 1;
// Недопустимый индекс пользователя
constexpr userNGroupIdType userWrongId = std::numeric_limits<userNGroupIdType>::max();
// Максимально допустимое число групп: id=min+1, id=min+2, id=min+3, ...-1
constexpr userNGroupIdType groupsMaxCount = std::numeric_limits<userNGroupIdType>::max() - 1;
// Недопустимый индекс группы
constexpr userNGroupIdType groupWrongId = std::numeric_limits<userNGroupIdType>::min();


//=======================
// Тип счётчика сообщений "messageIdType"
using messageIdType = uint16_t;
// Для экономии, пусть будет беззнаковым

// Максимально допустимое число сообщений
constexpr messageIdType messageMaxCount = std::numeric_limits<messageIdType>::max() - 1;
// Недопустимый индекс сообщения
constexpr messageIdType messageWrongId = std::numeric_limits<messageIdType>::max();

