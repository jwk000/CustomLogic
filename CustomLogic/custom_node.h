#pragma once
#include "cl_factory.h"
#include "variables_lib.h"
#include <bitset>
#include <functional>

class XmlCfgChecker
{
	enum eCheckFlags
	{
		NO_ATTRIBUTE,
		NO_CHILD,
		ONLY_ONE_CHILD,
		ALL_CHILD_CONDITIONS ,
		ALL_CHILD_BEHAVIORS,
		ALL_CHILD_CONTROLLERS,
		ALL_CHILD_NAME,
		ALL,
		_FLAG_COUNT_
	};
	std::string m_name;
	std::set<std::string> m_has_attrs;
	std::set<std::string> m_has_children;
	std::set<std::string> m_any_child;

	std::bitset<_FLAG_COUNT_> m_flags;
	//bool(XmlCfgChecker::*m_child_check)(pugi::xml_node);
	std::function<bool(pugi::xml_node)> m_child_check;
public:
	XmlCfgChecker(const char* name) :m_name(name)
	{

	}

	XmlCfgChecker& HasAttr(const char* attr)
	{
		m_has_attrs.insert(attr);
		return *this;
	}

	XmlCfgChecker& HasNoAttr()
	{
		m_flags.set(NO_ATTRIBUTE);
		return *this;
	}

	XmlCfgChecker& HasChild(const char* node)
	{
		m_has_children.emplace(node);
		return *this;
	}

	XmlCfgChecker& HasAnyChild(const char* child)
	{
		m_any_child.insert(child);
		return *this;
	}

	XmlCfgChecker& HasAnyChild(const char* child1, const char* child2)
	{
		m_any_child.insert(child1);
		m_any_child.insert(child2);
		return *this;
	}

	XmlCfgChecker& HasNoChild()
	{
		m_flags.set(NO_CHILD);
		return *this;
	}

	XmlCfgChecker& OnlyOneChild()
	{
		m_flags.set(ONLY_ONE_CHILD);
		return *this;
	}


	XmlCfgChecker& AllChildrenCondition()
	{
		m_flags.set(ALL_CHILD_CONDITIONS);
		m_flags.set(ALL);
		m_child_check = std::bind(&XmlCfgChecker::IsConditionNode, this, std::placeholders::_1);
		return *this;
	}

	XmlCfgChecker& AllChildrenBehavior()
	{
		m_flags.set(ALL_CHILD_BEHAVIORS);
		m_flags.set(ALL);
		m_child_check = std::bind(&XmlCfgChecker::IsBehaviorNode, this, std::placeholders::_1);
		return *this;
	}

	XmlCfgChecker& AllChildrenControl()
	{
		m_flags.set(ALL_CHILD_CONTROLLERS);
		m_flags.set(ALL);
		m_child_check = std::bind(&XmlCfgChecker::IsControlNode, this, std::placeholders::_1);
		return *this;
	}

	XmlCfgChecker& AllChildrenName(const char* name)
	{
		m_flags.set(ALL_CHILD_NAME);
		m_flags.set(ALL);
		m_child_check = std::bind(&XmlCfgChecker::IsNodeName, this,std::placeholders::_1, name);
		return *this;
	}

