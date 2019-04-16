# CustomLogic �淶

�淶���������֣����ù淶��ִ�й淶��

## CustomLogic�����﷨�淶��

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

## CustomLogicִ�й淶��

```cpp
struct ICustomNode
{
    // cl���������д���node��������awake
    virtual void Awake() = 0;
    // cl������ɺ����start
    virtual void Start() = 0;
    // cl��Ҫÿ֡���� ��ʵ�ִ˽ӿڣ���ڵ㲻�ᰴ֡����
    virtual void Update(float dt) = 0;
    // node��������ִ�У���Ҫresetһ��
    virtual void Reset() = 0;
    // ����
    virtual void Destroy() = 0;
    // ����
    virtual void Activate() = 0;
    // ����
    virtual void Deactivate() = 0;
    // �Ƿ񼤻�
    virtual bool IsActive() = 0;
};
 
struct ICondition :virtual public ICustomNode
{
    //�����Ƿ���
    virtual bool IsConditionReached() = 0;
};
 
struct IBehavior :virtual public ICustomNode
{
    // cl��Ҫ����Ƿ�ִ�н��� ��ʵ�ִ˽ӿڣ���ڵ��ڵ�ǰ֡�ͻ����
    virtual bool CanStop() = 0;
};
 
struct IController : public IBehavior
{
 
};
```

������
CustomLogic ֻ��ͨ��CustomLogicFactory������CustomNode����������InitializeNode()���֮����������Awake()��

������
CustomLogic����һ�������ڰ壬�����ӽڵ㹲������ڰ壬CustomNode֮������ݽ���ֻ��ͨ���ڰ壬CustomNode�����������ݽ���Ҳֻ��ͨ���ڰ壻

ִ�У�
CustomLogic�������֮�������ݹ����Start();Ȼ�����Update() �C CanStop() ѭ����CanStop()��ʱ������CustomLogic����CustomLogicFactory���٣�



����淶������������ȷ��

CustomLogic��ʲô����һ��xml��ʽ�ı�����ԡ�
CustomLogic���ŵ���ʲô���ӽ������﷨������ʽ���������׽�����
CustomLogic��ȱ����ʲô���ͽű����Ա���д�����ӣ�����û���������ơ������ڵ���Ҫ�ֶ����������������ԵĶ���ͷ���������lua���Զ�ע�������ˡ�
CustomLogic�ʺ���ʲô��С��ģʹ�õģ��򵥵������߼���û��Ҫ����ű����Ե�ʱ�����һ�¡�
CustomLogic���ʺ���ʲô�����ģ�ġ����ӵ��߼���������һ��ϵͳ��