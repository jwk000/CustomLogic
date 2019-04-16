#pragma once
#include "custom_node.h"
#include "dsl.h"

class DelaySecondsCfg :public ICustomNodeCfg
{
public:
	Expression timeLen;
	float timeLenNumerical = 0;
	bool canSkip = true;
	bool isDslTime = false;

	std::string NodeType()
	{
		return "DelaySeconds";
	}

	bool EndParseXml(pugi::xml_node bhvNode)
	{
		std::string expressionTypeStr = bhvNode.attribute("ExpressionType").as_string();

		// 根据ExpressionType解析延时时间
		std::string timeStr = bhvNode.attribute("Time").as_string();
		if (expressionTypeStr == "DSL")
		{
			timeLen.Compile(timeStr);
			isDslTime = true;
		}
		else
		{
			timeLenNumerical = std::stof(timeStr);
		}

		canSkip = bhvNode.attribute("CanSkip").as_bool(true);
		return true;
	}
};

class DelaySeconds :public  ACustomNode<DelaySecondsCfg>
{
	bool m_finish = false;
	float m_time = 0;

public:
	void Start()override
	{
		m_time = ResolveTime();

		if (m_time < 0.0001)
		{
			m_finish = true;
		}
	}

	void Reset()override
	{
		base::Reset();
		m_time = ResolveTime();
		m_finish = false;
	}

	void Update(float dt)override
	{
		m_time -= dt;

		if (m_time >= 0)
		{
			return;
		}
		m_finish = true;
		return;
	}

	bool CanStop()
	{
		return m_finish;
	}

	void OnSkip()
	{
		if (m_cfg->canSkip)
			m_finish = true;
	}

	float ResolveTime()
	{
		float time;
		if (m_cfg->isDslTime)
			time = (float)m_cfg->timeLen.Evaluate(m_varLibRef);
		else
			time = m_cfg->timeLenNumerical;
		return time;
	}
};
