#pragma once
#include "custom_node.h"


//��̬����
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
		//����ģʽ��Once��ֻ�ڵ�һ֡��飬Always��ÿ֡�����
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
//  If�ڵ㣺���� + true��Ϊ + false��Ϊ  true��false��ΪΪ��ѡ�������һ��
//////////////////////////////////////////////////////////////////////////
class If :public ACustomNode<IfCfg>
{
public:
	ICustomNode* m_condition = nullptr;  //��������
	ICustomNode* m_trueBehavior = nullptr;    //Ϊ��ʱ��ִ�е���Ϊ
	ICustomNode* m_falseBehavior = nullptr;   //Ϊ��ʱ��ִ�е���Ϊ
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
		// 1. ��������Ƿ��ܱ�ֹͣ
		if (!m_condition->CanStop())
		{
			return false;
		}

		// 2. ������ɺ���Ϊ�Ƿ��ܱ�ֹͣ
		if (m_isConditionReached)
		{
			if (m_trueBehavior && !m_trueBehavior->CanStop())
			{
				return false;
			}
		}
		// 3. ����δ���ʱ����Ϊ�Ƿ��ܱ�ֹͣ
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
		//���ֻ���һ������
		if (m_cfg->CheckMode == IfCfg::ECheckMode::Once)
		{
			//���û�������ż��
			if (!m_hasFirstCheck)
			{
				UpdateContion(dt);
				//��¼�����
				m_isConditionReached = m_condition->IsConditionReached();

				ChangeActiveState(m_isConditionReached);

				m_hasFirstCheck = true;
			}
		}
		//����ǲ�ͣ���������ģʽ
		else
		{
			UpdateContion(dt);
			//�����
			bool isConditionTrue = m_condition->IsConditionReached();
			//�������֮ǰ��״̬��һ��ʱ������Active״̬
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
