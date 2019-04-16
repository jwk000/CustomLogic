#include "cl_factory.h"
#include "customlogic_node.h"

CustomLogic* CustomLogicFactory::CreateCustomLogic(int configID)
{
	CustomLogicCfg* config = GetCustomLogicCfg(configID);
	if (config == nullptr)
	{
		LogError( "CreateCustomLogic Cant Find Config : id = " , configID);
		return nullptr;
	}

	auto logicType = config->NodeType();
	if (logicType != "CustomLogic")
	{
		return nullptr;
	}

	CustomLogic* customLogic = new CustomLogic();
	VariablesLib* vlib = new VariablesLib();
	vlib->WriteVar("CustomLogicID", configID);
	customLogic->InitializeNode(config, vlib);

	return customLogic;
}

void CustomLogicFactory::DestroyCustomLogic(CustomLogic* cl)
{
	if (cl != nullptr)
	{
		cl->Destroy();
		delete cl;
	}
}

ICustomNode* CustomLogicFactory::CreateCustomNode(ICustomNodeCfg* cfg, VariablesLib* context)
{
	auto nodetype = cfg->NodeType();

	auto it = mStr2Node.find(nodetype);
	if (it != mStr2Node.end())
	{
		auto node = std::get<2>(it->second)();
		node->InitializeNode(cfg, context);
		return node;
	}
	return nullptr;
}

void CustomLogicFactory::ClearConfigMap()
{
	mCustomCfgMap.clear();
}

CustomLogicCfg* CustomLogicFactory::GetCustomLogicCfg(int id)
{
	auto it = mCustomCfgMap.find(id);
	if (it != mCustomCfgMap.end())
	{
		return it->second;
	}
	return nullptr;
}

std::map<int, CustomLogicCfg*> CustomLogicFactory::mCustomCfgMap;

std::map<std::string, 
		 std::tuple<std::function<bool(pugi::xml_node)>,
					std::function<ICustomNodeCfg*()>, 
					std::function<ICustomNode*()>>> 
	CustomLogicFactory::mStr2Node;

bool CustomLogicFactory::RegisterNode(std::string name, 
	std::function<bool(pugi::xml_node)> xml_checker,
	std::function<ICustomNodeCfg*()> cfg_factory,
	std::function<ICustomNode*()> node_factory)
{
	return mStr2Node.emplace(name, std::make_tuple(xml_checker, cfg_factory, node_factory)).second;
}


bool CustomLogicFactory::CheckXmlFormat(pugi::xml_node node)
{
	auto it = mStr2Node.find(node.name());
	if (it != mStr2Node.end())
	{
		return std::get<0>(it->second)(node);
	}

	return false;
}

bool CustomLogicFactory::ReadXml(std::string xmlPath)
{
	pugi::xml_document xml;
	pugi::xml_parse_result result = xml.load_file(xmlPath.c_str());
	if (!result)
	{
		LogError( "CustomLogicFactory  Cant Find Xml File: " , xmlPath);
		return false;
	}

	//逐个解析 各个配置
	pugi::xml_node root = xml.child("CustomLogicConfig");
	if (!root)
	{
		return false;
	}

	for (pugi::xml_node cfgNode : root.children("CustomLogic"))
	{
		CustomLogicCfg* cfg = dynamic_cast<CustomLogicCfg*>(CreateNodeCfg(cfgNode));
		if (cfg == nullptr)
		{
			continue;
		}
		if (mCustomCfgMap.find(cfg->ID) != mCustomCfgMap.end())
		{
			LogError(xmlPath , "中出现重复的CustomLogicID :" , cfg->ID);
			continue;
		}
		mCustomCfgMap[cfg->ID] = cfg;
	}
	return true;
}

bool CustomLogicFactory::ReadXmlStr(std::string xmlstr)
{
	pugi::xml_document xml;
	pugi::xml_parse_result result = xml.load_string(xmlstr.c_str());
	if (!result)
	{
		LogError("load xml string failed!");
		return false;
	}

	pugi::xml_node root = xml.child("CustomLogicConfig");
	if (!root)
	{
		return false;
	}

	//逐个解析 各个配置
	for (pugi::xml_node cfgNode : root.children("CustomLogic"))
	{
		if (!CheckXmlFormat(cfgNode))
		{
			LogError("Xml配置customlogic格式错误！");
			continue;
		}

		CustomLogicCfg* cfg = dynamic_cast<CustomLogicCfg*>(CreateNodeCfg(cfgNode));
		if (cfg == nullptr)
		{
			continue;
		}
		if (mCustomCfgMap.find(cfg->ID) != mCustomCfgMap.end())
		{
			LogError("xml中出现重复的CustomLogicCfg ID :", cfg->ID);
			continue;
		}
		mCustomCfgMap[cfg->ID] = cfg;
	}
	return true;

}

ICustomNodeCfg* CustomLogicFactory::CreateNodeCfg(pugi::xml_node node)
{
	if (!node)
	{
		LogError( "CustomLogicConfig PraseNodeCfg ParseError  cusNode == null");
		return nullptr;
	}

	std::string nodeTypeStr = node.name();
	if (nodeTypeStr == "Behavior" || nodeTypeStr == "Condition")
	{
		nodeTypeStr = node.attribute("Type").as_string();
	}
	ICustomNodeCfg* nodeCfg = CustomLogicFactory::CreateNodeCfg(nodeTypeStr);
	if (nodeCfg == nullptr)
	{
		LogError( "NodeConfigTypeRegistry.CreateNodeCfg == null  nodeTypeStr = " , nodeTypeStr);
		return nullptr;
	}

	if (!nodeCfg->ParseFromXml(node))
	{
		return nullptr;
	}

	return nodeCfg;
}

ICustomNodeCfg* CustomLogicFactory::CreateNodeCfg(std::string nodetype)
{

	auto it = mStr2Node.find(nodetype);
	if (it != mStr2Node.end())
	{
		auto cfg = std::get<1>(it->second)();
		return cfg;
	}

	return nullptr;

}
