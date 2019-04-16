#pragma once
#include "custom_node.h"
#include "template_node.h"

class CustomLogicCfg : public CustomNodeCfg
{
public:
	int ID = 0;

	virtual std::string NodeType()
	{
		return "CustomLogic";
	}
	
	static bool CheckXmlFormat(pugi::xml_node node)
	{
		//customlogic : <CustomLogic ID="id" Tips="tips"> ControlNode </CustomLogic>
		return XmlCfgChecker("CustomLogic").HasAttr("ID").HasAttr("Tips").OnlyOneChild().AllChildrenControl().Check(node);
	}

	virtual bool EndParseXml(pugi::xml_node node)
	{
		ID = node.attribute("ID").as_int();
		return true;
	}
};

class CustomLogic : public ACustomNode<CustomLogicCfg>
{
public:

	virtual void InitializeNode(ICustomNodeCfg* cfg, VariablesLib* vlib)override
	{
		base::InitializeNode(cfg, vlib);
		std::set<int> usedTempLogicSet;
		_InitializeNodes(m_cfg, vlib, usedTempLogicSet);
	}

	void _InitializeNodes(CustomLogicCfg* logicCfg, VariablesLib* vlib, std::set<int>& usedTemplate)
	{
		//װ��ڵ�
		for (auto nodeCfg : logicCfg->Children())
		{
			//////////////////////////// ����ģ������ Begin ////////////////////////////
			TempleteCfg* templeteCfg = dynamic_cast<TempleteCfg*>(nodeCfg);
			if (templeteCfg != nullptr)
			{
				int templeteID = templeteCfg->ID;
				if (!usedTemplate.emplace(templeteID).second)
				{
					LogError("ERROR: ѭ������CustomLogicģ��!  CustomLogicID=", vlib->__UnsafeReadVar<long>("CustomLogicID"), ", templeteID=", templeteID);
					continue;
				}

				CustomLogicCfg* logiccfg = CustomLogicFactory::GetCustomLogicCfg(templeteID);
				if (logiccfg != nullptr)
				{
					//����ģ��CustomLogic�����õĸ����ڵ�
					_InitializeNodes(logiccfg, vlib, usedTemplate);
				}
				else
				{
					LogError("ERROR: CustomLogicģ���Ҳ��� CustomLogicID=", vlib->__UnsafeReadVar<long>("CustomLogicID"), ", templeteID=", templeteID);
				}
				continue;
			}
			////////////////////////////// ����ģ������ End ////////////////////////////

			ICustomNode* theNode = CustomLogicFactory::CreateCustomNode(nodeCfg, vlib);
			AddCustomNode(theNode);
		}
	}


};

