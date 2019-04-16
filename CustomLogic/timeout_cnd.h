#pragma once
#include "custom_node.h"

//静态配置
class TimeOutCndCfg :public CustomNodeCfg
{
public:
	std::string NodeType() { return "TimeOutCnd"; }

	float TimeLimit;  //定时设置（秒）

	static bool CheckXmlFormat(pugi::xml_node node)
	{
		return XmlCfgChecker("Condition").HasAttr("time").Check(node);
	}

	bool EndParseXml(pugi::xml_node cndNode)
	{
		TimeLimit = cndNode.attribute("time").as_float();
		return true;
	}

};

//////////////////////////////////////////////////////////////////////////
// 时间条件，超时
//////////////////////////////////////////////////////////////////////////
class TimeOutCnd :public ACustomNode<TimeOutCndCfg> 
{
	float mTimeAcc = 0;

public:
	void Awake()
	{
		mTimeAcc = 0;
	}

	void Update(float dt)override
	{
		if (mTimeAcc <= m_cfg->TimeLimit)
		{
			mTimeAcc += dt;
		}
	}

	bool IsConditionReached()override
	{
		return mTimeAcc > m_cfg->TimeLimit;
	}

	void Reset()override
	{
		mTimeAcc = 0;
	}

	bool CanStop()
	{
		return IsConditionReached();
	}

};

