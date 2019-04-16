#pragma once
#include "custom_node.h"


//��̬����
class RandomChanceCndCfg :public CustomNodeCfg
{
public:
	float ProbPercent;   //�ٷֱȸ���

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
// �����������
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

