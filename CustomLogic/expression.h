#pragma once
#include "token.h"
#include "expression_compiler.h"
#include <stack>
#include "variables_lib.h"
#include <assert.h>

 class Expression
{
	double m_constant = 0;
	ExpressionCompiler* m_program;

public:
	void Reset();

	// void CopyFrom(Expression& rhs)
	//{
	//	m_constant = rhs.m_constant;
	//	if (rhs.m_program != nullptr)
	//	{
	//		m_program = new ExpressionCompiler();
	//		m_program->CopyFrom(rhs.m_program);
	//	}
	//}

	std::vector<Variable>* GetAllVariables();

	bool Compile(std::string expression);

	double Evaluate(VariablesLib* variable_env);

	bool IsConst()
	{
		return m_program == nullptr;
	}

};

 namespace std {
	 template<class T>
	 constexpr const T& clamp(const T& v, const T& lo, const T& hi)
	 {
		 return assert(!(hi < lo)),
			 (v < lo) ? lo : (hi < v) ? hi : v;
	 }
}