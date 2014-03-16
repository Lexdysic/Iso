
#include "WdPch.h"

namespace Widgets
{

const float32 SPACING = 5.0f;

//=============================================================================
CCellMenu::CCellMenu (UserInterface::CWidget * parent, const Point2s & cellPos) :
    CWidget(parent),
    m_cellPos(cellPos),
    m_offset(0.0f)
{
    
    m_tiles.Reserve(Content::TileSetGetCount());
    for (uint i = 0; i < Content::TileSetGetCount(); ++i)
    {
        const Content::TileSetDef * tileSet = Content::TileSetEnum(i);
        m_tiles.Add(new CCellMenuTile(this, tileSet, this));
    }

    SetMouseEnable(UserInterface::FLAG_MOUSE_CLICK | UserInterface::FLAG_MOUSE_WHEEL);
}

//=============================================================================
CCellMenu::~CCellMenu ()
{
}

//=============================================================================
void CCellMenu::OnWidgetUpdate ()
{

}

//=============================================================================
void CCellMenu::OnWidgetRender (const Point2 & pos, const Vector2 & size)
{
    Graphics::IRenderTarget * backbuffer = Graphics::GetContext()->Backbuffer();

    // Background
    backbuffer->Rectangle(
        pos,
        pos + size,
        Color(0.5f, 0.6f, 0.7f, 0.5f)
    );
}

//=============================================================================
void CCellMenu::OnWidgetLayout (const Point2 & thisPos, const Vector2 & thisSize)
{
    float32 y = m_offset;
    for (CCellMenuTile * tile : m_tiles)
    {
        const Vector2 & nativeSize = tile->GetNativeSize();
        const Vector2 & size = Vector2(thisSize.x, nativeSize.y);
        tile->SetPosition(thisPos + Vector2(0.0f, y));
        tile->SetSize(size);
        y += size.y + SPACING;
    }
}

//=============================================================================
void CCellMenu::OnWidgetGetNativeSize (Vector2 * outSize)
{
    float maxX = 0.0f;
    for (CCellMenuTile * tile : m_tiles)
    {
        const Vector2 & size = tile->GetNativeSize();
        maxX = Max(maxX, size.x);
    }

    const Vector2 & screenSize = Graphics::GetContext()->GetScreenSize();
    outSize->x = maxX;
    outSize->y = screenSize.y;
}

//=============================================================================
void CCellMenu::OnWidgetMouseDown (EMouse button, const Point2 & pos)
{

}

//=============================================================================
void CCellMenu::OnWidgetMouseWheel (sint ticks, const Point2 & pos)
{
    m_offset += ticks * 100.0f;
    ScheduleLayout();
}

//=============================================================================
void CCellMenu::OnCellMenuTileClicked (CCellMenuTile * tile)
{
    const Content::TileSetDef * tileSetDef = tile->GetTileSetDef();
    Map::GetContext()->CellSetTileSet(m_cellPos, tileSetDef);
}


} // namespace Widgets
