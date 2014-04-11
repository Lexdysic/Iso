
//=============================================================================
//
// CAppMachi
//
//=============================================================================

class CAppMachi :
    public IApp
{
public:
    CAppMachi ();
    ~CAppMachi ();

public: // IApp

    void Render () override;
    void Update () override;

private:

    IEntity * m_entity;
};