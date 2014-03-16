
#include "WdPch.h"

namespace Widgets
{

//=============================================================================
CText::CText (UserInterface::CWidget * parent) :
    CWidget(parent)
{

}

//=============================================================================
CText::~CText ()
{

}

//=============================================================================
void CText::SetText (const wchar text[])
{
    m_text = text;
}

//=============================================================================
void CText::OnWidgetUpdate ()
{

}

//=============================================================================
void CText::OnWidgetRender (const Point2 & thisPos, const Vector2 & thisSize)
{
    Graphics::GetContext()->DebugText(
        m_text.c_str(),
        thisPos,
        thisSize
    );
}

//=============================================================================
void CText::OnWidgetGetNativeSize (Vector2 * outSize)
{
    const Vector2 & size = Graphics::GetContext()->DebugTextMeasure(
        m_text.c_str(),
        Vector2::One * 10000.0f
    );
    *outSize = size;
}

} // namespace Widgets
