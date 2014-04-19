
#include "AppPch.h"

#if 0
#define APP_RENDER_CENTER_DOT
#endif

//=============================================================================
//
// CApplication
//
//=============================================================================

//=============================================================================
CApplication::CApplication ()
{
    m_window = System::WindowCreate("Game", 1024, 768);
    m_window->NotifyRegister(this);

    Graphics::GetContext()->Initialize(m_window);
    UserInterface::GetContext()->Initialize(m_window);
    InputGetManager()->Initialize(m_window);
    EntityGetContext()->Initialize();
    Physics::GetContext()->Initialize();

    Map::GetContext()->Initialize();

    m_app = new CAppBouyant();
}

//=============================================================================
CApplication::~CApplication ()
{
    delete m_app;

    EntityGetContext()->Uninitialize();
    InputGetManager()->Uninitialize();
    Map::GetContext()->Uninitialize();
    UserInterface::GetContext()->Uninitialize();
    Graphics::GetContext()->Uninitialize();
    Physics::GetContext()->Uninitialize();

    m_window->NotifyUnregister(this);
    System::WindowDestroy(m_window);
}

//=============================================================================
void CApplication::Go ()
{
    while (m_window->Update())
    {
        Update();
        Render();
    }
}

//=============================================================================
void CApplication::Update ()
{
    m_frameTimer.Update();
    Time::Update();
    const Time::Delta deltaTime = Time::GetGameDelta();

    InputGetManager()->Update();
    Physics::GetContext()->Update(deltaTime);
    Pathing::GetContext()->Update();
    m_app->Update();
    EntityGetContext()->Update();

    // TODO: only need to do this when the window's size actually changes
    WorldView::GetContext()->SetScreenSize(Graphics::GetContext()->GetScreenSize());

    WorldView::GetContext()->Update(deltaTime);

    UserInterface::GetContext()->Update(deltaTime);
}

//=============================================================================
void CApplication::Render ()
{
    Graphics::IRenderTarget * backbuffer = Graphics::GetContext()->Backbuffer();
    backbuffer->Begin();
    backbuffer->Clear(Color(0.0f, 0.15f, 0.25f));

    m_app->Render();
    Graphics::GetContext()->Render();

    // User Interface
    backbuffer->SetView(Matrix23::Identity);
    backbuffer->SetWorld(Matrix23::Identity);

    UserInterface::GetContext()->Render();

    Graphics::GetContext()->DebugRender();

    // Debug Info
    {
        // TODO: move these lines to more appropriate places
        const float curFps = 1.0f / m_frameTimer.GetCurrentFrameTime().GetSeconds();
        const float aveFps = 1.0f / m_frameTimer.GetAverageFrameTime().GetSeconds();
        DebugValue("Time Scale", Time::GetGameScale());
        DebugValue("Current FPS", curFps);
        DebugValue("Average FPS", aveFps);

        char text[256];
        DebugValueGetString(text, array_size(text));
        DebugValueClear();

        const Vector2 & size = Graphics::GetContext()->DebugTextMeasure(text, Vector2(10000.0f, 10000.0f));
        backbuffer->Rectangle(Point2(0, 0), size, Color(1, 1, 1, 0.5f));

        Graphics::GetContext()->DebugText(text, Point2(0, 0), Vector2(200, 500));
    }

#ifdef APP_RENDER_CENTER_DOT
    backbuffer->Circle(
        Graphics::GetContext()->GetScreenSize() * 0.5f,
        1.0f,
        Color(1.0f, 0.0f, 0.0f)
    );
#endif

    backbuffer->End();
}

//=============================================================================
void CApplication::OnWindowKeyDown (EKey key)
{
    const float TIME_SCALE = 2.0f;

    switch (key)
    {
        case EKey::PageUp:
            Time::SetGameScale(Time::GetGameScale() * TIME_SCALE);
        return;

        case EKey::PageDown:
            Time::SetGameScale(Time::GetGameScale() / TIME_SCALE);
        return;

        case EKey::Home:
            Time::SetGameScale(1.0f);
        return;

        case EKey::End:
            Time::SetGameScale(0.0f);
        return;

        case EKey::F1:
            EntityGetContext()->DebugToggleDraw();
        return;

        case EKey::F2:
            Physics::GetContext()->DebugToggleCollider();
        return;

        case EKey::F3:
            Physics::GetContext()->DebugToggleRigidBody();
        return;
    }

    m_app->OnWindowKeyDown(key);
}

//=============================================================================
void CApplication::OnWindowMouseDown (EMouse button, const Point2s & pos)
{
    m_app->OnWindowMouseDown(button, pos);
}
