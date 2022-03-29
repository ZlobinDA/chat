#pragma once

/** ��������� ��� ������������� �����: ����������, �������. */
enum class LanguageType { english, russian };

/** ����� ��� ���������� ����������� �����, ������������� � ����������. */
class Language {
private:
	LanguageType _language;	/** ����, ������������ � ����������. */
public:
	Language();
	Language(const LanguageType& type);
	~Language() = default;

	/** ���������, ���������� �� ���������� ���������� ����. */
	bool isEnglish() const;
	/** ���������, ���������� �� ���������� ������� ����. */
	bool isRussian() const; 
};

