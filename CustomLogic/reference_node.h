#pragma once

#include "custom_node.h"
#include "customlogic_node.h"

class ReferenceCfg :public CustomNodeCfg
{
public:
	int LogicID;

	std::string NodeType() { return "Reference"; }
	static bool CheckXmlFormat(pugi::xml_node node)
	{
		//ReferenceNode:<Reference ID="id" />
		return XmlCfgChecker("Reference").HasAttr("ID").HasNoChild().Check(node);
	}

	bool EndParseXml(pugi::xml_node xmlNode)
	{
		LogicID = xmlNode.attribute("LogicID").as_int();
		return true;
	}
};

//////////////////////////////////////////////////////////////////////////
// CustomLogic引用节点， 不同于模板，它是独立完整的子逻辑
//////////////////////////////////////////////////////////////////////////
class Reference :public CustomLogic
{
public:
	void InitializeNode(ICustomNodeCfg* cfg, VariablesLib* context)override
	{
		Reference::SwitchReferenceCfg(cfg, context);
		CustomLogic::InitializeNode(cfg, context);
	}

	static void SwitchReferenceCfg( ICustomNodeCfg*& cfg, VariablesLib*& context)
	{
		auto theCfg = dynamic_cast<ReferenceCfg*>(cfg);
		CustomLogicCfg* newCfg = CustomLogicFactory::GetCustomLogicCfg(theCfg->LogicID);
		if (newCfg == nullptr)
		{
			LogError("ReferenceNode Cant Find Config : id = ", theCfg->LogicID);
		}

		cfg = newCfg;
		context = new VariablesLib();
	}


};

