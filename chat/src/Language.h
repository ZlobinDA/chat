#pragma once

/** Доступные для использования языки: английский, русский. */
enum class LanguageType { english, russian };

/** Класс для управления параметрамя языка, используемого в приложении. */
class Language {
private:
	LanguageType _language;	/** язык, используемый в приложении. */
public:
	Language();
	Language(const LanguageType& type);
	~Language() = default;

	/** Проверить, использует ли приложение английский язык. */
	bool isEnglish() const;
	/** Проверить, использует ли приложение русский язык. */
	bool isRussian() const; 
};

