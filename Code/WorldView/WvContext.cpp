
#include "WvPch.h"

namespace WorldView
{

//=============================================================================
CContext CContext::s_context;

//=============================================================================
void CContext::Update (float32 elapsedTime)
{
    m_camera.Update();
}

//=============================================================================
void CContext::SetScreenSize (const Vector2 & size)
{
    m_camera.SetSize(size);
}

//=============================================================================
IContext * GetContext ()
{
    return CContext::Get();
}

} // namespace WorldView