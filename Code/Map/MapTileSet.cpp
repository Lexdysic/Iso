
#include "MapPch.h"

namespace Map
{

//=============================================================================
//
// TileSet
//
//=============================================================================

//=============================================================================
TileSet::TileSet () :
    tileSetDef(NULL)
{

    MemZero(images);
}

//=============================================================================
TileSet::~TileSet ()
{
}

} // namespace Map