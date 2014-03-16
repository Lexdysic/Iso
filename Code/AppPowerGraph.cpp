
#include "AppPch.h"

//=============================================================================
//
// Constants
//
//=============================================================================

const Vector2 NODE_SIZE(100.0f, 75.0f);



//=============================================================================
//
// 
//
//=============================================================================

//=============================================================================
IEntity * CreateSource (const Point2 & pos)
{
    IEntity * node = EntityGetContext()->CreateEntity();

    // Transform
    {
        auto * transform = EnsureComponent<CTransformComponent2>(node);
        transform->SetPosition(pos);
    }

    // Graphics
    {
        using namespace Graphics;
        auto * primative = IPrimativeComponent::Attach(node, Vector2(100.0f, 40.0f));
    }

    // Output
    {
        auto * output = CResourceOutputComponent::Attach(node);
    }

    return node;
}

//=============================================================================
IEntity * CreateTransform (const Point2 & pos)
{
    IEntity * node = EntityGetContext()->CreateEntity();

    // Transform
    {
        auto * transform = EnsureComponent<CTransformComponent2>(node);
        transform->SetPosition(pos);
    }

    // Graphics
    {
        using namespace Graphics;
        auto * primative = EnsureComponent<IPrimativeComponent>(node, NODE_SIZE);
        primative->SetColor(Color::Black);
    }

    // Output
    {
        auto * output = EnsureComponent<CResourceOutputComponent>(node);
    }

    // Input
    {
        auto * input = EnsureComponent<CResourceInputComponent>(node);
    }

    return node;
}

//=============================================================================
IEntity * CreateLine (
	const Color &              color,
    CResourceOutputComponent * from,
    Token                      fromName,
    CResourceInputComponent *  to,
    Token                      toName
) {
    IEntity * node = EntityGetContext()->CreateEntity();

    // Graphics
    {
        using namespace Graphics;
        auto * comp = EnsureComponent<CResourceLineComponent>(node, color);
        comp->Connect(from, fromName, to, toName);
    }

    return node;
}

//=============================================================================
template <typename T>
void AddTerminal (IEntity * entity, Token name)
{
    auto * x = entity->Get<T>();
    x->AddTerminal(name);
}





//=============================================================================
//
// 
//
//=============================================================================

const ComponentType CResourceInputComponent::TYPE('R','e','s','I','n','p','u','t');

//=============================================================================
void CResourceInputComponent::AddTerminal (Token name)
{
    m_terminals.Add(name);
}

//=============================================================================
Point2 CResourceInputComponent::GetPosition (Token name)
{
    const uint index = m_terminals.Find(name);
    const uint count = m_terminals.Count();
    ASSERT(index < count);

    const float y = (index + 1) * NODE_SIZE.y / (count + 1);

    const auto * transform = GetOwner()->Get<CTransformComponent2>();
    return transform->GetPosition() - NODE_SIZE * 0.5f + Vector2(0.0f, y);
}

//=============================================================================
CResourceInputComponent * CResourceInputComponent::Attach (IEntity * entity)
{
    auto * comp = new CResourceInputComponent();

    entity->Attach(comp);

    return comp;
}



//=============================================================================
//
// 
//
//=============================================================================

const ComponentType CResourceOutputComponent::TYPE('R','e','s','O','u','t','p','u','t');


//=============================================================================
void CResourceOutputComponent::AddTerminal (Token name)
{
    m_terminals.Add(name);
}

//=============================================================================
Point2 CResourceOutputComponent::GetPosition (Token name)
{
    const uint index = m_terminals.Find(name);
    const uint count = m_terminals.Count();
    ASSERT(index < count);

    const float y = (index + 1) * NODE_SIZE.y / (count + 1);

    const auto * transform = GetOwner()->Get<CTransformComponent2>();
    return transform->GetPosition() + Vector2(NODE_SIZE.x, -NODE_SIZE.y) * 0.5f + Vector2(0.0f, y);
}

//=============================================================================
CResourceOutputComponent * CResourceOutputComponent::Attach (IEntity * entity)
{
    auto * comp = new CResourceOutputComponent();

    entity->Attach(comp);

    return comp;
}


//=============================================================================
//
// 
//
//=============================================================================

const ComponentType CResourceLineComponent::TYPE('R','e','s','L','i','n','e');

//=============================================================================
CResourceLineComponent::CResourceLineComponent(const Color & color) :
	m_color(color)
{

}

//=============================================================================
void CResourceLineComponent::Connect (
    CResourceOutputComponent * from,
    Token                      fromName,
    CResourceInputComponent *  to,
    Token                      toName
) {
    m_from     = from;
    m_fromName = fromName;
    m_to       = to;
    m_toName   = toName;
}

//=============================================================================
void CResourceLineComponent::Render (Graphics::IRenderTarget * renderTarget)
{
    if (!m_from || !m_to || m_fromName.IsNull() || m_toName.IsNull())
        return;

    renderTarget->SetWorld(Matrix23::Identity);

    const Point2 & fromPos = m_from->GetPosition(m_fromName);
    const Point2 & toPos = m_to->GetPosition(m_toName);

    CubicCurve2 curve(
        fromPos,
        Vector2(100.0f, 0.0f),
        toPos,
        Vector2(100.0f, 0.0f)
    );
    renderTarget->Draw(curve, m_color, 1.0f);
}

//=============================================================================
CResourceLineComponent * CResourceLineComponent::Attach (IEntity * entity, const Color & color)
{
    auto * comp = new CResourceLineComponent(color);

    entity->Attach(comp);

    return comp;
}



//=============================================================================
//
// CAppPowerGraph
//
//=============================================================================

//=============================================================================
CAppPowerGraph::CAppPowerGraph ()
{
    const Token POWER("Power");
    const Token ORE("Ore");

    IEntity * power = CreateSource(Point2(100, 100));
    AddTerminal<CResourceOutputComponent>(power, POWER);

    IEntity * mine = CreateSource(Point2(100, 300));
    AddTerminal<CResourceOutputComponent>(mine, ORE);

    m_factory = CreateTransform(Point2(320, 250));
    AddTerminal<CResourceInputComponent>(m_factory, ORE);
    AddTerminal<CResourceInputComponent>(m_factory, POWER);


    IEntity * rk1 = CreateLine(
        Color(0.0f, 0.5f, 1.0f),
        power->Get<CResourceOutputComponent>(),
        POWER,
        m_factory->Get<CResourceInputComponent>(),
        POWER
    );

    IEntity * rk2 = CreateLine(
        Color(1.0f, 0.5f, 0.0f),
        mine->Get<CResourceOutputComponent>(),
        ORE,
        m_factory->Get<CResourceInputComponent>(),
        ORE
    );

}

//=============================================================================
CAppPowerGraph::~CAppPowerGraph ()
{

}

//=============================================================================
void CAppPowerGraph::Update ()
{
    const Point2 CENTER(500.0f, 500.0f);
    const float32 RADIUS = 200.0f;
    const Point2 pos = CENTER + RADIUS * Vector2(Cos(m_angle), Sin(m_angle));

    m_angle = m_angle + float32(Time::GetGameDelta());

    auto * transform = m_factory->Get<CTransformComponent2>();
    transform->SetPosition(pos);

}

//=============================================================================
void CAppPowerGraph::Render ()
{

}

//=============================================================================
void CAppPowerGraph::OnWindowKeyDown (EKey key)
{

}

//=============================================================================
void CAppPowerGraph::OnWindowMouseDown (EMouse button, const Point2s & pos)
{

}