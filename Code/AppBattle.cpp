
#include "AppPch.h"


//=============================================================================
//
// Helpers
//
//=============================================================================

//=============================================================================
IEntity * CreateShot (IEntity * shooter, float speed)
{
    const float32 RADIUS = 4.0f;

    auto * shooterTransform = shooter->Get<CTransformComponent2>();

    IEntity * shot = EntityGetContext()->CreateEntity();

    // Transform
    {
        auto * transform = EnsureComponent<CTransformComponent2>(shot);
        transform->SetPosition(shooterTransform->GetPosition());
        transform->SetRotation(shooterTransform->GetRotation());
    }

    // Rigid Body
    {
        using namespace Physics;
        auto * rigidBody = EnsureComponent<IRigidBodyComponent>(shot);

        const Radian shooterRotation = shooterTransform->GetRotation();
        const Vector2 aim(
            Cos(shooterRotation),
            Sin(shooterRotation)
        );
        rigidBody->SetVelocity(aim * speed);
        rigidBody->SetAngularVelocity(Radian(Math::Tau * -2.0f));
    }

    // Collider
    {
        using namespace Physics;
        auto * collider = EnsureComponent<IColliderComponent>(shot, Circle(Point2::Zero, RADIUS));
    }

    // Graphics
    {
        using namespace Graphics;
        auto primative = IPrimativeComponent::Attach(shot, RADIUS);
    }

    return shot;
}

//=============================================================================
IEntity * CreateTank ()
{
    auto * entityBody = EntityGetContext()->CreateEntity();
    {
        const Vector2 SIZE(40.0f, 20.0f);
        {
            auto * input = EnsureComponent<CTankControlComponent>(entityBody);
        }

        {
            auto * transform = EnsureComponent<CTransformComponent2>(entityBody);
            transform->SetPosition(Point2(100.0f, 400.0f));
        }

        {
            using namespace Graphics;
            auto * primative = IPrimativeComponent::Attach(entityBody, Vector2(40.0f, 20.0f));
        }

        {
            using namespace Physics;
            auto * collider = EnsureComponent<IColliderComponent>(entityBody, Aabb2(Point2::Zero, 0.5f * SIZE));
        }
    }

    auto * entityGun = EntityGetContext()->CreateEntity();
    {
        {
            auto * input = EnsureComponent<CGunControlComponent>(entityGun);
        }

        {
            using namespace Graphics;
            auto * primative = IPrimativeComponent::Attach(entityGun, Vector2(40.0f, 10.0f));
            primative->SetPosition(Point2(20.0f, 0.0f));
        }

        {
            auto * transform = EnsureComponent<CTransformComponent2>(entityGun);
            transform->SetParent(entityBody);
            transform->SetPositionLocal(Point2(0.0f, -5.0));
        }
    }

    return entityBody;
}

//=============================================================================
float32 MapRange (float32 inMin, float32 inMax, float32 outMin, float32 outMax, float32 input)
{
    const float32 t = InvLerp(Clamp(input, inMin, inMax), inMin, inMax);
    return Lerp(outMin, outMax, t);
}


//=============================================================================
//
// CTankControlComponent
//
//=============================================================================

//=============================================================================
const ComponentType CTankControlComponent::TYPE('T','a','n','k','C','o','n','t','r','o','l');

//=============================================================================
CTankControlComponent::CTankControlComponent ()
{
    EntityGetContext()->NotifyRegister(this);
}

//=============================================================================
CTankControlComponent::~CTankControlComponent ()
{
    EntityGetContext()->NotifyUnregister(this);
}

//=============================================================================
void CTankControlComponent::OnEntityUpdate ()
{
    auto * owner = GetOwner();
    auto * transform = owner->Get<CTransformComponent2>();

    const Vector2 LINEAR_SPEED(100.0f, 0.0f);

    if (InputGetManager()->KeyIsDown(EKey::Left))
        transform->UpdatePositionLocal(-LINEAR_SPEED * Time::GetGameDelta().GetSeconds());

    if (InputGetManager()->KeyIsDown(EKey::Right))
        transform->UpdatePositionLocal(LINEAR_SPEED * Time::GetGameDelta().GetSeconds());

    transform->UpdatePosition(LINEAR_SPEED * Time::GetGameDelta().GetSeconds() * InputGetManager()->GetAxis(EGameAxis::LeftStickX));
}

//=============================================================================
CTankControlComponent * CTankControlComponent::Attach (IEntity * entity)
{
    CTankControlComponent * comp = new CTankControlComponent();

    entity->Attach(comp);

    return comp;
}


//=============================================================================
//
// CGunControlComponent
//
//=============================================================================

