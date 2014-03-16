


//=============================================================================
//
// IApp
//
//=============================================================================
interface IApp
{
    virtual ~IApp () {}

    virtual void Render () {}
    virtual void Update () {}

    virtual void OnWindowKeyDown (EKey key) {}
    virtual void OnWindowMouseDown (EMouse button, const Point2s & pos) {}
};



//=============================================================================
//
// CApplication
//
//=============================================================================
class CApplication :
    System::CWindowNotify
{
public:
    CApplication ();
    ~CApplication ();

    void Go ();

private: // System::IWindowNotify

    void OnWindowKeyDown (EKey key) override;
    void OnWindowMouseDown (EMouse button, const Point2s & pos) override;

private:
    // Data
    System::IWindow * m_window;
    CFrameTimer       m_frameTimer;
    IApp *            m_app;

    // Helpers
    void Render ();
    void Update ();
};