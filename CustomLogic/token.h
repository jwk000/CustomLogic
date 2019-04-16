#pragma once

#include <string>
#include <unordered_map>
#include "text_buffer.h"


enum class TokenType
{
	ERROR = 0,
	TOKEN_EOF,

	//Value
	NUMBER,

	//Symbol
	PLUS,
	MINUS,
	STAR,
	SLASH,
	LEFT_PAREN,
	RIGHT_PAREN,
	LEFT_BRACKET,
	RIGHT_BRACKET,
	PERIOD,
	COMMA,
	COLON,

	//Comparison
	GREATER_THAN,
	LESS_THAN,
	GREATER_EQUAL,
	LESS_EQUAL,
	EQUAL,
	NOT_EQUAL,

	//BooleanLogic
	NOT,
	AND,
	OR,

	//Assignment
	SET,

	// Word-------------------
	//Function
	SINE,
	COSINE,
	TANGENT,
	SQUARE_ROOT,
	MINIMUM,
	MAXIMUM,
	CLAMP,
	//Variable
	IDENTIFIER,

	STRING_TYPE,
};

class Token
{
protected:
	TokenType m_type;

public:
	TokenType Type()
	{
		return m_type;
	}

	virtual bool FitType(TextBuffer& text_buffer) = 0;

	virtual bool Get(TextBuffer& text_buffer) = 0;

	virtual float GetNumber()
	{
		return 0;
	}

	virtual std::string GetRawString()
	{
		return "";
	}
};

class ErrorToken : public Token
{
public:
	ErrorToken()
	{
		m_type = TokenType::ERROR;
	}
	bool FitType(TextBuffer& text_buffer) override
	{
		return true;
	}
	bool Get(TextBuffer& text_buffer) override
	{
		return true;
	}
};

class EofToken :public Token
{
public:
	EofToken()
	{
		m_type = TokenType::TOKEN_EOF;
	}
	bool FitType(TextBuffer& text_buffer)override
	{
		return true;
	}
	bool Get(TextBuffer& text_buffer)override
	{
		return true;
	}
};

class NumberToken : public Token
{
	float m_value;

public:
	NumberToken()
	{
		m_type = TokenType::NUMBER;
	}

	bool FitType(TextBuffer& text_buffer)override;

	bool Get(TextBuffer& text_buffer)override;

	float GetNumber()override
	{
		return m_value;
	}
};

class StringToken :public Token
{
	std::string m_raw_string;

public:
	StringToken()
	{
		m_type = TokenType::STRING_TYPE;
	}

	bool FitType(TextBuffer& text_buffer)override;

	bool Get(TextBuffer& text_buffer)override;

	std::string GetRawString()override
	{
		return m_raw_string;
	}
};

class SymbolToken :public Token
{

public:
	static std::unordered_map<std::string, TokenType> ms_reserved_symbol;
	SymbolToken()
	{
		//定义一些符号的别名 （例如xml中不能出现<&, 此时可以用别名）
		ms_reserved_symbol["#LT"] = TokenType::LESS_THAN;
		ms_reserved_symbol["#GT"] = TokenType::GREATER_THAN;
		ms_reserved_symbol["#LE"] = TokenType::LESS_EQUAL;
		ms_reserved_symbol["#GE"] = TokenType::GREATER_EQUAL;
		ms_reserved_symbol["#AND"] = TokenType::AND;
		ms_reserved_symbol["#And"] = TokenType::AND;
		ms_reserved_symbol["#and"] = TokenType::AND;
		ms_reserved_symbol["#OR"] = TokenType::OR;
		ms_reserved_symbol["#Or"] = TokenType::OR;
		ms_reserved_symbol["#or"] = TokenType::OR;
	}

	bool FitType(TextBuffer& text_buffer)override;

	bool Get(TextBuffer& text_buffer)override;
};

class WordToken :public Token
{
	static std::unordered_map<std::string, TokenType> ms_reserved_words;
	std::string m_raw_string;
public:
	WordToken()
	{
		ms_reserved_words["Sin"] = TokenType::SINE;
		ms_reserved_words["sin"] = TokenType::SINE;
		ms_reserved_words["Cos"] = TokenType::COSINE;
		ms_reserved_words["cos"] = TokenType::COSINE;
		ms_reserved_words["Tan"] = TokenType::TANGENT;
		ms_reserved_words["tan"] = TokenType::TANGENT;
		ms_reserved_words["Sqrt"] = TokenType::SQUARE_ROOT;
		ms_reserved_words["sqrt"] = TokenType::SQUARE_ROOT;
		ms_reserved_words["Min"] = TokenType::MINIMUM;
		ms_reserved_words["min"] = TokenType::MINIMUM;
		ms_reserved_words["Max"] = TokenType::MAXIMUM;
		ms_reserved_words["max"] = TokenType::MAXIMUM;
		ms_reserved_words["Clamp"] = TokenType::CLAMP;
		ms_reserved_words["clamp"] = TokenType::CLAMP;
	}

	bool FitType(TextBuffer& text_buffer)override;

	bool Get(TextBuffer& text_buffer)override;

	std::string GetRawString()override
	{
		return m_raw_string;
	}
};
