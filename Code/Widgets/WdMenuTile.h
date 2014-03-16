namespace Widgets
{

interface ICellMenuTileNotify;
class CCellMenuTile;


class CCellMenuTile :
    public UserInterface::CWidget
{
public:
    CCellMenuTile (
        UserInterface::CWidget * parent,
        const Content::TileSetDef * tileSet,
        ICellMenuTileNotify * notify
    );
    ~CCellMenuTile ();

    const Content::TileSetDef * GetTileSetDef () const { return m_tileset; }

private: // UserInterface::CWidget
    void OnWidgetUpdate () override;
    void OnWidgetRender (const Point2 & pos, const Vector2 & size) override;
    void OnWidgetGetNativeSize (Vector2 * outSize) override;
    void OnWidgetMouseDown (EMouse button, const Point2 & pos) override;
    void OnWidgetMouseEnter () override;
    void OnWidgetMouseExit () override;

private:
    const Content::TileSetDef * m_tileset;
    Graphics::IImage *          m_image;
    bool                        m_hasFocus;
    ICellMenuTileNotify *       m_notify;
};


interface ICellMenuTileNotify
{
    virtual void OnCellMenuTileClicked (CCellMenuTile * tile);
};

} // namespace Widgets