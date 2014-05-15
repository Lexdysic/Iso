
//=============================================================================
//
// CAppBouyant
//
//=============================================================================

class CAppBouyant :
    public IApp,
    public Physics::IContextNotify
{
public:
    CAppBouyant ();
    ~CAppBouyant ();

public: // IApp

    void Render () override;
    void Update () override;

public: // Physics::IContextNotify

    void OnPhysicsPreTick () override;

private:

    IEntity * m_entityBox;
    IEntity * m_entityWater;

    Circle m_ball;
    Aabb2 m_water;
    Polygon2 m_bouyantPoly;
};