#pragma once
#include "custom_node.h"
#include "case_node.h"

class SwitchCfg :public CustomNodeCfg
{
public:
	std::string ValueType;
	std::string ReadValue;

	std::string NodeType()
	{
		return "Switch";
	}

	static bool CheckXmlFormat(pugi::xml_node node)
	{
		//SwitchNode: <Switch Check="expression"> CaseNode+ </Switch>
		return XmlCfgChecker("Switch").HasAttr("Check").AllChildrenName("Case").Check(node);
	}

	bool EndParseXml(pugi::xml_node xmlNode)
	{
		ReadValue = xmlNode.attribute("Check").as_string();
		ValueType = xmlNode.attribute("ValueType").as_string();
		return true;
	}


};

//静态Switch运行，不支持runtime改变case的情况
class Switch :public ACustomNode<SwitchCfg>
{
	Case* m_matchCase = nullptr;
public:
	virtual void Awake() override
	{
		for (auto node : m_customNodes)
		{
			node->Deactivate();
		}
	}

	//switch节点的Activate，应该意味着啥呢
	void Activate() override
	{
		base::Activate();
		if (m_matchCase != nullptr)
		{
			m_matchCase->Activate();
		}
	}

	void Deactivate()override
	{
		base::Deactivate();
		if (m_matchCase != nullptr)
		{
			m_matchCase->Deactivate();
		}
	}

	bool CanStop()
	{
		if (m_matchCase != nullptr && !m_matchCase->CanStop())
		{
			return false;
		}
		return true;
	}

	virtual void Reset()
	{
		if (m_matchCase != nullptr)
		{
			m_matchCase->Reset();
		}
		Start();
	}

	virtual void Start()
	{
		m_matchCase = GetMatchedCase();
		m_matchCase->Activate();
	}
	virtual void Update(float dt)
	{
		if (m_matchCase != nullptr)
		{
			m_matchCase->Update(dt);
		}
	}

	Case* GetMatchedCase()
	{
		int value = 0;
		if (m_varLibRef->ReadVar(m_cfg->ReadValue, value) == false)
		{
			return nullptr;
		}

		for (auto switchCaseNode : m_customNodes)
		{
			auto caseNode = dynamic_cast<Case*>(switchCaseNode);
			auto caseValue = caseNode->GetValue();
			if (m_cfg->ValueType == "Int")
			{
				if (value == std::stoi(caseValue))
				{
					return caseNode;
				}
			}
		}
		return nullptr;
	}
};
