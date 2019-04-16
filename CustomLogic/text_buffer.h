#pragma once
#include <string>

class TextBuffer 
{
	std::string m_buffer;
	int m_index = -1;

public:
	void Construct(std::string buffer)
	{
		m_buffer = buffer;
		m_index = 0;
	}

	void Reset()
	{
		m_buffer.clear();
		m_index = -1;
	}

	 int CurrentIndex()
	{
		 return m_index; 
	}

	 bool Eof()
	{
		return m_index >= m_buffer.length() || m_buffer[m_index] == 0;
	}

	 char Char()
	{
		if (Eof())
			return '\0';
		return m_buffer[m_index];
	}

	 char NextChar()
	{
		++m_index;
		if (Eof())
			return '\0';
		return m_buffer[m_index];
	}

	 std::string SubString(int start_index, int length)
	{
		return m_buffer.substr(start_index, length);
	}

	 bool StartsWith(std::string str)
	{
		 if (m_buffer.length() < str.length()) return false;

		 return m_buffer.substr(0, str.length()) == str;
	}

};
