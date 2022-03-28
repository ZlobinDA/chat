#include "Language.h"

Language::Language() : _language(LanguageType::russian) {
}

bool Language::isEnglish() const {
	return (_language == LanguageType::english);
}

bool Language::isRussian() const {
	return (_language == LanguageType::russian);
}

void Language::set_lnguage(LanguageType language) {
	_language = language;
}
