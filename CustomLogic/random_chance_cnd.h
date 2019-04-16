#pragma once
#include "custom_node.h"


//静态配置
class RandomChanceCndCfg :public CustomNodeCfg
{
public:
	float ProbPercent;   //百分比概率

	std::string NodeType() { return "RandomChanceCnd"; }

	static bool CheckXmlFormat(pugi::xml_node node)
	{
		return XmlCfgChecker("Condition").HasAttr("ProbPercent").Check(node);
	}

	bool EndParseXml(pugi::xml_node cndNode)
	{
		ProbPercent = cndNode.attribute("ProbPercent").as_float();
		return true;
	}
};

//////////////////////////////////////////////////////////////////////////
// 随机概率条件
//////////////////////////////////////////////////////////////////////////
class RandomChanceCnd :public ACustomNode<RandomChanceCndCfg>
{
	int mRandNum;

public:

	void Awake()
	{
		base::Awake();
		m_varLibRef->ReadVar("RandomNumber", mRandNum);
	}

	bool IsConditionReached()
	{
		return mRandNum < m_cfg->ProbPercent;
	}
};

