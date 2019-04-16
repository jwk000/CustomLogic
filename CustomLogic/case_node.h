#pragma once
#include "custom_node.h"

class CaseCfg : public CustomNodeCfg
{
public:
	std::string Value;

	std::string NodeType()
	{
		return "Case";
	}

	static bool CheckXmlFormat(pugi::xml_node node)
	{
		//CaseNode:<Case Value="x"> BehaviorNode </Case>
		return XmlCfgChecker("Case").HasAttr("Value").OnlyOneChild().AllChildrenBehavior().Check(node);
	}

	bool EndParseXml(pugi::xml_node node)
	{
		Value = node.attribute("Value").as_string();
		return true;
	}
};

class Case :public ACustomNode<CaseCfg>
{
	CustomNode* m_subNode;

public:

	virtual void Awake() override
	{
		Deactivate();
	}

	virtual void Reset() override
	{
		base::Reset();
		Deactivate();
	}

	virtual bool CanStop() override
	{
		//如果没处于激活状态 则可以停止
		if (IsActive() == false)
		{
			return true;
		}

		//所有子节点都可以停止，则可以停止
		return base::CanStop();
	}

	std::string GetValue()
	{
		return m_cfg->Value;
	}
};
