#pragma once
#include "cl_factory.h"
#include "custom_node.h"

class ParallelCfg : public CustomNodeCfg
{
public:

	std::string NodeType()
	{
		return "Parallel";
	}

	static bool CheckXmlFormat(pugi::xml_node node)
	{
		//ParellelNode: <Parellel> BehaviorNode+ </Parellel>
		return XmlCfgChecker("Parellel").HasNoAttr().AllChildrenBehavior().Check(node);
	}

};

// ����ִ�� ��Ϊ���װ
class Parallel : public ACustomNode<ParallelCfg>
{
public:

	virtual void Awake() override
	{
		Reset();
	}

	void Activate() override
	{
		base::Activate();
		for (auto node : m_customNodes)
		{
			node->Activate();
		}
	}

	void Deactivate()override
	{
		base::Deactivate();
		for (auto node : m_customNodes)
		{
			node->Deactivate();
		}
	}

	void Update(float dt)
	{
		for (auto node : m_customNodes)
		{
			if (node->IsActive()) 
			{
				node->Update(dt);
				if (node->CanStop())
				{
					node->Deactivate();
				}
			}
		}
	}

};

