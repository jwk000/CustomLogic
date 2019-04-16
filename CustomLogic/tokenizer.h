#pragma once

#include "token.h"

class Tokenizer
{
public:
	enum {
		Eof = 0,
		WhiteSpace = 1,
		Error = 2,
		Digit = 3,
		Symbol = 4,
		Letter = 5,
		Quote = 6,

		Error_____ = Error,
		Digit_____ = Digit,
		Symbol____ = Symbol,
		Letter____ = Letter,
		Quote_____ = Quote,

	};

	static const char CodeMap[128];

	static char GetCode(char ch)
	{
		if (ch > 127)
			return Letter;
		else
			return CodeMap[ch];
	}

	TextBuffer m_text_buffer;
	bool m_error = false;
	Token* m_current_token;

	ErrorToken m_error_token;
	EofToken m_eof_token;
	NumberToken m_number_token;
	SymbolToken m_symbol_token;
	WordToken m_word_token;
	StringToken m_string_token;

	void Construct(std::string expression)
	{
		m_text_buffer.Construct(expression);
	}

	void Reset()
	{
		m_error = false;
		m_current_token = nullptr;
	}

	Token* GetCurrentToken()
	{
		return m_current_token;
	}

	Token* GetNextToken();

	char SkipWhiteSpace();
};
