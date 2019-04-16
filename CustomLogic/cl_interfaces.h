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

//===================NodeCfg接口========================
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


//===================Node接口=======================
class VariablesLib;
struct ICustomNode
{
	// 构建node
	virtual void InitializeNode(ICustomNodeCfg* cfg, VariablesLib* vlib) = 0;
	// 构建node完成立即调用awake
	virtual void Awake() = 0;
	// cl构建完成后调用start
	virtual void Start() = 0;
	// cl需要每帧更新 不实现此接口，则节点不会按帧更新
	virtual void Update(float dt) = 0;
	// node可以重新执行，需要reset一下
	virtual void Reset() = 0;
	// 销毁
	virtual void Destroy() = 0;
	// 激活
	virtual void Activate() = 0;
	// 禁用
	virtual void Deactivate() = 0;
	// 是否激活
	virtual bool IsActive() = 0;
	// 条件是否达成 原ICondition接口
	virtual bool IsConditionReached() = 0;
	// cl需要检查是否执行结束 不实现此接口，则节点在当前帧就会结束
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