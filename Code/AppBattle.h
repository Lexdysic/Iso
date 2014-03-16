

//=============================================================================
//
// CTankControlComponent
//
//=============================================================================

class CTankControlComponent :
    public CComponent,
    public CEntityNotify
{
public:

    CTankControlComponent ();
    ~CTankControlComponent ();

public: // Links

    LIST_LINK(CTankControlComponent) m_link;

public: // CComponent

    ComponentType GetType () const override { return TYPE; }

public:

    static const ComponentType TYPE;
    static CTankControlComponent * Attach (IEntity * entity);

private: // CEntityNotify

    void OnEntityUpdate () override;

};


//=============================================================================
//
// CGunControlComponent
//
//=============================================================================

class CGunControlComponent :
    public CComponent,
    public CInputNotify,
    public CEntityNotify
{
public:

    CGunControlComponent ();
    ~CGunControlComponent ();

public: // Links

    LIST_LINK(CGunControlComponent) m_link;

public: // CComponent

    ComponentType GetType () const override { return TYPE; }

public:

    static const ComponentType TYPE;
    static CGunControlComponent * Attach (IEntity * entity);

private: // CInputNotify

    void OnInputKeyDown (EKey key) override;
    void OnInputKeyUp (EKey key) override;
    void OnInputGamepadDown (EButton button) override;
    //void OnInputGamepadAxisChanged(EGameAxis axis)

private: // CEntityNotify

    void OnEntityUpdate () override;

private: // 

    Time::Point m_shotStart;
};



//=============================================================================
//
// CAppBattle
//
//=============================================================================

class CAppBattle :
    public IApp
{
public:
    CAppBattle ();
    ~CAppBattle ();

public: // IApp

    void Render () override;
    void Update () override;

private:

    Graphics::IImage *          m_ground;
    Graphics::IImage *          m_mask;
    Graphics::IRenderTarget *   m_renderTarget;

    IEntity *   m_tank;
    IEntity *   m_shot;
};