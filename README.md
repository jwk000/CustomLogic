# CustomLogic

一种xml形式的语法树，用于逻辑配置。

规范包括两部分：配置规范和执行规范。

## CustomLogic配置语法规范：

```bnf
grammar : customlogic
customlogic : <CustomLogic ID="id" Tips="tips"> ControlNode </CustomLogic>
 
ControlNode: SequenceNode
            | ParellelNode
            | IfNode
            | SwitchNode
            | ForeachNode
            | TemplateNode
            | ReferenceNode
 
BehaviorNode:  <Behavior Type="name" Param="param"* />
            | ControlNode
 
SequenceNode: <Sequence> BehaviorNode+ </Sequence>
 
ParellelNode: <Parellel> BehaviorNode+ </Parellel>
 
IfNode: <If> ConditionNode TrueNode? FalseNode? </If>
 
SwitchNode: <Switch Check="expression"> CaseNode+ </Switch>
 
ForeachNode: <Foreach Element="element" List="list"> BehaviorNode+ </Foreach>
 
ConditionNode: <Condition Type="name" Param="param"* />
                    | ConditionAndNode
                    | ConditionOrNode
                    | ConditionNotNode
 
ConditionAndNode:<And> ConditionNode+ </And>
 
ConditionOrNode:<Or> ConditionNode+ </Or>
 
ConditionNotNode:<Not> ConditionNode </Not>
 
TrueNode:<True> BehaviorNode </True>
 
FalseNode:<False> BehaviorNode </False>
 
CaseNode:<Case Value="x"> BehaviorNode </Case>
 
TemplateNode:<Template ID="id" />
 
ReferenceNode:<Reference ID="id" />
```

## CustomLogic执行规范：

```cpp
struct ICustomNode
{
    // cl构建过程中创建node立即调用awake
    virtual void Awake() = 0;
    // cl构建完成后调用start
    virtual void Start() = 0;
    // cl需要每帧更新 不实现此接口，则节点不会按帧更新
    virtual void Update(float dt) = 0;
    // node可以重新执行，需要reset一下
    virtual void Reset() = 0;
    // 销毁
    virtual void Destroy() = 0;
    // 激活
    virtual void Activate() = 0;
    // 禁用
    virtual void Deactivate() = 0;
    // 是否激活
    virtual bool IsActive() = 0;
};
 
struct ICondition :virtual public ICustomNode
{
    //条件是否达成
    virtual bool IsConditionReached() = 0;
};
 
struct IBehavior :virtual public ICustomNode
{
    // cl需要检查是否执行结束 不实现此接口，则节点在当前帧就会结束
    virtual bool CanStop() = 0;
};
 
struct IController : public IBehavior
{
 
};
```

构建：
CustomLogic 只能通过CustomLogicFactory创建；CustomNode创建并调用InitializeNode()完成之后立即调用Awake()；

交互：
CustomLogic包含一个独立黑板，所有子节点共享这个黑板，CustomNode之间的数据交互只能通过黑板，CustomNode和宿主的数据交互也只能通过黑板；

执行：
CustomLogic构建完成之后立即递归调用Start();然后进入Update() – CanStop() 循环；CanStop()的时候整个CustomLogic交给CustomLogicFactory销毁；



这个规范的意义在于明确了

CustomLogic是什么：是一种xml形式的编程语言。
CustomLogic的优点是什么：接近抽象语法树的形式，所以容易解析。
CustomLogic的缺点是什么：和脚本语言比书写更复杂，而且没有性能优势。新增节点需要手动解析、绑定宿主语言的对象和方法，比起lua的自动注入弱爆了。
CustomLogic适合做什么：小规模使用的，简单的语义逻辑，没必要引入脚本语言的时候代替一下。
CustomLogic不适合做什么：大规模的、复杂的逻辑，比如做一个系统。