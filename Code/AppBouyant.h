
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

    IEntity * m_entity;

    Circle m_ball;
    Aabb2 m_water;
};