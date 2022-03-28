#pragma once

/** ��������� ��� ������������� �����: ����������, �������. */
enum class LanguageType { english, russian };

/** ����� ��� ���������� ����������� �����, ������������� � ����������. */
class Language {
private:
	LanguageType _language;	/** ����, ������������ � ����������. */
public:
	Language();
	~Language() = default;

	/** ���������, ���������� �� ���������� ���������� ����. */
	bool isEnglish() const;
	/** ���������, ���������� �� ���������� ������� ����. */
	bool isRussian() const; 
	/** ���������� ���� ����������. */
	void set_lnguage(LanguageType language);
};

