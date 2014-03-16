namespace Widgets
{

class CCellMenu;

class CRoot :
    public UserInterface::CWidget
{
public:
    CRoot ();
    ~CRoot ();

private:

    void OnWidgetLayout (const Point2 & pos, const Vector2 & size) override;
    void OnWidgetMouseDown (EMouse button, const Point2 & pos) override;
    void OnWidgetMouseUp (EMouse button, const Point2 & pos) override;
    void OnWidgetMouseMove (const Point2 & pos) override;
    void OnWidgetMouseWheel (sint ticks, const Point2 & pos) override;

private:

    bool        m_isMouseDown;
    bool        m_isMouseDrag;
    Point2      m_dragStart;
    Point2      m_dragLast;
    CCellMenu * m_menu;
};




class CNode :
    public UserInterface::CWidget
{
public:
    CNode (UserInterface::CWidget * parent);
    ~CNode ();

private:
    void OnWidgetUpdate () override;
    void OnWidgetRender (const Point2 & pos, const Vector2 & size) override;
    void OnWidgetGetNativeSize (Vector2 * outSize) override;
    void OnWidgetMouseDown (EMouse button, const Point2 & pos) override;
    void OnWidgetMouseUp (EMouse button, const Point2 & pos) override;
    void OnWidgetMouseMove (const Point2 & pos) override;

private:
    bool m_isMouseDown;
    Vector2 m_dragOffset;
};

} // namespace Widgets