namespace Widgets
{

class CText :
    public UserInterface::CWidget
{
public:
    CText (UserInterface::CWidget * parent);
    ~CText ();

    void SetText (const wchar text[]);

private: // UserInterface::CWidget

    void OnWidgetUpdate () override;
    void OnWidgetRender (const Point2 & pos, const Vector2 & size) override;
    void OnWidgetGetNativeSize (Vector2 * outSize) override;

private:
    std::wstring     m_text;
};

} // namespace Widgets