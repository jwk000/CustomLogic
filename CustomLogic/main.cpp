#include <algorithm>
#include <chrono>
#include<thread>

#include "dsl.h"
#include "template_node.h"
#include "sequence_node.h"
#include "parallel_node.h"
#include "expression_cnd.h"
#include "random_chance_cnd.h"
#include "timeout_cnd.h"
#include "case_node.h"
#include "foreach_node.h"
#include "if_node.h"
#include "true_node.h"
#include "false_node.h"
#include "and_node.h"
#include "not_node.h"
#include "or_node.h"
#include "parallel_node.h"
#include "switch_node.h"
#include "reference_node.h"
#include "customlogic_node.h"
#include "log_bhv.h"


bool RegisterNodes()
{
	REGNODE(CustomLogic);
	REGNODE(Templete);
	REGNODE(Reference);
	REGNODE(If);
	REGNODE(And);
	REGNODE(Not);
	REGNODE(Or);
	REGNODE(True);
	REGNODE(False);
	REGNODE(Parallel);
	REGNODE(Sequence);
	REGNODE(Switch);
	REGNODE(Case);
	REGNODE(Foreach);
	REGNODE(RandomChanceCnd);
	REGNODE(TimeOutCnd);
	REGNODE(ExpressionCnd);
	REGNODE(LogBhv);
	return true;
}

#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

TEST_CASE("dsl expression", "[expression]")
{
	VariablesLib vlib;
	Expression exp;
	SECTION("1+1==2")
	{
		REQUIRE(exp.Compile("1+1"));
		REQUIRE(exp.Evaluate(nullptr)==2);
	}

	SECTION("1+2*(3-4)/-1 == 3")
	{
		REQUIRE(exp.Compile("1+2*(3-4)/-1"));
		REQUIRE(exp.Evaluate(nullptr) == 3);
	}

	SECTION("1+1<6/2")
	{
		REQUIRE(exp.Compile("1+1<6/2"));
		REQUIRE(exp.Evaluate(nullptr));
	}

	SECTION("x=1+1") {
		REQUIRE(exp.Compile("x=1+1"));
		exp.Evaluate(&vlib);
		double x = 0;
		vlib.ReadVar("x", x);
		REQUIRE(x == 2);
	}


	SECTION("a-1<=b+1")
	{
		vlib.WriteVar<double>("a", 100);
		vlib.WriteVar<double>("b", 200);
		REQUIRE(exp.Compile("a-1<=a+1"));
		REQUIRE(exp.Evaluate(&vlib));
	}
}

TEST_CASE("dsl code", "[dslcode]")
{
	VariablesLib vlib;
	DSLCode dsl;
	vlib.WriteVar("a", 1.2);//dsl里直接取值

	dsl.Compile("b=2/1.0;c=a*10+b*3.3");
	dsl.Execute(&vlib);
	double a, b, c;
	vlib.ReadVar("a", a);
	vlib.ReadVar("b", b);
	vlib.ReadVar("c", c);
	REQUIRE(a == 1.2);
	REQUIRE(b == 2.0);
	REQUIRE(c+0.0001 - 18.6<0.001);
}


TEST_CASE("custom logic", "[cl]")
{
	REQUIRE(RegisterNodes());

	auto xml = R"(
		<CustomLogicConfig>
		  <CustomLogic ID="100" Tips="test">
		  <Sequence>
			  <If>
				<And>
					<Condition Type="ExpressionCnd" Code="A=100;B=100+A*2-50;B==A+150"/>
					<Condition Type="RandomChanceCnd" ProbPercent="60.99"/>
				</And>
				<True>
					<Behavior Type="LogBhv" Message="here is a true message"/>
				</True>
				<False>
					<Behavior Type="LogBhv" Message="here is a false message"/>
				</False>
			  </If>
		  </Sequence>
		  </CustomLogic>
		</CustomLogicConfig>
)";
	SECTION("xml 2 cl")
	{
		CustomLogicFactory factory;
		REQUIRE(factory.ReadXmlStr(xml));
		
		CustomLogic* cl = factory.CreateCustomLogic(100);
		REQUIRE(cl != nullptr);
		cl->BlackBoard()->WriteVar("RandomNumber", 10);
		cl->Start();
		auto last = std::chrono::system_clock::now();
		do
		{
			auto now = std::chrono::system_clock::now();
			cl->Update(std::chrono::duration_cast<std::chrono::milliseconds>(now-last).count()/1000.0f);
			last = now;
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		} while (!cl->CanStop());

		factory.DestroyCustomLogic(cl);

	}
}