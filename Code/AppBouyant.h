
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

    IEntity::Ptr m_entityBox;
    IEntity::Ptr m_entityWater;
    IEntity::Ptr m_bullet;

    Circle m_ball;
    Aabb2 m_water;
    Polygon2 m_bouyantPoly;
    Point2 m_bouyantCentroid;
    Vector2 m_bouyantForce;
};