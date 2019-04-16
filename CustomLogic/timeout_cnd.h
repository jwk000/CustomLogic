#pragma once
#include "custom_node.h"

//��̬����
class TimeOutCndCfg :public CustomNodeCfg
{
public:
	std::string NodeType() { return "TimeOutCnd"; }

	float TimeLimit;  //��ʱ���ã��룩

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
// ʱ����������ʱ
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

