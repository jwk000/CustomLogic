#pragma once
#include "expression.h"
#include "custom_node.h"


class ExpressionCndCfg :public CustomNodeCfg
{
public:
	Expression* Code;

	std::string NodeType()
	{
		return "ExpressionCnd";
	}

	static bool CheckXmlFormat(pugi::xml_node node)
	{
		//
		return XmlCfgChecker("Condition").HasAttr("Type").HasAttr("Code").HasNoChild().Check(node);
	}

	bool EndParseXml(pugi::xml_node bhvNode)
	{
		auto str = bhvNode.attribute("Code").as_string("");
		Code = new Expression();
		Code->Compile(str);
		return true;
	}
};


class ExpressionCnd :public ACustomNode<ExpressionCndCfg>
{
public :

	virtual bool IsConditionReached() override
	{
		float v = (float)m_cfg->Code->Evaluate(m_varLibRef);
		return ((int)v != 0);
	}

};

