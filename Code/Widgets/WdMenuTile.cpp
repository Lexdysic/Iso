
#include "WdPch.h"

namespace Widgets
{

//=============================================================================
CCellMenuTile::CCellMenuTile (
    UserInterface::CWidget * parent,
    const Content::TileSetDef * tileSet,
    ICellMenuTileNotify * notify
) :
    CWidget(parent),
    m_tileset(tileSet),
    m_hasFocus(false),
    m_notify(notify)
{
    const CString & filename = CStringUtf16::FromData(m_tileset->tiles[0].image);

    m_image = Graphics::GetContext()->ImageLoad(filename);

    SetMouseEnable(UserInterface::FLAG_MOUSE_CLICK | UserInterface::FLAG_MOUSE_FOCUS);
}

//=============================================================================
CCellMenuTile::~CCellMenuTile ()
{
    Graphics::GetContext()->ImageDestroy(m_image);
}

//=============================================================================
void CCellMenuTile::OnWidgetUpdate ()
{

}

//=============================================================================
void CCellMenuTile::OnWidgetRender (const Point2 & thisPos, const Vector2 & thisSize)
{
    Graphics::IRenderTarget * backbuffer = Graphics::GetContext()->Backbuffer();

    // Background
    if (m_hasFocus)
    {
        backbuffer->Rectangle(
            thisPos,
            thisPos + thisSize,
            Color(1.0f, 1.0f, 1.0f, 0.5f)
        );
    }

    // Image
    {
        const Vector2 & size = m_image->GetSize();
        const Point2 pos = thisPos + (thisSize - size) * 0.5f;
        backbuffer->Draw(m_image, pos);
    }
}

//=============================================================================
void CCellMenuTile::OnWidgetGetNativeSize (Vector2 * outSize)
{
    *outSize = m_image->GetSize();
}

//=============================================================================
void CCellMenuTile::OnWidgetMouseDown (EMouse button, const Point2 & pos)
{
    m_notify->OnCellMenuTileClicked(this);
}

//=============================================================================
void CCellMenuTile::OnWidgetMouseEnter ()
{
    m_hasFocus = true;
}

//=============================================================================
void CCellMenuTile::OnWidgetMouseExit ()
{
    m_hasFocus = false;
}

//=============================================================================
//
//  ICellMenuTileNotify
//
//=============================================================================

void ICellMenuTileNotify::OnCellMenuTileClicked (CCellMenuTile *) {}

} // namespace Widgets
