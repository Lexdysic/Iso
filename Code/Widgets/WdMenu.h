namespace Widgets
{

class CCellMenu :
    public UserInterface::CWidget,
    public ICellMenuTileNotify
{
public:
    CCellMenu (UserInterface::CWidget * parent, const Point2s & cellPos);
    ~CCellMenu ();

private:
    void OnWidgetUpdate () override;
    void OnWidgetRender (const Point2 & pos, const Vector2 & size) override;
    void OnWidgetLayout (const Point2 & pos, const Vector2 & size) override;
    void OnWidgetGetNativeSize (Vector2 * outSize) override;
    void OnWidgetMouseDown (EMouse button, const Point2 & pos) override;
    void OnWidgetMouseWheel (sint ticks, const Point2 & pos) override;

private: // ICellMenuTileNotify
    void OnCellMenuTileClicked (CCellMenuTile * tile) override;

private:
    Point2s m_cellPos;
    TArray<CCellMenuTile *> m_tiles;
    float32 m_offset;
};

} // namespace Widgets