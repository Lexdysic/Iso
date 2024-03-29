
namespace Graphics
{
    interface IBackbuffer;
}


//=============================================================================
//
// CResourceInputComponent
//
//=============================================================================

class CResourceInputComponent :
    public CComponent,
    public IRefCounted<CResourceInputComponent>
{
    REFCOUNTED_ADAPTER(CResourceInputComponent)
public:

    typedef TStrongPtr<CResourceInputComponent> Ptr;
    typedef TWeakPtr<CResourceInputComponent> Ref;

    void AddTerminal (Token name);
    Point2 GetPosition (Token name);

public: // CComponent

    ComponentType GetType () const override { return TYPE; }

public:

    static const ComponentType TYPE;
    static CResourceInputComponent * Attach (IEntity * entity);

private:
    TArray<Token> m_terminals;
};



//=============================================================================
//
// CResourceOutputComponent
//
//=============================================================================

class CResourceOutputComponent :
    public CComponent,
    public IRefCounted<CResourceOutputComponent>
{
    REFCOUNTED_ADAPTER(CResourceOutputComponent)
public:

    typedef TStrongPtr<CResourceOutputComponent> Ptr;
    typedef TWeakPtr<CResourceOutputComponent> Ref;
    
    void AddTerminal (Token name);
    Point2 GetPosition (Token name);

public: // CComponent

    ComponentType GetType () const override { return TYPE; }

public:

    static const ComponentType TYPE;
    static CResourceOutputComponent * Attach (IEntity * entity);

private:
    TArray<Token> m_terminals;
};



//=============================================================================
//
// 
//
//=============================================================================

class CResourceLineComponent :
    public Graphics::CRenderComponent,
    public IRefCounted<CResourceLineComponent>
{
    REFCOUNTED_ADAPTER(CResourceLineComponent)
public:

    CResourceLineComponent (const Color & color);

    void Connect (
        CResourceOutputComponent * from = null,
        Token                      fromName = Token::Null,
        CResourceInputComponent *  to = null,
        Token                      toName = Token::Null
    );

    typedef TStrongPtr<CResourceLineComponent> Ptr;
    typedef TWeakPtr<CResourceLineComponent> Ref;

public: // CRenderComponent

    void Render (Graphics::IRenderTarget *) override;
    ComponentType GetType () const override { return TYPE; }

public:

    static const ComponentType TYPE;
    static CResourceLineComponent * Attach(IEntity * entity, const Color & color);

private:

	Color							m_color;
    CResourceOutputComponent::Ref   m_from;
    Token                           m_fromName;
    CResourceInputComponent::Ref    m_to;
    Token                           m_toName;
};



//=============================================================================
//
// 
//
//=============================================================================

class CResourceRenderComponent :
    public CComponent
{

};




//=============================================================================
//
// 
//
//=============================================================================

class CAppPowerGraph :
    public IApp
{
public:

    CAppPowerGraph ();
    ~CAppPowerGraph ();

public: // IApp

    void OnWindowKeyDown (EKey key) override;
    void OnWindowMouseDown (EMouse button, const Point2s & pos) override;

    void Render () override;
    void Update () override;

private:
    IEntity * m_factory;
    Radian m_angle;
};