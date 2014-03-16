
#include "MapPch.h"

namespace Map
{

//=============================================================================
void Cell::SetTileSetDef (const Content::TileSetDef * tileSetDef)
{
    tileSet = CContext::Get()->FindTileSet(tileSetDef);
    assert(tileSet);
}

//=============================================================================
const Content::TileSetDef * Cell::GetTileSetDef () const
{
    return tileSet->tileSetDef;
}

//=============================================================================
float32 Cell::GetHeight ()
{
    return height;
}

//=============================================================================
void Cell::SetHeight (float32 h)
{
    height = h;
}

//=============================================================================
void Cell::Rotate ()
{
    tileIndex = (tileIndex + 1) % tileSet->tileSetDef->tileCount;
}

} // namespace Map