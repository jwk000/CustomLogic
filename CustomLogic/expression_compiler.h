#pragma once
#include "variable.h"
#include <vector>
#include "tokenizer.h"
#include "operation.h"

struct InsType
{
	union {
		long int_value;
		double float_value;
	} _inner_value;

	InsType(long v)
	{
		_inner_value.int_value = v;
	}
	InsType(double v) 
	{
		_inner_value.float_value = v;
	}

	InsType& operator=(long v)
	{
		_inner_value.int_value = v;
		return *this;
	}

	InsType& operator=(double v)
	{
		_inner_value.float_value = v;
		return *this;
	}

	operator long()
	{
		return _inner_value.int_value;
	}

	operator double()
	{
		return _inner_value.float_value;
	}
};

class ExpressionCompiler
{
	bool m_error_occurred = false;
	std::vector<Variable> m_variables;
	std::vector<InsType> m_instructions;//64位系统下用long有点浪费，数据都是float就够了

	Tokenizer m_tokenizer;
	Token* m_token;
	TokenType m_token_type;
	std::vector<std::string> m_raw_variable;

	OperationSetting m_op_setting;
public:
	std::vector<Variable>& Variables() { return m_variables; }
	std::vector<InsType>& Instructions() { return m_instructions; }

	virtual void Reset()
	{
		m_error_occurred = false;
		m_variables.clear();
		m_instructions.clear();
	}

	virtual bool Compile(std::string expression_string);

	std::vector<Variable>& GetAllVariables()
	{
		return m_variables;
	}

	bool IsConstant()
	{
		return m_variables.empty();
	}

	void GetToken();

	void ParseExpression();

	void ParseBinaryOperator(int symbolPriority);

	void ParseHighPriorityBinary(int symbolPriority);

	void ParseFactor();

	void ParseBuildInFunction(OperationCode opcode, int param_count);

	void AppendOperation(OperationCode op_code)
	{
		m_instructions.push_back((long)op_code);
	}
	void AppendOperation(int op_code)
	{
		m_instructions.push_back((long)op_code);
	}

	void AppendNumber(float number)
	{
		AppendOperation(OperationCode::PUSH_NUMBER);
		m_instructions.push_back((double)number);
	}

	void AppendVariable(const std::vector<std::string>& variable)
	{
		AppendOperation(OperationCode::PUSH_VARIABLE);
		int index = AddVariable(variable);
		m_instructions.push_back((long)index);
	}

	void SetVariable(const std::vector<std::string>& variable)
	{
		AppendOperation(OperationCode::SET_VARIABLE);
		int index = AddVariable(variable);
		m_instructions.push_back((long)index);
	}

	int AddVariable(const std::vector<std::string>& raw_variable)
	{
		Variable variable ;
		variable.Construct(raw_variable);
		int index = (int)m_variables.size();
		m_variables.push_back(variable);
		return index;
	}

};
