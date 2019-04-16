#include "expression.h"
#include <algorithm>

void Expression::Reset()
{
	m_constant = 0;
	if (m_program != nullptr)
	{
		delete (m_program);
		m_program = nullptr;
	}
}

std::vector<Variable>* Expression::GetAllVariables()
{
	if (m_program != nullptr)
		return &m_program->GetAllVariables();
	else
		return nullptr;
}

bool Expression::Compile(std::string expression)
{
	ExpressionCompiler* program = new ExpressionCompiler();
	if (!program->Compile(expression))
		return false;

	m_program = program;
	if (program->IsConstant())
	{
		m_constant = Evaluate(nullptr);
		delete m_program;
		m_program = nullptr;
	}

	return true;
}

#pragma push_macro("min")
#pragma push_macro("max")

double Expression::Evaluate(VariablesLib* variable_env)
{
	if (m_program == nullptr)
		return m_constant;

	auto instructions = m_program->Instructions();
	auto variables = m_program->Variables();

	std::stack<double> stack;
	double var1, var2, var3;
	size_t index = 0;
	size_t total_count = instructions.size();
	while (index < total_count)
	{
		OperationCode op_code = (OperationCode)(long)instructions[index];
		++index;
		switch (op_code)
		{
		case OperationCode::PUSH_NUMBER:
			stack.push((double)(instructions[index]));
			++index;
			break;
		case OperationCode::PUSH_VARIABLE:
			if (variable_env != nullptr)
			{
				auto& v = variables[(long)instructions[index]];
				variable_env->ReadVar(v[0], var1);
				stack.push(var1);
			}
			else
			{
				stack.push(0);
			}
			++index;
			break;
		case OperationCode::SET_VARIABLE:
			stack.push((long)instructions[index]);
			++index;
			break;
		case OperationCode::NEGATE:
			var1 = stack.top(); stack.pop();
			stack.push(-var1);
			break;
		case OperationCode::NOT:
			var1 = stack.top(); stack.pop();
			if (var1 == 0)
				stack.push(1);
			else
				stack.push(0);
			break;
		case OperationCode::SET:
		{
			var2 = stack.top(); stack.pop();
			var1 = stack.top(); stack.pop();
			Variable dslVar = variables[(long)var1];
			//if (dslVar.m_varType == VariableType::Int)
			//	variable_env->SetVariable(dslVar, (int)var2);
			//else if (dslVar.m_varType == VariableType::Long)
			//	variable_env->SetVariable(dslVar, (long)var2);
			//else if (dslVar.m_varType == VariableType::Float)
			//	variable_env->SetVariable(dslVar, (float)var2);
			//else if (dslVar.m_varType == VariableType::Double)
				variable_env->SetVariable(dslVar, var2);
			stack.push(var2);
		}
			break;
		case OperationCode::ADD:
			var2 = stack.top(); stack.pop();
			var1 = stack.top(); stack.pop();
			stack.push(var1 + var2);
			break;
		case OperationCode::SUBTRACT:
			var2 = stack.top(); stack.pop();
			var1 = stack.top(); stack.pop();
			stack.push(var1 - var2);
			break;
		case OperationCode::MULTIPLY:
			var2 = stack.top(); stack.pop();
			var1 = stack.top(); stack.pop();
			stack.push(var1 * var2);
			break;
		case OperationCode::GREATER_THAN:
			var2 = stack.top(); stack.pop();
			var1 = stack.top(); stack.pop();
			if (var1 > var2)
				stack.push(1);
			else
				stack.push(0);
			break;
		case OperationCode::LESS_THAN:
			var2 = stack.top(); stack.pop();
			var1 = stack.top(); stack.pop();
			if (var1 < var2)
				stack.push(1);
			else
				stack.push(0);
			break;
		case OperationCode::GREATER_EQUAL:
			var2 = stack.top(); stack.pop();
			var1 = stack.top(); stack.pop();
			if (var1 >= var2)
				stack.push(1);
			else
				stack.push(0);
			break;
		case OperationCode::LESS_EQUAL:
			var2 = stack.top(); stack.pop();
			var1 = stack.top(); stack.pop();
			if (var1 <= var2)
				stack.push(1);
			else
				stack.push(0);
			break;
		case OperationCode::EQUAL:
			var2 = stack.top(); stack.pop();
			var1 = stack.top(); stack.pop();
			if (var1 == var2)
				stack.push(1);
			else
				stack.push(0);
			break;
		case OperationCode::NOT_EQUAL:
			var2 = stack.top(); stack.pop();
			var1 = stack.top(); stack.pop();
			if (var1 != var2)
				stack.push(1);
			else
				stack.push(0);
			break;
		case OperationCode::AND:
			var2 = stack.top(); stack.pop();
			var1 = stack.top(); stack.pop();
			if (var1 == 0 || var2 == 0)
				stack.push(0);
			else
				stack.push(1);
			break;
		case OperationCode::OR:
			var2 = stack.top(); stack.pop();
			var1 = stack.top(); stack.pop();
			if (var1 != 0 && var2 != 0)
				stack.push(1);
			else
				stack.push(0);
			break;
		case OperationCode::DIVIDE:
			var2 = stack.top(); stack.pop();
			var1 = stack.top(); stack.pop();
			stack.push(var1 / var2);
			break;
		case OperationCode::SIN:
			var1 = stack.top(); stack.pop();
			stack.push(std::sin(var1));
			break;
		case OperationCode::COS:
			var1 = stack.top(); stack.pop();
			stack.push(std::cos(var1));
			break;
		case OperationCode::TAN:
			var1 = stack.top(); stack.pop();
			stack.push(std::tan(var1));
			break;
		case OperationCode::SQRT:
			var1 = stack.top(); stack.pop();
			stack.push(std::sqrt(var1));
			break;
		case OperationCode::MIN:
			var2 = stack.top(); stack.pop();
			var1 = stack.top(); stack.pop();
			stack.push(std::min(var1, var2));
			break;
		case OperationCode::MAX:
			var2 = stack.top(); stack.pop();
			var1 = stack.top(); stack.pop();
			stack.push(std::max(var1, var2));
			break;
		case OperationCode::CLAMP:
			var3 = stack.top(); stack.pop();
			var2 = stack.top(); stack.pop();
			var1 = stack.top(); stack.pop();
			stack.push(std::clamp(var1, var2, var3));
			break;
		default:
			break;
		}
	}

	if (!stack.empty())
	{
		double ret = stack.top();
		stack.pop();
		return ret;
	}
	else
		return 0.0;
}

#pragma pop_macro("min")
#pragma pop_macro("max")
