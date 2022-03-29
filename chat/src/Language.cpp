// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "Language.h"

Language::Language() : _language(LanguageType::english) {
}

Language::Language(const LanguageType& type) {
	_language = type;
}

bool Language::isEnglish() const {
	return (_language == LanguageType::english);
}

bool Language::isRussian() const {
	return (_language == LanguageType::russian);
}
