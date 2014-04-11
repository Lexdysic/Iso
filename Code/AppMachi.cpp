
#include "AppPch.h"

//  for each player
//      for each phase

// for each phase
//      for each player

// for each phase
//      while players still have actions

//=============================================================================
//
// CAppMachi
//
//=============================================================================

//=============================================================================
CAppMachi::CAppMachi ()
{

    {
        const Vector2 SIZE = { 50.0f, 20.0f };

        m_entity = EntityGetContext()->CreateEntity();

        // Transform
        {
            auto * transform = EnsureComponent<CTransformComponent2>(m_entity);
            transform->SetPosition({500, 400});
            transform->SetRotation(Radian(Degree(20.0f)));
        }

        // Collider
        {
            using namespace Physics;
            auto * collider = EnsureComponent<IColliderComponent>(m_entity, Aabb2(Point2(-0.5f * SIZE), Point2(0.5f * SIZE)));
        }

        // Graphics
        {
            using namespace Graphics;
            auto * primative = IPrimativeComponent::Attach(m_entity, SIZE);
        }
    }
}

//=============================================================================
CAppMachi::~CAppMachi ()
{
    EntityGetContext()->DestroyEntity(m_entity);
}

//=============================================================================
void CAppMachi::Update ()
{

}

//=============================================================================
void CAppMachi::Render ()
{
    Graphics::IRenderTarget * backbuffer = Graphics::GetContext()->Backbuffer();
    backbuffer->SetView(Matrix23::Identity);
    backbuffer->SetWorld(Matrix23::Identity);
}