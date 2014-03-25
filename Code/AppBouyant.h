
//=============================================================================
//
// CAppBouyant
//
//=============================================================================

class CAppBouyant :
    public IApp
{
public:
    CAppBouyant ();
    ~CAppBouyant ();

public: // IApp

    void Render () override;
    void Update () override;

private:
    float ComputeDisplacement();

    Obb2 m_box;
    Circle m_ball;
    Vector2 m_ballVel;
    Vector2 m_ballAccel;
    Aabb2 m_water;
};