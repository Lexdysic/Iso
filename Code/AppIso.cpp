
#include "AppPch.h"


//=============================================================================
//
// Constants
//
//=============================================================================

const float32 MOVE = 10.0f;
const float32 ZOOM = 1.1f;


//=============================================================================
//
// CAppIso
//
//=============================================================================

//=============================================================================
CAppIso::CAppIso ()
{
    Map::GetContext()->Initialize();

    m_root = new Widgets::CRoot();
}

//=============================================================================
CAppIso::~CAppIso ()
{
    Map::GetContext()->Uninitialize();
}

//=============================================================================
void CAppIso::Update ()
{

}

//=============================================================================
void CAppIso::Render ()
{
    // Camera View
    WorldView::ICamera * camera = WorldView::GetContext()->GetCamera();
    Graphics::IRenderTarget * backbuffer = Graphics::GetContext()->Backbuffer();
    backbuffer->SetView(camera->GetWorldToScreen());
    Map::GetContext()->Render();

    // Render the outline of the selected tile
    {
        const Point2 cursorPos = CursorGetPos(System::GetWindow());
        const Point2 & worldPos = camera->Unproject(cursorPos);
        const Point2s tilePos = Map::GetContext()->Pick(worldPos);
        Map::GetContext()->DrawOutline(tilePos);
    }

}

//=============================================================================
void CAppIso::OnWindowKeyDown (EKey key)
{
    WorldView::ICamera * camera = WorldView::GetContext()->GetCamera();
    switch (key)
    {
        case EKey::A:
        case EKey::Left:
            camera->Move(Vector2(MOVE, 0.0f));
        break;

        case EKey::D:
        case EKey::Right:
            camera->Move(Vector2(-MOVE, 0.0f));
        break;

        case EKey::W:
        case EKey::Up:
            camera->Move(Vector2(0.0f, MOVE / 2));
        break;

        case EKey::S:
        case EKey::Down:
            camera->Move(Vector2(0.0f, -MOVE / 2));
        break;

        case EKey::Home:
            camera->Zoom(ZOOM);
        break;

        case EKey::End:
            camera->Zoom(1.0f / ZOOM);
        break;

        case EKey::Space:
            Map::GetContext()->Generate();
        break;
    }
}

void CAppIso::OnWindowMouseDown (EMouse button, const Point2s & pos)
{

}
