namespace Content
{
    struct TileSetDef;
} // namespace Content

namespace Map
{
interface ICell;
interface IContext;

interface IContext
{
    virtual void Initialize () = 0;
    virtual void Uninitialize () = 0;
    
    virtual void Generate () = 0;

    virtual void Render () = 0;

    virtual Point2s Pick (const Point2 & worldPos) = 0;

    virtual void DrawOutline (const Point2s & cellPos) = 0;

    virtual ICell * GetCell (const Point2s & cellPos) = 0;

    virtual bool CellIsValid (const Point2s & cellPos) = 0;
    virtual void CellRotate (const Point2s & cellPos) = 0;
    virtual void CellSetTileSet (const Point2s & cellPos, const Content::TileSetDef * tileSetDef) = 0;
    virtual void CellSetHeight (const Point2s & cellPos, float32 height) = 0;
};

IContext * GetContext ();


interface ICell
{
    virtual void                        SetTileSetDef (const Content::TileSetDef * tileSetDef) = 0;
    virtual const Content::TileSetDef * GetTileSetDef () const = 0;
    virtual float32                     GetHeight () = 0;
    virtual void                        SetHeight (float32 height) = 0;

    virtual void Rotate () = 0;
};

} // namespace Map