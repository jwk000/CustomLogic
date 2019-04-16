#pragma once
#include "custom_node.h"

class SequenceCfg :public CustomNodeCfg
{
public:
	std::string NodeType()
	{
		return "Sequence";
	}

	static bool CheckXmlFormat(pugi::xml_node node)
	{
		//SequenceNode: <Sequence> BehaviorNode+ </Sequence>
		return XmlCfgChecker("Sequence").HasNoAttr().AllChildrenBehavior().Check(node);
	}

};

//////////////////////////////////////////////////////////////////////////
// 顺序执行
//////////////////////////////////////////////////////////////////////////
class Sequence :public ACustomNode<SequenceCfg>
{
	int m_curBhvIndex = 0;
	bool m_isFinished = false;

public:
	void Awake() override
	{
		for (auto node: m_customNodes)
		{
			node->Deactivate();
		}
		Activate();
	}

	void Activate()override
	{
		base::Activate();
		ActivateCurBhv();
	}

	void Deactivate()override
	{
		base::Deactivate();
		DeactivateCurBhv();
	}



	void Update(float dt)
	{
		//while会把线程阻塞
		if (m_curBhvIndex < m_customNodes.size())
		{
			auto node = m_customNodes[m_curBhvIndex];
			node->Update(dt);
			if (node->CanStop())
			{
				DeactivateCurBhv();
				m_curBhvIndex++;
				ActivateCurBhv();
			}
			return;
		}

		m_isFinished = true;
	}

	bool CanStop()
	{
		return m_isFinished;
	}

	virtual void Reset()
	{
		m_isFinished = false;
		m_curBhvIndex = 0;
		base::Reset();
	}

	void ActivateCurBhv()
	{
		if (m_curBhvIndex >= 0 && m_curBhvIndex < m_customNodes.size())
		{
			m_customNodes[m_curBhvIndex]->Activate();
		}
	}

	void DeactivateCurBhv()
	{
		if (m_curBhvIndex >= 0 && m_curBhvIndex < m_customNodes.size())
		{
			m_customNodes[m_curBhvIndex]->Deactivate();
		}
	}
};
