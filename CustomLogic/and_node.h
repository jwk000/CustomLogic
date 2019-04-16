#pragma once
#include "custom_node.h"


class AndCfg :public CustomNodeCfg
{
public:
	std::string NodeType() { return "And"; }

	static bool CheckXmlFormat(pugi::xml_node node)
	{
		//ConditionAndNode:<And> ConditionNode+ </And>
		return XmlCfgChecker("And").AllChildrenCondition().Check(node);
	}

};

//////////////////////////////////////////////////////////////////////////
// ������ϣ� �����߼� �루AND��
//////////////////////////////////////////////////////////////////////////
class And :public CustomNode
{
public:

	bool IsConditionReached()override
	{
		for (auto node : m_customNodes)
		{
			if (!node->IsConditionReached())
			{
				return false;
			}
		}
		return true;
	}

};

