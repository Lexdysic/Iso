
namespace Map
{

struct TileSet
{
    TileSet ();
    ~TileSet ();
    const Content::TileSetDef * tileSetDef;
    Graphics::IImage * images[4];
};

} // namespace Map