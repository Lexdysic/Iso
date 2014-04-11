
#include "WdPch.h"

namespace Widgets
{


//=============================================================================
//
// Constants
//
//=============================================================================

const float32 ZOOM = 1.1f;



//=============================================================================
//
// CRoot
//
//=============================================================================

//=============================================================================
CRoot::CRoot () :
    CWidget(null),
    m_menu(null),
    m_isMouseDrag(false),
    m_isMouseDown(false)
{
    SetMouseEnable(
        UserInterface::FLAG_MOUSE_CLICK |
        UserInterface::FLAG_MOUSE_WHEEL |
        UserInterface::FLAG_MOUSE_FOCUS
    );
}

//=============================================================================
CRoot::~CRoot ()
{

}

//=============================================================================
void CRoot::OnWidgetLayout (const Point2 & thisPos, const Vector2 & thisSize)
{
    if (m_menu)
    {
        const Vector2 & size = m_menu->GetNativeSize();
        const Point2 pos(0.0f, 0.0f);
        m_menu->SetPosition(pos);
        m_menu->SetSize(size);
    }
}

//=============================================================================
void CRoot::OnWidgetMouseDown (EMouse button, const Point2 & mousePos)
{
    WorldView::ICamera * camera = WorldView::GetContext()->GetCamera();
    const Point2 & worldPos  = camera->Unproject(mousePos);
    const Point2s cellPos    = Map::GetContext()->Pick(worldPos);

    if (m_menu)
    {
        delete m_menu;
        m_menu = null;
    }

    if (button == EMouse::Primary)
    {
        //Map::GetContext()->CellRotate(cellPos);
        m_dragStart = mousePos;
        m_isMouseDown = true;
    }
    else
    {
        m_menu = new CCellMenu(this, cellPos);
    }
}

//=============================================================================
void CRoot::OnWidgetMouseUp (EMouse button, const Point2 & mousePos)
{
    if (button == EMouse::Primary)
    {
        if (m_isMouseDrag)
        {

        }
        else
        {
            WorldView::ICamera * camera = WorldView::GetContext()->GetCamera();
            const Point2 &  worldPos  = camera->Unproject(mousePos);
            const Point2s & cellPos   = Map::GetContext()->Pick(worldPos);

            Map::ICell * cell = Map::GetContext()->GetCell(cellPos);
            if (cell)
                cell->Rotate();
        }
    }

    m_isMouseDrag = false;
    m_isMouseDown = false;
}

//=============================================================================
void CRoot::OnWidgetMouseMove (const Point2 & pos)
{
    if (m_isMouseDown)
    {
        if (m_isMouseDrag)
        {
            const Vector2 delta = pos - m_dragLast;
            m_dragLast = pos;
            WorldView::ICamera * camera = WorldView::GetContext()->GetCamera();
            const float32 zoom = camera->GetZoom();
            camera->Move(delta / zoom);
        }
        else
        {
            const Vector2 delta = pos - m_dragStart;
            if (LengthSq(delta) > Sq(5.0f))
            {
                m_isMouseDrag = true;
                m_dragLast = pos;
            }
        }
    }
}

//=============================================================================
void CRoot::OnWidgetMouseWheel (sint ticks, const Point2 & mousePos)
{
    WorldView::ICamera * camera = WorldView::GetContext()->GetCamera();
    //const Point2 &  worldPos  = camera->Unproject(mousePos);
    //const Point2s & cellPos   = Map::GetContext()->Pick(worldPos);

    //Map::ICell * cell = Map::GetContext()->GetCell(cellPos);
    //if (!cell)
    //    return;

    //const float32 height = cell->GetHeight();
    //cell->SetHeight(height - ticks * 15.0f);

    const float32 zoom = Pow(ZOOM, float32(ticks));
    camera->Zoom(zoom);
}










CNode::CNode (UserInterface::CWidget * parent) :
    CWidget(parent),
    m_isMouseDown(false)
{
    SetMouseEnable(
        UserInterface::FLAG_MOUSE_CLICK |
        UserInterface::FLAG_MOUSE_FOCUS
    );
}

CNode::~CNode ()
{
}

void CNode::OnWidgetUpdate ()
{
}

void CNode::OnWidgetRender (const Point2 & pos, const Vector2 & size)
{
    Graphics::IRenderTarget * backbuffer = Graphics::GetContext()->Backbuffer();

    // Background
    backbuffer->Rectangle(
        pos,
        pos + size,
        Color(0.6f, 0.6f, 0.6f)
    );
}

void CNode::OnWidgetGetNativeSize (Vector2 * outSize)
{
    *outSize = Vector2(150.0f, 100.0);
}

void CNode::OnWidgetMouseDown (EMouse button, const Point2 & pos)
{
    m_isMouseDown = true;
    m_dragOffset = pos - GetPosition();
}

void CNode::OnWidgetMouseUp (EMouse button, const Point2 & pos)
{
    m_isMouseDown = false;
}

void CNode::OnWidgetMouseMove (const Point2 & pos)
{
    if (!m_isMouseDown)
        return;

    SetPosition(pos - m_dragOffset);
}







} // namespace Widget