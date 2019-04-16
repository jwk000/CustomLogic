#pragma once
#include "variable.h"
#include "cl_interfaces.h"
#include <array>


struct IVariables
{
	virtual ~IVariables() {}
	virtual void Clear() = 0;
	
};

//TODO ø…“‘”√variantÃÊªª
template<typename T> class VariablesImp :public IVariables
{
	std::map<std::string, T> mVarDic;
public:
	int TypeID() { return ttidof<T>::value; }
	void WriteVar(std::string ID,const T& value)
	{
		mVarDic[ID] = value;
	}

	bool ReadVar(std::string ID, T& getV)
	{
		auto it = mVarDic.find(ID);
		if (mVarDic.end() != it)
		{
			getV = it->second;
			return true;
		}
		return false;
	}

	bool HasVar(std::string ID)
	{
		return mVarDic.end() != mVarDic.find(ID);
	}

	virtual void Clear()
	{
		mVarDic.clear();
	}
};


class VariablesLib
{
	std::array<IVariables*, VariableType::VTCount> m_vars; 
public:
	VariablesLib()
	{
		m_vars.fill(0);
	}

	template<typename T> VariablesImp<T>* GetVariables()
	{
		return dynamic_cast<VariablesImp<T>*>(m_vars[ttidof<T>::value]);
	}

	template<typename T> bool ReadVar(std::string key, T& value)
	{
		auto variables = GetVariables<T>();
		if (variables != nullptr && variables->ReadVar(key, value))
		{
			return true;
		}
		LogError("ReadVar<T> is null " , ", key=" + key);
		return false;
	}

	template<typename T> T __UnsafeReadVar(const std::string& key)
	{
		T v;
		ReadVar(key, v);
		return v;
	}

	template<typename T> void WriteVar(std::string key, const T& value)
	{
		auto variables = GetVariables<T>();
		if (variables == nullptr)
		{
			variables = new VariablesImp<T>();
			m_vars[ttidof<T>::value] = variables;
		}
		variables->WriteVar(key, value);
	}

	template<typename T> bool HasVar(std::string key)
	{
		auto varType = GetVariables<T>();
		if (varType != nullptr)
		{
			return varType->HasVar(key);
		}
		return false;
	}

	void Clear()
	{
		for (auto v : m_vars)
		{
			v->Clear();
		}
	}

	template<typename T> bool SetVariable(Variable& variable, const T& value)
	{
		if (variable.MaxIndex() == 0)
		{
			WriteVar<T>(variable[0], value);
			return true;
		}
		return false;
	}
};
