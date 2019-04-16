#include "expression_compiler.h"
#include "cl_interfaces.h"

bool ExpressionCompiler::Compile(std::string expression_string)
{
	m_error_occurred = false;
	m_tokenizer.Construct(expression_string);
	m_token = nullptr;
	m_token_type = TokenType::ERROR;
	GetToken();
	ParseExpression();
	m_token = nullptr;
	m_token_type = TokenType::ERROR;
	m_raw_variable.clear();
	return !m_error_occurred;
}

void ExpressionCompiler::GetToken()
{
	m_token = m_tokenizer.GetNextToken();
	m_token_type = m_token->Type();
	if (m_token_type == TokenType::ERROR)
		m_error_occurred = true;
}

void ExpressionCompiler::ParseExpression()
{
	ParseBinaryOperator(OperationSetting::MinBinaryPriority);
}

void ExpressionCompiler::ParseBinaryOperator(int symbolPriority)
{
	ParseHighPriorityBinary(symbolPriority);

	while (m_op_setting.GetBinaryOPTokenPriority(m_token_type) == symbolPriority)
	{
		OperationCode op_code = m_op_setting.GetBinaryOperationCode(m_token_type);
		GetToken();

		ParseHighPriorityBinary(symbolPriority);

		AppendOperation(op_code);
	}
}

void ExpressionCompiler::ParseHighPriorityBinary(int symbolPriority)
{
	if (OperationSetting::isMaxBinaryPriority(symbolPriority))
	{
		OperationCode unary_op = OperationCode::END;
		if (m_token_type == TokenType::MINUS)
		{
			unary_op = OperationCode::NEGATE;
			GetToken();
		}
		else if (m_token_type == TokenType::NOT)
		{
			unary_op = OperationCode::NOT;
			GetToken();
		}

		ParseFactor();

		if (unary_op != OperationCode::END)
			AppendOperation(unary_op);
	}
	else
	{
		ParseBinaryOperator(symbolPriority + 1);
	}
}

void ExpressionCompiler::ParseFactor()
{
	switch (m_token_type)
	{
	case TokenType::NUMBER:
		AppendNumber(m_token->GetNumber());
		GetToken();
		break;
	case TokenType::LEFT_PAREN:
		GetToken();
		ParseExpression();
		if (m_token_type != TokenType::RIGHT_PAREN)
		{
			m_error_occurred = true;
			LogError("Expression: ParseFactor(), ')' expected");
			return;
		}
		GetToken();
		break;
	case TokenType::IDENTIFIER:
		m_raw_variable.clear();
		m_raw_variable.push_back(m_token->GetRawString());
		GetToken();
		////左值被赋值时可以指定类型  eg: float:Atk = BaseAtk * MultFactor;
		//if (m_token_type == TokenType::COLON)
		//{
		//	GetToken();
		//	if (m_token_type != TokenType::IDENTIFIER)
		//	{
		//		m_error_occurred = true;
		//		LogError("Expression: ParseFactor(), COLON TokenType::Identifier expected");
		//		return;
		//	}
		//	m_raw_variable.push_back(m_token->GetRawString());
		//	GetToken();
		//	SetVariable(m_raw_variable);
		//	m_raw_variable.clear();
		//	break;
		//}
		////支持 Attacker.Attr.atk 这类的调用
		//while (m_token_type == TokenType::PERIOD)
		//{
		//	GetToken();
		//	if (m_token_type != TokenType::IDENTIFIER)
		//	{
		//		m_error_occurred = true;
		//		LogError("Expression: ParseFactor(), TokenType::Identifier expected");
		//		return;
		//	}
		//	m_raw_variable.push_back(m_token->GetRawString());
		//	GetToken();
		//}
		if (m_token_type == TokenType::SET)
		{
			SetVariable(m_raw_variable);
		}
		else
		{
			AppendVariable(m_raw_variable);
		}
		m_raw_variable.clear();
		break;
	case TokenType::SINE:
		ParseBuildInFunction(OperationCode::SIN, 1);
		break;
	case TokenType::COSINE:
		ParseBuildInFunction(OperationCode::COS, 1);
		break;
	case TokenType::TANGENT:
		ParseBuildInFunction(OperationCode::TAN, 1);
		break;
	case TokenType::SQUARE_ROOT:
		ParseBuildInFunction(OperationCode::SQRT, 1);
		break;
	case TokenType::MINIMUM:
		ParseBuildInFunction(OperationCode::MIN, 2);
		break;
	case TokenType::MAXIMUM:
		ParseBuildInFunction(OperationCode::MAX, 2);
		break;
	case TokenType::CLAMP:
		ParseBuildInFunction(OperationCode::CLAMP, 3);
		break;
	default:
		break;
	}
}

void ExpressionCompiler::ParseBuildInFunction(OperationCode opcode, int param_count)
{
	GetToken();
	if (m_token_type != TokenType::LEFT_PAREN)
		LogError("Expression: ParseBuildInFunction, '(' expected");
		if (param_count > 0)
		{
			GetToken();
			ParseExpression();
			--param_count;
			while (param_count > 0)
			{
				if (m_token_type != TokenType::COMMA)
				{
					m_error_occurred = true;
					LogError("Expression: ParseBuildInFunction, ',' expected");
					return;
				}
				GetToken();
				ParseExpression();
				--param_count;
			}
		}
	if (m_token_type != TokenType::RIGHT_PAREN)
	{
		m_error_occurred = true;
		LogError("Expression: ParseBuildInFunction, ')' expected");
		return;
	}
	AppendOperation(opcode);
	GetToken();
}
