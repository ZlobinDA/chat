#pragma once

#include "sha1.h" // uint*
#include <string> // string
#include <unordered_map> // unordered_map

// ������������ ����� �������������, ������� ����� ������������������ � ����.
// ���������� ��������� ��������, ����� �������������� ������ ���� ��� ������������ ���� �������������.
// ��������� ������ ���� ��� 3 �������������, �.�. 1 ������������ - �������������.
constexpr int maxUsers = 4;

class Chat {
private:
	// ��������� ��� �������� ������ ������������.
	struct UserData {
		std::string _name;
		uint* _password;

		UserData();
		UserData(const UserData& other);
		UserData(const std::string& password, const std::string& name = "noName");
		~UserData();

		UserData& operator = (const UserData& other);
	};

	// ���������� ������������� ��������� unordered_map ��� �������� ���� ������ �������������:
	//	- ����� ������������ ����������, ������� ������ ������ � ���� ����
	//	 ���� (�����) - �������� (��������� � ������� � ������������);
	//	- ������ � ������������� ����� ������� ��������������;
	//	- ������� �����, ������� � �������� ������������.
	std::unordered_map <std::string, UserData> _dataBase;

	/** ������ �������������� ������ ������������. ����� ��� ������ ���������� ����������. */
	void printPassword(const uint*);
public:
	Chat();
	~Chat();

	/** ���������� ������ ������������ � ���� ������������� ����. */
	void addUser(const std::string& login, const std::string& password, const std::string& name = "noName");

	/**
	* ����� ��������� ������������ ��������� ������������� ������
	*  � ������, ������������ ��� ����������� ������������.
	*/
	bool checkPassword(const std::string& login, const std::string& password);

	/** ����� ���������� ����� �������������, ������������������ � ����. */
	size_t getUsersCount() const;

	/**
	* ����� ���������, ���������� �� ������������ � ������ ������� � ���� ����.
	* ���� ������������ ������ ����� ���������� true, ����� false.
	*/
	bool findLogin(const std::string& login) const;

	/** ����� ���������� ��� ������������ �� ��� ������. */
	std::string getName(const std::string& login) const;

	/** ����� ���������� ������ � �������� ������������������ �������������. */
	std::vector <std::string> getUsersList() const;
};