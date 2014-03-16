


class CAppIso :
    public IApp
{
public:
    CAppIso ();
    ~CAppIso ();

public: // IApp

    void OnWindowKeyDown (EKey key) override;
    void OnWindowMouseDown (EMouse button, const Point2s & pos) override;

    void Render () override;
    void Update () override;

private:
    Widgets::CRoot * m_root;
};