#pragma once

enum e_language
{
	k_first_language = 0,

	_language_english = k_first_language,
	_language_japanese,
	_language_german,
	_language_french,
	_language_spanish,
	_language_mexican_spanish,
	_language_italian,
	_language_korean,
	_language_chinese_traditional,
	_language_chinese_simplified,
	_language_portuguese,
	_language_russian, // polish replaced with russian

	k_language_count,

	_language_invalid = -1,

	k_language_default = k_first_language
};

e_language __cdecl get_current_language();