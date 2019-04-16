#pragma once
#include "custom_node.h"

class TrueCfg :public CustomNodeCfg
{
public:
	std::string NodeType()
	{
		return "True";
	}

	static bool CheckXmlFormat(pugi::xml_node node)
	{
		//TrueNode:<True> BehaviorNode </True>
		return XmlCfgChecker("True").HasNoAttr().OnlyOneChild().AllChildrenBehavior().Check(node);
	}

};

class True :public ACustomNode<TrueCfg>
{

};