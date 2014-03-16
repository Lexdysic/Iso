
namespace Map
{

struct Cell :
    public ICell
{
    void                        SetTileSetDef (const Content::TileSetDef * tileSetDef) override;
    const Content::TileSetDef * GetTileSetDef () const override;
    float32                     GetHeight () override;
    void                        SetHeight (float32 height) override;
    void                        Rotate () override;

    // Data
    Point2s   pos;
    uint32       tileIndex;
    TileSet * tileSet;
    float32   height;
};

} // namespace Map