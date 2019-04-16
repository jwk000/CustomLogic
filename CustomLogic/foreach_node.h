#pragma once
#include "custom_node.h"

//æ≤Ã¨≈‰÷√
class ForeachCfg :public CustomNodeCfg
{
public:
	std::string WriteValue;
	std::string ReadList;
	std::string ListType;
	int BeginIndex;

	std::string NodeType()
	{
		return "Foreach";
	}

	static bool CheckXmlFormat(pugi::xml_node node)
	{
		//ForeachNode: <Foreach Element="element" List="list"> BehaviorNode </Foreach>
		return XmlCfgChecker("Foreach").HasAttr("Element").HasAttr("List").OnlyOneChild().AllChildrenBehavior().Check(node);
	}

	bool EndParseXml(pugi::xml_node bhvNode)
	{
		WriteValue = bhvNode.attribute("Element").as_string();
		ReadList = bhvNode.attribute("List").as_string();
		ListType = bhvNode.attribute("ListType").as_string();
		BeginIndex = bhvNode.attribute("BeginIndex").as_int();

		return true;
	}

};

class Foreach :public ACustomNode<ForeachCfg>
{
	int m_curListIndex = 0;
	bool m_isFinished = false;

public:
	virtual void Awake() override
	{
		Reset();
	}

	virtual void Activate()override
	{
		base::Activate();
		m_customNodes.front()->Activate();
	}

	virtual void Deactivate()override
	{
		base::Deactivate();
		m_customNodes.front()->Deactivate();
	}

	virtual void Reset()
	{
		m_curListIndex = m_cfg->BeginIndex;
		m_isFinished = false;
	}

	void Update(float dt)
	{
		if (m_isFinished) return ;

		std::vector<int> v;
		m_varLibRef->ReadVar(m_cfg->ReadList,  v);

		if (v.empty())
		{
			return;
		}

		if (m_curListIndex >= v.size())
		{
			return;
		}

		do 
		{
			auto value = v[m_curListIndex];
			m_varLibRef->WriteVar(m_cfg->WriteValue, value);
			base::Update(dt);
		} 
		while (m_curListIndex < v.size());
		m_isFinished = true;
	}

	bool CanStop()
	{
		return m_isFinished;
	}
};
