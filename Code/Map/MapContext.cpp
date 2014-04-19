
#include "MapPch.h"

namespace Map
{



//=============================================================================
//
// Map
//
//=============================================================================

//=============================================================================
CContext CContext::s_context;

//=============================================================================
CContext::CContext ()
{

}

//=============================================================================
CContext::~CContext ()
{

}

//=============================================================================
void CContext::Initialize ()
{
    // Populate the tileSets
    for (const Content::TileSetDef & tileSetDef : Content::TileSetList())
    {
        TileSet tileSet;
        tileSet.tileSetDef = &tileSetDef;

        for (uint32 i = 0; i < array_size(tileSetDef.tiles); ++i)
        {
            const CString & filename = CStringUtf16::FromData(tileSetDef.tiles[i].image);
            if (filename.IsNullOrEmpty())
                break;

            Graphics::IImage * image = Graphics::GetContext()->ImageLoad(filename);
            ASSERT_MSG(image, "Could not find file \"%s\"", filename.Ptr());

            tileSet.images[i] = image;
        }
        m_tileSets.Add(tileSet);
    }

    SetTileSize(100.0f, 16.0f);

    Generate();
}

//=============================================================================
void CContext::Uninitialize ()
{
    for (const TileSet & tileSet : m_tileSets)
    {
        for (Graphics::IImage * image : tileSet.images)
        {
            if (!image)
                continue;

            Graphics::GetContext()->ImageDestroy(image);
        }
    }

    m_tileSets.Clear();
}

//=============================================================================
void CContext::SetTileSize (float32 width, float32 thick)
{
    m_tileThickness = thick;

    const Vector2 tileSize(width, 0.5f * width);
    m_tileToIso = Matrix22(
        0.5f * tileSize.x, -0.5f * tileSize.x,
        0.5f * tileSize.y,  0.5f * tileSize.y
    );
    m_isoToTile = Inverse(m_tileToIso);
}

//=============================================================================
void CContext::Generate ()
{
    m_cells.Clear();

    for (sint32 y = 0; y < 10; ++y)
    {
        for (sint32 x = 0; x < 10; ++x)
        {
            const uint32 tileSetIdx = m_random.Max(m_tileSets.Count());

            Cell cell;
            cell.pos.x     = x;
            cell.pos.y     = y;
            cell.tileSet   = &m_tileSets[tileSetIdx];
            cell.tileIndex = m_random.Max(cell.tileSet->tileSetDef->tileCount);
            cell.height    = 0.0f;
            assert(cell.tileIndex < cell.tileSet->tileSetDef->tileCount);

            m_cells.Add(cell);
        }
    }

    ResortCells();
}

//=============================================================================
void CContext::Render ()
{
    for (const Cell & cell : m_cells)
        DrawCell(cell);
}

//=============================================================================
void CContext::DrawCell (const Cell & cell)
{
    Graphics::IRenderTarget * backbuffer = Graphics::GetContext()->Backbuffer();

    const Point2 tilePos(float32(cell.pos.x), float32(cell.pos.y));

    Graphics::IImage * image = cell.tileSet->images[cell.tileIndex];
    ASSERT(image);

    const Vector2 & imageSize = image->GetSize();
    const Vector2 tileSize(imageSize.x, imageSize.x * 0.5f);

    const Vector2 worldPos = m_tileToIso * tilePos;
    const Point2 drawPos(
        worldPos.x - 0.5f * imageSize.x,
        worldPos.y - 1.0f * imageSize.y - cell.height + m_tileThickness
    );
    backbuffer->Draw(image, drawPos);
    backbuffer->Circle(worldPos, 1.0f, Color(0.0f, 0.0f, 1.0f));
}

//=============================================================================
void CContext::DrawOutline (const Point2s & tilePos)
{
    Graphics::IRenderTarget * backbuffer = Graphics::GetContext()->Backbuffer();

    const Cell * cell = FindCell(tilePos);

    const Content::TileDef * tileDef = cell ? (cell->tileSet->tileSetDef->tiles + cell->tileIndex) : null;
    const Vector2 height(0.0f, cell ? -cell->height : 0.0f);

    Vector2 tileBotPos(float32(tilePos.x), float32(tilePos.y));

    Point2 cornersBottom[Content::TILE_CORNERS];
    cornersBottom[Content::TILE_CORNER_Top]    = m_tileToIso * (tileBotPos - Vector2(1.0f, 1.0f)) + height;
    cornersBottom[Content::TILE_CORNER_Right]  = m_tileToIso * (tileBotPos - Vector2(0.0f, 1.0f)) + height;
    cornersBottom[Content::TILE_CORNER_Bottom] = m_tileToIso * (tileBotPos - Vector2(0.0f, 0.0f)) + height;
    cornersBottom[Content::TILE_CORNER_Left]   = m_tileToIso * (tileBotPos - Vector2(1.0f, 0.0f)) + height;

    Point2 cornersTop[Content::TILE_CORNERS];
    for (uint32 i = 0; i < array_size(cornersTop); ++i) 
    {
        cornersTop[i] = cornersBottom[i];
        cornersTop[i].y -= (tileDef ? tileDef->height[i] : 0) - m_tileThickness;
        //cornersTop[i] = cornersTop[i] + height;
    }

    const Color color(1.0f, 0.0f, 0.0f, 1.0f);
    const Color color50(1.0f, 0.0f, 0.0f, 0.5f);
    const Color color25(1.0f, 0.0f, 0.0f, 0.25f);
    for (int a = 0, b = array_size(cornersBottom) - 1; a < array_size(cornersBottom); b = a, a++)
    {
        backbuffer->Line(cornersTop[a], cornersTop[b], color); // Top
        backbuffer->Line(cornersTop[a], cornersBottom[a], color50); // Sides
        backbuffer->Line(cornersBottom[a], cornersBottom[b], color25); // Bottom
    }

}

//=============================================================================
Point2s CContext::Pick (const Point2 & worldPos)
{
    const Vector2 tilePos = m_isoToTile * worldPos;

    const Point2s tile(
       FloatToSint(tilePos.x + 0.5f),
       FloatToSint(tilePos.y + 0.5f)
    );

    return tile;
}

//=============================================================================
ICell * CContext::GetCell (const Point2s & cellPos)
{
    Cell * cell = FindCell(cellPos);
    return cell;
}

//=============================================================================
bool CContext::CellIsValid (const Point2s & cellPos)
{
    auto it = std::find_if(
        m_cells.Ptr(),
        m_cells.Term(),
        [=](const Cell & c) { return c.pos == cellPos; }
    );
    return it != m_cells.Term();
}

//=============================================================================
void CContext::CellRotate (const Point2s & tilePos)
{
    Cell * cell = FindCell(tilePos);
    if (!cell)
        return;

    cell->tileIndex = (cell->tileIndex + 1) % cell->tileSet->tileSetDef->tileCount;
}

//=============================================================================
void CContext::CellSetTileSet (const Point2s & cellPos, const Content::TileSetDef * tileSetDef)
{
    TileSet * tileSet = FindTileSet(tileSetDef);
    assert(tileSet);

    Cell * cell = FindCell(cellPos);
    if (cell)
    {
        cell->tileSet = tileSet;
        cell->tileIndex = 0;
    }
    else
    {
        Cell cell;
        cell.pos       = cellPos;
        cell.tileSet   = tileSet;
        cell.tileIndex = m_random.Max(cell.tileSet->tileSetDef->tileCount);
        cell.height    = 0.0f;

        ASSERT(cell.tileIndex < cell.tileSet->tileSetDef->tileCount);

        m_cells.Add(cell);
    }

    ResortCells();
}

//=============================================================================
void CContext::CellSetHeight (const Point2s & cellPos, float32 height)
{
    Cell * cell = FindCell(cellPos);
    if (!cell)
        return;

    cell->height = height;
}

//=============================================================================
Cell * CContext::FindCell (const Point2s & tilePos)
{
    auto cellIt = std::find_if(
        m_cells.Ptr(),
        m_cells.Term(),
        [=] (const Cell & c) {
        return c.pos == tilePos;
    });

    if (cellIt == m_cells.Term())
        return null;

    return &*cellIt;
}

//=============================================================================
TileSet * CContext::FindTileSet (const Content::TileSetDef * tileSetDef)
{
    auto it = std::find_if(
        m_tileSets.Ptr(),
        m_tileSets.Term(),
        [=] (const TileSet & t) {
            return t.tileSetDef == tileSetDef;
    });

    if (it == m_tileSets.Term())
        return null;

    return &*it;
}

//=============================================================================
void CContext::ResortCells ()
{
    // Sort by diagonal
    std::sort(
        m_cells.Ptr(),
        m_cells.Term(),
        [](const Cell & a, const Cell & b){ return (a.pos.x + a.pos.y) < (b.pos.x + b.pos.y); }
    );
}


//=============================================================================
//
// Free Functions
//
//=============================================================================

//=============================================================================
IContext * GetContext ()
{
    return CContext::Get();
}

} // namespace Map