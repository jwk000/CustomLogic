#pragma once
#include "custom_node.h"

struct NotCfg :public CustomNodeCfg
{
public:
	std::string NodeType() { return "Not"; }

	static bool CheckXmlFormat(pugi::xml_node node)
	{
		//ConditionNotNode:<Not> ConditionNode </Not>
		return XmlCfgChecker("Not").HasNoAttr().OnlyOneChild().AllChildrenCondition().Check(node);
	}

};

//////////////////////////////////////////////////////////////////////////
// ���������߼� �ǣ�NOT��
//////////////////////////////////////////////////////////////////////////
class Not :public CustomNode
{
public:
	bool IsConditionReached()override
	{
		return !m_customNodes.front()->IsConditionReached();
	}

};