	bool Check(pugi::xml_node node)
	{
		//����
		if (node.name() != m_name)
		{
			return false;
		}
		if (m_flags.test(NO_ATTRIBUTE) && node.first_attribute())
		{
			LogError("CustomLogic ID=", GetCustomLogicID(node), " Node [", m_name, "] cannot has attributes");
			return false;
		}

		//��������
		for (auto& attr : m_has_attrs)
		{
			if (!node.attribute(attr.c_str()))
			{
				LogError("CustomLogic ID=", GetCustomLogicID(node), " Node [", m_name, "] must have attribute [", attr,"]");
				return false;
			}
		}
		//����ڵ�
		for (auto& child : m_has_children)
		{
			if (!node.child(child.c_str()))
			{
				LogError("CustomLogic ID=", GetCustomLogicID(node), " Node [", m_name, "] must have child [",child,"]");
				return false;
			}
		}
		//�ڵ���
		int cnt = 0;
		for (auto& child : node.children())
		{
			cnt++;
			if (m_flags.test(ONLY_ONE_CHILD) && cnt > 1)
			{
				LogError("CustomLogic ID=", GetCustomLogicID(node), " Node [", m_name, "] has more than one child");
				return false;
			}
			if (m_flags.test(ALL) && m_child_check && !m_child_check(child))
			{
				if (m_flags.test(ALL_CHILD_BEHAVIORS)) 
				{
					LogError("CustomLogic ID=", GetCustomLogicID(node), " Node [", m_name, "] all children must be behaviors");
				}
				else if (m_flags.test(ALL_CHILD_CONDITIONS))
				{
					LogError("CustomLogic ID=", GetCustomLogicID(node), " Node [", m_name, "] all children must be conditions");
				}
				else if (m_flags.test(ALL_CHILD_CONTROLLERS))
				{
					LogError("CustomLogic ID=", GetCustomLogicID(node), " Node [", m_name, "] all children must be controllers");
				}
				return false;
			}
		}
		return true;
	}
protected:
	int GetCustomLogicID(pugi::xml_node node)
	{
		return node.root().attribute("ID").as_int();
	}

	bool IsConditionNode(pugi::xml_node node)
	{
		static const char* conditions[] = { "Condition","And","Or","Not" };
		return IsNodeNameIn(node, conditions);
	}

	bool IsControlNode(pugi::xml_node node)
	{
		static const char* controllers[] = { "Sequence", "Parellel", "If", "Switch", "Foreach","Template","Reference" };
		return IsNodeNameIn(node, controllers);
	}

	bool IsBehaviorNode(pugi::xml_node node)
	{
		static const char* behaviors[] = { "Behavior" };
		return IsNodeNameIn(node, behaviors) || IsControlNode(node);
	}

	bool IsNodeName(pugi::xml_node node, const char* name)
	{
		return node.name() == name;
	}

	template<typename C> bool IsNodeNameIn(pugi::xml_node node, const C& names)
	{
		auto it = std::find_if(std::begin(names), std::end(names), [&](auto e) 
		{ 
			return strcmp(node.name(), e) == 0;
		});
		return it != std::end(names);
	}

};

// ����config�Ļ���
class CustomNodeCfg :public ICustomNodeCfg
{
protected:
	using base = CustomNodeCfg;

	std::vector<ICustomNodeCfg*> ChildrenCfg;
public:
	virtual std::vector<ICustomNodeCfg*>& Children() override
	{
		return ChildrenCfg;
	}

	virtual std::string NodeType() override
	{
		return "CustomNode";
	}

	//�ӽڵ������ɺ�����������
	virtual bool EndParseXml(pugi::xml_node node) override
	{
		return true;
	}

	//Ĭ�ϵݹ�����ӽڵ�
	virtual bool ParseFromXml(pugi::xml_node node) override
	{
		ChildrenCfg.clear();
		for (pugi::xml_node subNode : node.children())
		{
			ICustomNodeCfg* cndCfg = CustomLogicFactory::CreateNodeCfg(subNode);
			ChildrenCfg.push_back(cndCfg);
		}

		return EndParseXml(node);
	}

	//���xml��ʽ�Ϸ�����CustomLogic����ʱ�ݹ���һ��
	static bool CheckXmlFormat(pugi::xml_node node)
	{
		return true;
	}

	bool IsCondition()
	{
		static const char* conditions[] = { "Condition","And","Or","Not" };
		return IsNodeNameIn(conditions);
	}

	bool IsController()
	{
		static const char* controllers[] = { "Sequence", "Parellel", "If", "Switch", "Foreach","Template","Reference" };
		return IsNodeNameIn(controllers);
	}

	bool IsBehavior()
	{
		static const char* behaviors[] = { "Behavior" };
		return IsNodeNameIn(behaviors) || IsController();
	}

	bool IsNodeName(const char* name)
	{
		return NodeType() == name;
	}

