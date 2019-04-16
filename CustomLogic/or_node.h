#pragma once
#include "custom_node.h"

//��̬����
class OrCfg : public CustomNodeCfg
{
public:
	std::string NodeType() { return "Or"; }
	static bool CheckXmlFormat(pugi::xml_node node)
	{
		//ConditionOrNode:<Or> ConditionNode+ </Or>
		return XmlCfgChecker("Or").HasNoAttr().AllChildrenCondition().Check(node);
	}

};

//////////////////////////////////////////////////////////////////////////
// ������ϣ� �����߼� ��OR��
//////////////////////////////////////////////////////////////////////////
class Or :public CustomNode
{
public:

	bool IsConditionReached()override
	{
		for (auto node:m_customNodes)
		{
			if (node->IsConditionReached())
			{
				return true;
			}
		}
		return false;
	}
};

