// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "Message.h"

void Message::setText(const std::string& text) {
	_text = text;
}

void Message::setSender(const std::string& sender) {
	_sender = sender;
}

void Message::setReceiver(const std::string& receiver) {
	_receiver = receiver;
}

std::string Message::getText() const {
	return _text;
}

std::string Message::getSender() const {
	return _sender;
}

std::string Message::getReceiver() const {
	return _receiver;
}

std::ostream& operator << (std::ostream& output, const Message& object) {
	output << "Состояние объекта класса Message: текст - " << object._text <<
		", отправитель - " << object._sender <<
		", получатель - " << object._receiver << std::endl;
	return output;
}
