#pragma once
#include "custom_node.h"

class FalseCfg :public CustomNodeCfg
{
public:
	std::string NodeType()
	{
		return "False";
	}

	static bool CheckXmlFormat(pugi::xml_node node)
	{
		//FalseNode:<False> BehaviorNode </False>
		return XmlCfgChecker("False").HasNoAttr().OnlyOneChild().AllChildrenBehavior().Check(node);
	}

};

class False :public ACustomNode<FalseCfg>
{

};