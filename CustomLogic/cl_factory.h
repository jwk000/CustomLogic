#pragma once
#include "cl_interfaces.h"
#include <tuple>
#include <functional>

class CustomNodeCfg;
class CustomNode;
class CustomLogic;
class CustomLogicCfg;
class VariablesLib;
class CustomLogicFactory
{
protected:
	static std::map<int, CustomLogicCfg*> mCustomCfgMap;
	static std::map<std::string,
		std::tuple<std::function<bool(pugi::xml_node)>,
		std::function<ICustomNodeCfg*()>,
		std::function<ICustomNode*()>>> mStr2Node;
public:
	//customlogic
	static bool ReadXml(std::string xmlPath);
	static bool ReadXmlStr(std::string xml);
	static CustomLogicCfg* GetCustomLogicCfg(int id);
	static void ClearConfigMap();
	static CustomLogic* CreateCustomLogic(int configID);
	static void DestroyCustomLogic(CustomLogic* cl);

	//customnode
	static bool RegisterNode(std::string name, 
		std::function<bool(pugi::xml_node)> xml_checker, 
		std::function<ICustomNodeCfg*()> cfg_factory, 
		std::function<ICustomNode*()> node_factory);
	static bool CheckXmlFormat(pugi::xml_node node);
	static ICustomNodeCfg* CreateNodeCfg(pugi::xml_node node);
	static ICustomNodeCfg* CreateNodeCfg(std::string nodetype);
	static ICustomNode* CreateCustomNode(ICustomNodeCfg* cfg, VariablesLib* context);

};
#define REGNODE(N) if(!CustomLogicFactory::RegisterNode(#N, N##Cfg::CheckXmlFormat, [](){return new N##Cfg;}, [](){return new N;})) return false; 
