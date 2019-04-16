#pragma once
#include <unordered_map>
#include "token.h"

enum class OperationCode
{
	END = 0,
	NEGATE,
	ADD,
	SUBTRACT,
	MULTIPLY,
	DIVIDE,

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

	SIN,
	COS,
	TAN,
	SQRT,
	MIN,
	MAX,
	CLAMP,

	PUSH_NUMBER,
	PUSH_VARIABLE,
	SET_VARIABLE,
};

class OperationSetting
{
	std::unordered_map<int, int> TokenTypeToPriority ;
	std::unordered_map<int, OperationCode> TokenTypeToOperation;

public:
	enum {
		MaxBinaryPriority=3,
		MinBinaryPriority=0,
	};
	 static bool isMaxBinaryPriority(int priority)
	{
		return priority >= MaxBinaryPriority;
	}

	 OperationSetting()
	{

		AddBinaryOPToken(TokenType::STAR, 3, OperationCode::MULTIPLY);
		AddBinaryOPToken(TokenType::SLASH, 3, OperationCode::DIVIDE);

		AddBinaryOPToken(TokenType::PLUS, 2, OperationCode::ADD);
		AddBinaryOPToken(TokenType::MINUS, 2, OperationCode::SUBTRACT);

		AddBinaryOPToken(TokenType::GREATER_THAN, 1, OperationCode::GREATER_THAN);
		AddBinaryOPToken(TokenType::LESS_THAN, 1, OperationCode::LESS_THAN);
		AddBinaryOPToken(TokenType::GREATER_EQUAL, 1, OperationCode::GREATER_EQUAL);
		AddBinaryOPToken(TokenType::LESS_EQUAL, 1, OperationCode::LESS_EQUAL);
		AddBinaryOPToken(TokenType::EQUAL, 1, OperationCode::EQUAL);
		AddBinaryOPToken(TokenType::NOT_EQUAL, 1, OperationCode::NOT_EQUAL);
		AddBinaryOPToken(TokenType::AND, 1, OperationCode::AND);
		AddBinaryOPToken(TokenType::OR, 1, OperationCode::OR);

		AddBinaryOPToken(TokenType::SET, 0, OperationCode::SET);
	}

	void AddBinaryOPToken(TokenType token, int priority, OperationCode op)
	{
		TokenTypeToPriority.emplace((int)token, priority);
		TokenTypeToOperation.emplace((int)token, op);
	}

	int GetBinaryOPTokenPriority(TokenType token)
	{
		auto it = TokenTypeToPriority.find((int)token);
		if (it != TokenTypeToPriority.end())
			return it->second;
		return -1;
	}

	OperationCode GetBinaryOperationCode(TokenType token)
	{
		auto it = TokenTypeToOperation.find((int)token);
		if (it != TokenTypeToOperation.end())
			return it->second;
		return OperationCode::END;
	}
};

