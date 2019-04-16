#pragma once

#include "custom_node.h"

class LogBhvCfg :public CustomNodeCfg
{
public:
	std::string msg;
	std::string NodeType() { return "LogBhv"; }

	static bool CheckXmlFormat(pugi::xml_node node)
	{
		return XmlCfgChecker("Behavior").HasAttr("Message").HasNoChild().Check(node);
	}

	bool EndParseXml(pugi::xml_node node)
	{
		msg = node.attribute("Message").as_string();
		return true;
	}
};

class LogBhv :public ACustomNode<LogBhvCfg>
{
public :

	virtual void Update(float dt)
	{
		LogError("LogBhv: msg=", m_cfg->msg);
	}
};