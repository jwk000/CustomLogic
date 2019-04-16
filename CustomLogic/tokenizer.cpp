#include "tokenizer.h"


const char Tokenizer::CodeMap[128] =
{
	/*     0          1            2           3           4           5           6           7           8           9               */
	/*  0*/WhiteSpace, Error_____, Error_____, Error_____, Error_____, Error_____, Error_____, Error_____, Error_____, WhiteSpace,/*  0*/
	/* 10*/WhiteSpace, Error_____, Error_____, WhiteSpace, Error_____, Error_____, Error_____, Error_____, Error_____, Error_____,/* 10*/
	/* 20*/Error_____, Error_____, Error_____, Error_____, Error_____, Error_____, Error_____, Error_____, Error_____, Error_____,/* 20*/
	/* 30*/Error_____, Error_____, WhiteSpace, Error_____, Quote_____, Symbol____, Error_____, Error_____, Symbol____, Error_____,/* 30*/
	/* 40*/Symbol____, Symbol____, Symbol____, Symbol____, Symbol____, Symbol____, Symbol____, Symbol____, Digit_____, Digit_____,/* 40*/
	/* 50*/Digit_____, Digit_____, Digit_____, Digit_____, Digit_____, Digit_____, Digit_____, Digit_____, Symbol____, Error_____,/* 50*/
	/* 60*/Symbol____, Symbol____, Symbol____, Error_____, Error_____, Letter____, Letter____, Letter____, Letter____, Letter____,/* 60*/
	/* 70*/Letter____, Letter____, Letter____, Letter____, Letter____, Letter____, Letter____, Letter____, Letter____, Letter____,/* 70*/
	/* 80*/Letter____, Letter____, Letter____, Letter____, Letter____, Letter____, Letter____, Letter____, Letter____, Letter____,/* 80*/
	/* 90*/Letter____, Symbol____, Error_____, Symbol____, Error_____, Letter____, Error_____, Letter____, Letter____, Letter____,/* 90*/
	/*100*/Letter____, Letter____, Letter____, Letter____, Letter____, Letter____, Letter____, Letter____, Letter____, Letter____,/*100*/
	/*110*/Letter____, Letter____, Letter____, Letter____, Letter____, Letter____, Letter____, Letter____, Letter____, Letter____,/*110*/
	/*120*/Letter____, Letter____, Letter____, Error_____, Symbol____, Error_____, Error_____, Error_____/*127*/
};

Token* Tokenizer::GetNextToken()
{
	if (m_error)
	{
		m_current_token = &m_error_token;
	}
	else if (m_text_buffer.Eof())
	{
		m_current_token = &m_eof_token;
	}
	else
	{
		char code = SkipWhiteSpace();
		if (m_error)
		{
			m_current_token = &m_error_token;
		}
		else if (m_text_buffer.Eof())
		{
			m_current_token = &m_eof_token;
		}
		else
		{
			if (m_word_token.FitType(m_text_buffer))
			{
				m_current_token = &m_word_token;
			}
			else if (m_number_token.FitType(m_text_buffer))
			{
				m_current_token = &m_number_token;
			}
			else if (m_symbol_token.FitType(m_text_buffer))
			{
				m_current_token = &m_symbol_token;
			}
			else if (m_string_token.FitType(m_text_buffer))
			{
				m_current_token = &m_string_token;
			}
			else
			{
				m_error = true;
				m_current_token = &m_error_token;
			}

			if (!m_current_token->Get(m_text_buffer))
			{
				m_error = true;
				m_current_token = &m_error_token;
			}
		}
	}
	return m_current_token;
}

char Tokenizer::SkipWhiteSpace()
{
	while (!m_text_buffer.Eof())
	{
		char code = Tokenizer::GetCode(m_text_buffer.Char());
		switch (code)
		{
		case WhiteSpace:
			break;
		case Error:
			m_error = true;
			//LogError("Expression: Tokenizer.SkipWhiteSpace(), illegal character in the buffer, index = ", m_text_buffer.CurrentIndex, ", char = ", m_text_buffer.Char());
			return code;
		default:
			return code;
		}
		m_text_buffer.NextChar();
	}
	return Eof;
}