//=============================================================================
const ComponentType CGunControlComponent::TYPE('G','u','n','C','o','n','t','r','o','l');

//=============================================================================
CGunControlComponent::CGunControlComponent ()
{
    InputGetManager()->NotifyRegister(this);
    EntityGetContext()->NotifyRegister(this);
}

//=============================================================================
CGunControlComponent::~CGunControlComponent ()
{
    InputGetManager()->NotifyUnregister(this);
    EntityGetContext()->NotifyUnregister(this);
}

//=============================================================================
void CGunControlComponent::OnInputKeyDown (EKey key)
{
    auto * owner = GetOwner();

    switch (key)
    {
        case EKey::Space:
            m_shotStart = Time::GetGameTime();
        break;
    }
}

//=============================================================================
void CGunControlComponent::OnInputKeyUp (EKey key)
{
    switch (key)
    {
        case EKey::Space:
        {
            const Time::Delta delta = Time::GetGameTime() - m_shotStart;
            const float speed = MapRange(0.0f, 1.0f, 100.0f, 300.0f, delta.GetSeconds());
            CreateShot(GetOwner(), speed);
        }
        break;
    }
}

//=============================================================================
void CGunControlComponent::OnInputGamepadDown(EButton button)
{
    if (button != EButton::RightDpadDown)
        return;
    
    const float32 trigger = InputGetManager()->GetAxis(EGameAxis::RightTrigger);
    const float speed = MapRange(0.0f, 1.0f, 100.0f, 300.0f, trigger);
    CreateShot(GetOwner(), speed);
}

//=============================================================================
void CGunControlComponent::OnEntityUpdate ()
{
    auto * owner = GetOwner();
    auto * transform = owner->Get<CTransformComponent2>();

    const Radian ROTATION_SPEED = Radian(Degree(90.0f));
    const Vector2 LINEAR_SPEED(100.0f, 0.0f);

    const Radian rotation = transform->GetRotationLocal();

    if (InputGetManager()->KeyIsDown(EKey::Down) && rotation > 0.0f)
        transform->UpdateRotationLocal(-1.0f * ROTATION_SPEED * Time::GetGameDelta().GetSeconds());

    if (InputGetManager()->KeyIsDown(EKey::Up) && rotation < Math::Pi)
        transform->UpdateRotationLocal(ROTATION_SPEED * Time::GetGameDelta().GetSeconds());

    transform->UpdateRotationLocal(ROTATION_SPEED * Time::GetGameDelta().GetSeconds() * -InputGetManager()->GetAxis(EGameAxis::RightStickY));
}

//=============================================================================
CGunControlComponent * CGunControlComponent::Attach (IEntity * entity)
{
    auto * comp = new CGunControlComponent();

    entity->Attach(comp);

    return comp;
}



//=============================================================================
//
// CApplication
//
//=============================================================================

//=============================================================================
CAppBattle::CAppBattle () :
    m_shot(null),
    m_tank(null)
{
    m_ground        = Graphics::GetContext()->ImageLoad("Assets/Art/Backgrounds/ground.png");
    m_mask          = Graphics::GetContext()->ImageCreate(m_ground->GetDims());
    m_renderTarget  = Graphics::GetContext()->RenderTargetCreate(m_ground->GetDims());

    m_tank = CreateTank();
}

//=============================================================================
CAppBattle::~CAppBattle ()
{
    Graphics::GetContext()->ImageDestroy(m_ground);
    Graphics::GetContext()->ImageDestroy(m_mask);
    Graphics::GetContext()->RenderTargetDestroy(m_renderTarget);

    EntityGetContext()->DestroyEntity(m_tank);
    EntityGetContext()->DestroyEntity(m_shot);
}

//=============================================================================
void CAppBattle::Update ()
{

}

//=============================================================================
void CAppBattle::Render ()
{
    m_renderTarget->Begin();
    m_renderTarget->Clear();
    m_renderTarget->Line(Point2(0.0f, 0.0f), Point2(100.0f, 100.0f), Color(1.0f, 1.0f, 1.0f, 1.0f), 50.0f);
    m_renderTarget->End();


    Graphics::IRenderTarget * backbuffer = Graphics::GetContext()->Backbuffer();
    backbuffer->SetView(Matrix23::Identity);

    // User Interface
    backbuffer->SetView(Matrix23::Identity);
    backbuffer->SetWorld(Matrix23::Identity);
    //Graphics::GetContext()->Draw(m_ground, Vector2(0, 0));
    backbuffer->Draw(m_ground, m_renderTarget->GetImage(), Point2::Zero);

}