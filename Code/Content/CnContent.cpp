
#include "CnPch.h"

namespace Content
{

const TileSetDef s_tileSetDefs[] =
{
    //// Dirt
    //{
    //    TILE_TYPE_Fixed,
    //    1,
    //    { L"Assets/Art/Ground/dirt.png",        8,  8,  8,  8 },
    //},
    //// Dirt-Double
    //{
    //    TILE_TYPE_Fixed,
    //    1,
    //    { L"Assets/Art/Ground/dirtDouble.png",  15, 15, 15, 15 },
    //},
    // Grass
    {
        TILE_TYPE_Fixed,
        1,
        { L"Assets/Art/Ground/grass.png",        15, 15, 15, 15 },
    },
    // Grass-Double
    {
        TILE_TYPE_Fixed,
        1,
        { L"Assets/Art/Ground/grassWhole.png",  15, 15, 15, 15 },
    },
    // Hill Side
    {
        TILE_TYPE_Rotate,
        4,
        {
            { L"Assets/Art/Ground/hillN.png", 15, 30, 30, 15 },
            { L"Assets/Art/Ground/hillE.png", 15, 15, 30, 30 },
            { L"Assets/Art/Ground/hillS.png", 30, 15, 15, 30 },
            { L"Assets/Art/Ground/hillW.png", 30, 30, 15, 15 },
        },
    },
    // Hill Corner
    {
        TILE_TYPE_Rotate,
        4,
        {
            { L"Assets/Art/Ground/hillNE.png", 15, 15, 30, 15 },
            { L"Assets/Art/Ground/hillES.png", 15, 15, 15, 30 },
            { L"Assets/Art/Ground/hillSW.png", 30, 15, 15, 15 },
            { L"Assets/Art/Ground/hillNW.png", 15, 30, 15, 15 },
        }
    },
    // Road-cross
    {
        TILE_TYPE_Fixed,
        1,
        {
            { L"Assets/Art/Roads/crossroad.png",  15, 15, 15, 15 },
        }
    },
    // Road-T
    {
        TILE_TYPE_Rotate,
        4,
        {
            { L"Assets/Art/Roads/crossroadNES.png",  15, 15, 15, 15 },
            { L"Assets/Art/Roads/crossroadESW.png",  15, 15, 15, 15 },
            { L"Assets/Art/Roads/crossroadNSW.png",  15, 15, 15, 15 },
            { L"Assets/Art/Roads/crossroadNEW.png",  15, 15, 15, 15 },
        }
    },
    // Road-End
    {
        TILE_TYPE_Rotate,
        4,
        {
            { L"Assets/Art/Roads/endN.png",  15, 15, 15, 15 },
            { L"Assets/Art/Roads/endE.png",  15, 15, 15, 15 },
            { L"Assets/Art/Roads/endS.png",  15, 15, 15, 15 },
            { L"Assets/Art/Roads/endW.png",  15, 15, 15, 15 },
        }
    },
    // Road-Straight
    {
        TILE_TYPE_Rotate,
        2,
        {
            { L"Assets/Art/Roads/roadEW.png",  15, 15, 15, 15 },
            { L"Assets/Art/Roads/roadNS.png",  15, 15, 15, 15 },
        }
    },
    // Road-Corner
    {
        TILE_TYPE_Rotate,
        4,
        {
            { L"Assets/Art/Roads/roadNE.png",  15, 15, 15, 15 },
            { L"Assets/Art/Roads/roadES.png",  15, 15, 15, 15 },
            { L"Assets/Art/Roads/roadSW.png",  15, 15, 15, 15 },
            { L"Assets/Art/Roads/roadNW.png",  15, 15, 15, 15 },
        }
    },
    // Hill
    {
        TILE_TYPE_Rotate,
        4,
        {
            { L"Assets/Art/Roads/roadHillN.png",  15, 15, 15, 15 },
            { L"Assets/Art/Roads/roadHillE.png",  15, 15, 15, 15 },
            { L"Assets/Art/Roads/roadHillS.png",  15, 15, 15, 15 },
            { L"Assets/Art/Roads/roadHillW.png",  15, 15, 15, 15 },
        }
    },
    //// Lot-Exit
    //{
    //    TILE_TYPE_Rotate,
    //    4,
    //    {
    //        { L"Assets/Art/Roads/exitE.png",  15, 15, 15, 15 },
    //        { L"Assets/Art/Roads/exitN.png",  15, 15, 15, 15 },
    //        { L"Assets/Art/Roads/exitS.png",  15, 15, 15, 15 },
    //        { L"Assets/Art/Roads/exitW.png",  15, 15, 15, 15 },
    //    }
    //},
    //// Lot-Edge
    //{
    //    TILE_TYPE_Rotate,
    //    4,
    //    {
    //        { L"Assets/Art/Roads/lotE.png",  15, 15, 15, 15 },
    //        { L"Assets/Art/Roads/lotN.png",  15, 15, 15, 15 },
    //        { L"Assets/Art/Roads/lotS.png",  15, 15, 15, 15 },
    //        { L"Assets/Art/Roads/lotW.png",  15, 15, 15, 15 },
    //    }
    //},
    //// Lot-Corner
    //{
    //    TILE_TYPE_Rotate,
    //    4,
    //    {
    //        { L"Assets/Art/Roads/lotES.png",  15, 15, 15, 15 },
    //        { L"Assets/Art/Roads/lotNE.png",  15, 15, 15, 15 },
    //        { L"Assets/Art/Roads/lotNW.png",  15, 15, 15, 15 },
    //        { L"Assets/Art/Roads/lotSW.png",  15, 15, 15, 15 },
    //    }
    //},
    //// Lot
    //{
    //    TILE_TYPE_Fixed,
    //    1,
    //    {
    //        { L"Assets/Art/Roads/road.png",  15, 15, 15, 15 },
    //    }
    //},
    
    // City
    {
        TILE_TYPE_Rotate,
        4,
        {
            { L"Assets/Art/Carc/city1.png",  15, 15, 15, 15 },
            { L"Assets/Art/Carc/city2.png",  15, 15, 15, 15 },
            { L"Assets/Art/Carc/city3.png",  15, 15, 15, 15 },
            { L"Assets/Art/Carc/city4.png",  15, 15, 15, 15 },
        }
    },
};


const TileSetDef * TileSetList::begin () { return s_tileSetDefs; }
const TileSetDef * TileSetList::end () { return s_tileSetDefs + array_size(s_tileSetDefs); }

uint TileSetGetCount ()
{
    return array_size(s_tileSetDefs);
}

const TileSetDef * TileSetEnum (uint index)
{
    assert(index < array_size(s_tileSetDefs));
    return s_tileSetDefs + index;
}


AUTO_INIT_FUNC(TestJson)
{
    Json::CValue root = Json::ParseFile(L"P:\\Dev\\Iso\\Assets\\Entities\\bullet.json");
}

} // namespace Content