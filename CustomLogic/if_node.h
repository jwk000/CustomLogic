#pragma once
#include "custom_node.h"


//静态配置
class IfCfg :public CustomNodeCfg
{
public:
	enum ECheckMode
	{
		Once,
		Always
	};

	ECheckMode CheckMode = ECheckMode::Always;

public:
	std::string NodeType()
	{
		return "If";
	}

	static bool CheckXmlFormat(pugi::xml_node node)
	{
		//IfNode: <If> ConditionNode TrueNode? FalseNode? </If>
		return XmlCfgChecker("If").HasNoAttr().HasChild("Condition").HasAnyChild("True", "False").Check(node);
	}

	bool EndParseXml(pugi::xml_node xmlNode)
	{
		//更新模式，Once是只在第一帧检查，Always是每帧都检查
		std::string sCheckMode = xmlNode.attribute("CheckMode").as_string();

		if (sCheckMode == "Always")
		{
			CheckMode = ECheckMode::Always;
		}
		else if (sCheckMode == "Once")
		{
			CheckMode = ECheckMode::Once;
		}

		return true;
	}

};

//////////////////////////////////////////////////////////////////////////
//  If节点：条件 + true行为 + false行为  true和false行为为可选项，至少有一个
//////////////////////////////////////////////////////////////////////////
class If :public ACustomNode<IfCfg>
{
public:
	ICustomNode* m_condition = nullptr;  //激活条件
	ICustomNode* m_trueBehavior = nullptr;    //为真时候执行的行为
	ICustomNode* m_falseBehavior = nullptr;   //为假时候执行的行为
	bool m_isConditionReached = false;
	bool m_hasFirstCheck = false;

public:

	virtual void Awake() override
	{
		for (auto node : m_customNodes)
		{
			if (node->IsCondition()) m_condition = node;
			if (node->IsNodeName("True"))m_trueBehavior = node;
			if (node->IsNodeName("False"))m_falseBehavior = node;
		}
		if (m_trueBehavior) m_trueBehavior->Deactivate();
		if (m_falseBehavior) m_falseBehavior->Deactivate();

	}


	void Activate()override
	{
		base::Activate();

		m_condition->Activate();

		if (m_condition->IsConditionReached())
		{
			if (m_trueBehavior != nullptr)
				m_trueBehavior->Activate();
		}
		else
		{
			if (m_falseBehavior != nullptr)
				m_falseBehavior->Activate();
		}
	}

	void Deactivate()override
	{
		base::Deactivate();
		if (m_condition != nullptr)
			m_condition->Deactivate();
		if (m_trueBehavior != nullptr)
			m_trueBehavior->Deactivate();
		if (m_falseBehavior != nullptr)
			m_falseBehavior->Deactivate();
	}

	//////////////////////////////////////////////////////////////////////////
	//INeedStopCheck
	bool CanStop()
	{
		// 1. 条件检查是否能被停止
		if (!m_condition->CanStop())
		{
			return false;
		}

		// 2. 条件达成后，行为是否能被停止
		if (m_isConditionReached)
		{
			if (m_trueBehavior && !m_trueBehavior->CanStop())
			{
				return false;
			}
		}
		// 3. 条件未达成时，行为是否能被停止
		else
		{
			if (m_falseBehavior && !m_falseBehavior->CanStop())
			{
				return false;
			}
		}
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	//IBehavior
	virtual void Reset()
	{
		if (m_trueBehavior)
			m_trueBehavior->Reset();
		if (m_falseBehavior)
			m_falseBehavior->Reset();
		m_hasFirstCheck = false;
	}

	//////////////////////////////////////////////////////////////////////////
	// INeedUpdate
	virtual void Update(float dt)
	{
		//如果只检查一次条件
		if (m_cfg->CheckMode == IfCfg::ECheckMode::Once)
		{
			//如果没检查过，才检查
			if (!m_hasFirstCheck)
			{
				UpdateContion(dt);
				//记录检查结果
				m_isConditionReached = m_condition->IsConditionReached();

				ChangeActiveState(m_isConditionReached);

				m_hasFirstCheck = true;
			}
		}
		//如果是不停检查条件的模式
		else
		{
			UpdateContion(dt);
			//检查结果
			bool isConditionTrue = m_condition->IsConditionReached();
			//当结果与之前的状态不一致时，重置Active状态
			if (m_isConditionReached ^ isConditionTrue)
			{
				m_isConditionReached = isConditionTrue;
				ChangeActiveState(m_isConditionReached);
			}
		}

		UpdateBhv(m_isConditionReached, dt);

	}

	void ChangeActiveState(bool isTrue)
	{
		if (isTrue)
		{
			if (m_trueBehavior)
				m_trueBehavior->Activate();
			if (m_falseBehavior)
				m_falseBehavior->Deactivate();
		}
		else
		{
			if (m_trueBehavior)
				m_trueBehavior->Deactivate();
			if (m_falseBehavior)
				m_falseBehavior->Activate();
		}
	}

	void UpdateBhv(bool isTrue, float dt)
	{
		if (isTrue)
		{
			UpdateTrueBhv(dt);
		}
		else
		{
			UpdateFalseBhv(dt);
		}
	}

	void UpdateContion(float dt)
	{
		{
			m_condition->Update(dt);
		}
	}

	void UpdateTrueBhv(float dt)
	{
		if (m_trueBehavior)
			m_trueBehavior->Update(dt);
	}

	void UpdateFalseBhv(float dt)
	{
		if (m_falseBehavior)
			m_falseBehavior->Update(dt);
	}
};