	template<typename C> bool IsNodeNameIn(const C& names)
	{
		auto it = std::find_if(std::begin(names), std::end(names), [&](auto e) { return this->NodeType() == e; });
		return it != std::end(names);
	}

};

//����node�Ļ���
template<typename CFG>
class ACustomNode :public ICustomNode
{
protected:
	using base = ACustomNode<CFG>;

	CFG* m_cfg;
	VariablesLib* m_varLibRef;
	bool m_isActive = false;
	std::vector<ICustomNode*> m_customNodes;

public:
	VariablesLib* BlackBoard() { return m_varLibRef; }

	//�ռ�ĳ�ֽӿڣ��ڸ��ڵ�ͳһ����
	template<typename CT> void CollectInterface(CT& interfaceList)
	{
		using T = typename CT::value_type;

		//û���ӽڵ�˵����Ҷ�ӽڵ㣬�ռ�this�Ľӿ�
		if (m_customNodes.empty())
		{
			T notify = dynamic_cast<T>(this);
			if (notify)
			{
				interfaceList.push_back(notify);
			}
		}

		//���ӽڵ�Ķ��ǿ��ƽڵ㣬�����ռ�this�Ľӿ�
		for (auto node : m_customNodes)
		{
			T notify = dynamic_cast<T>(node);
			if (notify)
			{
				interfaceList.push_back(notify);
			}

			node->CollectInterface(interfaceList);
		}
	}

	virtual void AddCustomNode(ICustomNode* node)
	{
		m_customNodes.emplace_back(node);
	}


	// �����ù���ڵ㣬�ӽڵ㹹����ɵ���awake
	virtual void InitializeNode(ICustomNodeCfg* cfg, VariablesLib* vlib)
	{
		static_assert(std::is_base_of<ICustomNodeCfg, CFG>::value, "node cfg type is not ICustomNodeCfg");

		m_varLibRef = vlib;
		m_cfg = dynamic_cast<CFG*>(cfg);

		if (!cfg->Children().empty())
		{
			for (auto sub : cfg->Children())
			{
				auto node = CustomLogicFactory::CreateCustomNode(sub, vlib);
				m_customNodes.emplace_back(node);
			}
		}
		Awake();
	}

	//�Ƿ񼤻�
	virtual bool IsActive()
	{
		return m_isActive;
	}

	//�����Լ�
	virtual void Activate()
	{
		m_isActive = true;
	}

	//�����Լ�
	virtual void Deactivate()
	{
		m_isActive = false;
	}

	// node����������,�����ӽڵ㹹����ɺ����awake
	virtual void Awake()
	{
		Activate();
	}

	// cl������ɺ����start
	virtual void Start()
	{
		for (auto node : m_customNodes)
		{
			if (node->IsActive())
			{
				node->Start();
			}
		}

	}

	// cl��Ҫÿ֡���� ��ʵ�ִ˽ӿڣ���ڵ㲻�ᰴ֡����
	virtual void Update(float dt)
	{
		for (auto node : m_customNodes)
		{
			if (node->IsActive())
			{
				node->Update(dt);
			}
		}

	}

	virtual void Reset()
	{
		for (auto node : m_customNodes)
		{
			node->Reset();
		}
	}

	// cl������ݹ��������нڵ�
	virtual void Destroy()
	{
		for (auto node : m_customNodes)
		{
			node->Destroy();
			delete node;
		}
	}

	//Ĭ����Ϊ��ֹͣ���ж��ӽڵ���ֹ
	virtual bool CanStop()
	{
		for (auto node : m_customNodes)
		{
			if (!node->CanStop())
			{
				return false;
			}
		}
		return true;
	}

	//Ĭ��������
	virtual bool IsConditionReached()
	{
		return true;
	}

	virtual bool IsCondition()
	{
		return m_cfg->IsCondition();
	}
	virtual bool IsController()
	{
		return m_cfg->IsController();
	}
	virtual bool IsBehavior()
	{
		return m_cfg->IsBehavior();
	}
	virtual bool IsNodeName(const char* name)
	{
		return m_cfg->IsNodeName(name);
	}
};

class CustomNode :public ACustomNode<CustomNodeCfg> {};