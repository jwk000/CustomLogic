#pragma once

#include <string>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include "./pugixml/pugixml.hpp"
#include "mt19937.h"
#include <iostream>

//===================NodeCfg�ӿ�========================
struct ICustomNodeCfg
{
	virtual std::string NodeType() = 0;
	virtual std::vector<ICustomNodeCfg*>& Children() = 0;
	virtual bool ParseFromXml(pugi::xml_node node) = 0;
	virtual bool EndParseXml(pugi::xml_node node) = 0;
	virtual bool IsCondition() = 0;
	virtual bool IsController() = 0;
	virtual bool IsBehavior() = 0;
	virtual bool IsNodeName(const char* name) = 0;

};


//===================Node�ӿ�=======================
class VariablesLib;
struct ICustomNode
{
	// ����node
	virtual void InitializeNode(ICustomNodeCfg* cfg, VariablesLib* vlib) = 0;
	// ����node�����������awake
	virtual void Awake() = 0;
	// cl������ɺ����start
	virtual void Start() = 0;
	// cl��Ҫÿ֡���� ��ʵ�ִ˽ӿڣ���ڵ㲻�ᰴ֡����
	virtual void Update(float dt) = 0;
	// node��������ִ�У���Ҫresetһ��
	virtual void Reset() = 0;
	// ����
	virtual void Destroy() = 0;
	// ����
	virtual void Activate() = 0;
	// ����
	virtual void Deactivate() = 0;
	// �Ƿ񼤻�
	virtual bool IsActive() = 0;
	// �����Ƿ��� ԭICondition�ӿ�
	virtual bool IsConditionReached() = 0;
	// cl��Ҫ����Ƿ�ִ�н��� ��ʵ�ִ˽ӿڣ���ڵ��ڵ�ǰ֡�ͻ����
	virtual bool CanStop() = 0;

	virtual bool IsCondition() = 0;
	virtual bool IsController() = 0;
	virtual bool IsBehavior() = 0;
	virtual bool IsNodeName(const char* name) = 0;

};





template<typename T> void LogError(T&& x)
{
	std::cerr << x;
	std::cerr << std::endl;
}
template<typename X, typename... Args> void LogError(X&& x, Args&&... args)
{
	std::cerr << x;
	LogError(args...);
}