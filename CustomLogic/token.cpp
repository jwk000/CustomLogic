#include "token.h"
#include "tokenizer.h"
#include "cl_interfaces.h"

bool NumberToken::FitType(TextBuffer& text_buffer)
{
	char code = Tokenizer::GetCode(text_buffer.Char());
	if (code == Tokenizer::Digit)
		return true;
	return false;
}

bool NumberToken::Get(TextBuffer& text_buffer)
{
	int start_index = text_buffer.CurrentIndex();
	int length = 1;
	while (true)
	{
		char ch = text_buffer.NextChar();
		if (ch == '.' || Tokenizer::GetCode(ch) == Tokenizer::Digit)
			++length;
		else
			break;
	}
	std::string str = text_buffer.SubString(start_index, length);
	m_value = std::stof(str);
	return true;
}

bool StringToken::FitType(TextBuffer& text_buffer)
{
	char code = Tokenizer::GetCode(text_buffer.Char());
	if (code == Tokenizer::Quote)
		return true;
	return false;
}

bool StringToken::Get(TextBuffer& text_buffer)
{
	int start_index = text_buffer.CurrentIndex() + 1;
	int length = 0;
	while (true)
	{
		char ch = text_buffer.NextChar();
		if (text_buffer.Eof())
			break;
		if (Tokenizer::GetCode(ch) == Tokenizer::Quote)
			break;
		++length;
	}
	m_raw_string = text_buffer.SubString(start_index, length);
	return true;
}

std::unordered_map<std::string, TokenType> SymbolToken::ms_reserved_symbol;

bool SymbolToken::FitType(TextBuffer& text_buffer)
{
	char code = Tokenizer::GetCode(text_buffer.Char());
	if (code == Tokenizer::Symbol)
		return true;
	return false;
}

bool SymbolToken::Get(TextBuffer& text_buffer)
{
	char ch = text_buffer.Char();
	switch (ch)
	{
	case '+':
		m_type = TokenType::PLUS;
		text_buffer.NextChar();
		break;
	case '-':
		m_type = TokenType::MINUS;
		text_buffer.NextChar();
		break;
	case '*':
		m_type = TokenType::STAR;
		text_buffer.NextChar();
		break;
	case '/':
		m_type = TokenType::SLASH;
		text_buffer.NextChar();
		break;
	case '(':
		m_type = TokenType::LEFT_PAREN;
		text_buffer.NextChar();
		break;
	case ')':
		m_type = TokenType::RIGHT_PAREN;
		text_buffer.NextChar();
		break;
	case '[':
		m_type = TokenType::LEFT_BRACKET;
		text_buffer.NextChar();
		break;
	case ']':
		m_type = TokenType::RIGHT_BRACKET;
		text_buffer.NextChar();
		break;
	case '.':
		m_type = TokenType::PERIOD;
		text_buffer.NextChar();
		break;
	case ',':
		m_type = TokenType::COMMA;
		text_buffer.NextChar();
		break;
	case ':':
		m_type = TokenType::COLON;
		text_buffer.NextChar();
		break;
	case '=':
	{
		m_type = TokenType::SET;
		char nextch = text_buffer.NextChar();
		if (nextch == '=')
		{
			m_type = TokenType::EQUAL;
			text_buffer.NextChar();
		}
		break;
	}
	case '!':
	{
		m_type = TokenType::NOT;
		char nextch = text_buffer.NextChar();
		if (nextch == '=')
		{
			m_type = TokenType::NOT_EQUAL;
			text_buffer.NextChar();
		}
		break;
	}
	case '<':
	{
		m_type = TokenType::LESS_THAN;
		char nextch = text_buffer.NextChar();
		if (nextch == '=')
		{
			m_type = TokenType::LESS_EQUAL;
			text_buffer.NextChar();
		}
		break;
	}
	case '>':
	{
		m_type = TokenType::GREATER_THAN;
		char nextch = text_buffer.NextChar();
		if (nextch == '=')
		{
			m_type = TokenType::GREATER_EQUAL;
			text_buffer.NextChar();
		}
		break;
	}
	case '&':
	{
		m_type = TokenType::AND;
		text_buffer.NextChar();
		break;
	}
	case '|':
	{
		m_type = TokenType::OR;
		text_buffer.NextChar();
		break;
	}
	case '#':
	{
		int start_index = text_buffer.CurrentIndex();
		int length = 1;
		char nch = text_buffer.NextChar();
		char code = Tokenizer::GetCode(nch);
		while (code != Tokenizer::WhiteSpace)
		{
			nch = text_buffer.NextChar();
			code = Tokenizer::GetCode(nch);
			++length;
		}
		std::string jstr = text_buffer.SubString(start_index, length);
		auto it = ms_reserved_symbol.find(jstr);
		if (ms_reserved_symbol.end() == it)
		{
			LogError("Expression: SymbolToken.Get() # Error!");
			m_type = TokenType::ERROR;
		}
		m_type = it->second;
	}
	break;
	default:
		LogError("Expression: SymbolToken.Get(), illegal symbol, index = ", text_buffer.CurrentIndex(), ", char = ", text_buffer.Char());
		m_type = TokenType::ERROR;
		return false;
	}
	return true;
}

std::unordered_map<std::string, TokenType> WordToken::ms_reserved_words;

bool WordToken::FitType(TextBuffer& text_buffer)
{
	char code = Tokenizer::GetCode(text_buffer.Char());
	if (code == Tokenizer::Letter)
		return true;
	return false;
}

bool WordToken::Get(TextBuffer& text_buffer)
{
	int start_index = text_buffer.CurrentIndex();
	int length = 1;
	while (true)
	{
		char ch = text_buffer.NextChar();
		char code = Tokenizer::GetCode(ch);
		if (code != Tokenizer::Letter && code != Tokenizer::Digit)
		{
			break;
		}
		++length;
	}
	std::string str = text_buffer.SubString(start_index, length);
	auto it = ms_reserved_words.find(str);

	if (ms_reserved_words.end() == it)
	{
		m_type = TokenType::IDENTIFIER;
		m_raw_string = str;
	}
	else
	{
		m_type = it->second;
	}

	return true;
}
