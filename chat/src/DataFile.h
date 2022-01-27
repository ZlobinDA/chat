#pragma once

#include "Message.h"
#include "User.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

/** Шаблонный класс для работы с файлами. */
template <class T> class DataFile final {
private:
	std::string _path;	/** путь к файлу */
	T* _object;			/** указатель на объект класса, данные которого содержатся в файле */
public:
	/**
	* Конструктор.
	* При создании объекта класса выполняется проверка наличия файла.
	* Если файл найден, он открывается для чтения.
	*  Данные, считанные из файла, заносятся в поля объектов классов User и Message.
	*/
	DataFile(T* object, const std::string& path) : _path(path), _object(object) {
		std::cout << "Открываем файл " << _path << " для чтения" << std::endl;
		std::fstream _file(_path, std::ios::in);
		if (_file.is_open()) {
			std::cout << "Файл найден и успешно открыт" << std::endl;
			std::cout << "Считываем первоначальные данные из файла" << std::endl;
			if constexpr (std::is_same_v<std::vector<User>, T>) {
				while (!_file.eof()) {
					std::string _line;
					User user;
					// Считываем имя
					getline(_file, _line);
					// При нахождении в файле пустой строки, считаем, что записи в файле закончились.
					if (_line.empty()) {
						break;
					}
					user.setName(_line);
					// Считываем логин
					getline(_file, _line);
					user.setLogin(_line);
					// Считываем пароль
					getline(_file, _line);
					user.setFPassword(_line);
					user.setPassword();
					_object->emplace_back(user);
					std::cout << "Имя - " << user.getName() << ", логин - " <<
						user.getLogin() << ", пароль - " << user.getFPassword() << std::endl;
				}
			}
			else if constexpr (std::is_same_v<std::vector<Message>, T>) {
				while (!_file.eof()) {
					std::string _line;
					Message message;
					// Считываем сообщение 
					getline(_file, _line);
					// При нахождении в файле пустой строки, считаем, что записи в файле закончились.
					if (_line.empty()) {
						break;
					}
					message.setText(_line);
					// Считываем отправителя
					getline(_file, _line);
					message.setSender(_line);
					// Считываем получателя
					getline(_file, _line);
					message.setReceiver(_line);
 					_object->emplace_back(message);
					std::cout << "[" << message.getSender() << "] написал для " << 
						message.getReceiver() << " : " << message.getText() << std::endl;
				}
			}
			_file.close();
		}
		else {
			std::cout << "Внимание! Файл не найден" << std::endl;
		}
	}

	/** Деструктор. */
	~DataFile() = default;

	/** Запись состояния объектов в файл. */
	void writeState() {
		std::cout << "Открываем файл для записи текущего состояния объекта" << std::endl;
		std::fstream _file(_path, std::ios::out | std::ios::app);
		if (_file.is_open()) {
			std::cout << "Файл успешно открыт" << std::endl;
			// Изменяем права доступа к файлу: прочитать или записать файл может только пользователь,
			//  который запускает программу
			std::cout << "Изменяем права пользователя" << std::endl;
			fs::permissions(_path, fs::perms::group_all | fs::perms::others_all, fs::perm_options::remove);
			std::cout << "Записываем данные в файл" << std::endl;
			if constexpr (std::is_same_v< std::vector<User>, T>) {
				User currentObject = _object->back();
				// Записываем имя 
				_file << currentObject.getName() << std::endl;
				// Записываем логин
				_file << currentObject.getLogin() << std::endl;
				// Записываем пароль
				_file << currentObject.getFPassword() << std::endl;
			}
			else if constexpr (std::is_same_v< std::vector<Message>, T>) {
				Message currentObject = _object->back();
				// Записываем сообщение 
				_file << currentObject.getText() << std::endl;
				// Записываем отправителя
				_file << currentObject.getSender() << std::endl;
				// Записываем получателя
				_file << currentObject.getReceiver() << std::endl;
			}
			_file.close();
		}
		else {
			std::cout << "Внимание! Ошибка при создании файла" << std::endl;
		}
	};
};
