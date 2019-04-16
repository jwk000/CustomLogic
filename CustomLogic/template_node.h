#pragma once
#include "custom_node.h"

//引用其他配置节点
class TempleteCfg :public CustomNodeCfg
{
public:
	int ID;

	std::string NodeType()
	{
		return "Templete";
	}

	static bool CheckXmlFormat(pugi::xml_node node)
	{
		//TemplateNode:<Template ID="id" />
		return XmlCfgChecker("Template").HasAttr("ID").HasNoChild().Check(node);
	}

	bool EndParseXml(pugi::xml_node bhvNode)
	{
		ID = bhvNode.attribute("ID").as_int();
		return true;
	}

};

class Templete :public ACustomNode<TempleteCfg>
{

};
