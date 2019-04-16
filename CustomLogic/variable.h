#pragma once
#include <vector>
#include <string>

enum VariableType
{
	Int,
	IntList,
	Long,
	LongList,
	Float,
	FloatList,
	Double,
	DoubleList,
	String,
	UserData,
	VTCount
};

//自定义数据类型从这里继承
struct IVariableUserData
{
	virtual ~IVariableUserData() {}
};

template<typename T> struct ttidof;
template<> struct ttidof<int> { enum { value = VariableType::Int }; };
template<> struct ttidof<std::vector<int>> { enum { value = VariableType::IntList }; };
template<> struct ttidof<long> { enum { value = VariableType::Long }; };
template<> struct ttidof<std::vector<long>> { enum { value = VariableType::LongList }; };
template<> struct ttidof<float> { enum { value = VariableType::Float }; };
template<> struct ttidof<std::vector<float>> { enum { value = VariableType::FloatList }; };
template<> struct ttidof<double> { enum { value = VariableType::Double }; };
template<> struct ttidof<std::vector<double>> { enum { value = VariableType::DoubleList }; };
template<> struct ttidof<std::string> { enum { value = VariableType::String }; };
template<> struct ttidof<IVariableUserData*> { enum { value = VariableType::UserData }; };


class Variable
{
public:
	std::vector<std::string> m_variable;

	VariableType m_varType = VariableType::Double;

	void Construct(const std::vector<std::string>& raw_variable)
	{
		int count = (int)raw_variable.size();
		for (int i = 0; i < count; ++i)
		{
			auto str = raw_variable[i];
			m_variable.push_back(raw_variable[i]);
		}
	}

	void Reset()
	{
		m_variable.clear();
		m_varType = VariableType::Double;
	}

	std::string operator[](int index)
	{
		if (index >= 0 && index < m_variable.size())
			return m_variable[index];
		else
			return "";
	}

	int MaxIndex()
	{
		return (int)m_variable.size() - 1;
	}
};
