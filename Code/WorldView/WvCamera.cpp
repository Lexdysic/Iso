
#include "WvPch.h"

namespace WorldView
{

//=============================================================================
CCamera::CCamera () :
    m_pos(Point2::Zero),
    m_zoom(1.0f),
    m_size(Vector2::Zero)
{

}

//=============================================================================
CCamera::~CCamera ()
{

}

//=============================================================================
void CCamera::Update ()
{
    // TODO: cache these results
    const Matrix23 center    = Matrix23::CreateTranslation(0.5f * m_size);
    const Matrix23 scale     = Matrix23::CreateScale(m_zoom);
    const Matrix23 translate = Matrix23::CreateTranslation(m_pos);

    m_worldToScreen = center * scale * translate;
    m_screenToWorld = Inverse(m_worldToScreen);
}

//=============================================================================
void CCamera::Move (const Vector2 & delta)
{
    m_pos += delta;
}

//=============================================================================
void CCamera::Zoom (float32 factor)
{
    m_zoom *= factor;
}

} // namespace WorldView