#pragma once

#include <fstream>
#include <string>

class Message final {
private:
	std::string _text;
	std::string _sender;
	std::string _receiver;
public:
	Message() = default;
	~Message() = default;

	void setText(const std::string& text);
	void setSender(const std::string& sender);
	void setReceiver(const std::string& receiver);

	std::string getText() const;
	std::string getSender() const;
	std::string getReceiver() const;

	friend std::ostream& operator << (std::ostream& output, const Message& object);
};
