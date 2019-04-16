#pragma once
#include<vector>
#include "expression.h"


class DSLCode
{
	std::vector<Expression*> m_expressions;

public:
	void Reset()
	{
		for (auto exp : m_expressions)
		{
			delete exp;
		}
		m_expressions.clear();
	}

	std::vector<std::string> StrSplit(const std::string& rawStr, const std::string& delimier)
	{
		std::vector<std::string> ret;
		size_t offset = 0;
		size_t idx = rawStr.find_first_of(delimier, offset);
		while (idx != std::string::npos)
		{
			ret.emplace_back(rawStr.substr(offset, idx-offset));
			offset = idx+1;
			idx = rawStr.find_first_of(delimier, offset);
		}
		ret.emplace_back(rawStr.substr(offset));
		return ret;
	}

	bool Compile(const std::string& rawStr)
	{
		std::vector<std::string> codeStr = StrSplit(rawStr, ";\n");
		for (int line = 0; line < codeStr.size(); ++line)
		{
			std::string sub = codeStr[line];
			if (sub.empty())
				continue;

			auto expression = new Expression();
			if (expression->Compile(sub))
			{
				m_expressions.push_back(expression);
			}
			else
			{
				LogError("DSL Compile ERROR  line:", line);
				return false;
			}
		}
		return true;
	}

	void Execute(VariablesLib* variable_env)
	{
		for (int i = 0; i < m_expressions.size(); ++i)
		{
			m_expressions[i]->Evaluate(variable_env);
		}
	}
};
