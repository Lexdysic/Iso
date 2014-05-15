
#include "AppPch.h"

//=============================================================================
float32 OverlappedArea (const Circle & circle, const Aabb2 & box, float step = 1.0f)
{
    float total = 0.0f;
    for (float circleX = -circle.r; circleX < circle.r; circleX += step)
    {
        const float x = circle.x + circleX;
        const float circleHalfHeight = Sqrt(Sq(circle.r) - Sq(circleX));

        const Interval circleRange(
            circle.y - circleHalfHeight,
            circle.y + circleHalfHeight
        );

        const Interval boxRange = 
            Math::IsInRange(x, box.min.x, box.max.x) ?
            Interval(box.min.y, box.max.y) :
            Interval::Null;


        const Interval intersection = Intersection(circleRange, boxRange);

        if (!intersection.IsValid())
            continue;

        total += intersection.b - intersection.a;
    }

    const float area = total * step;
    return area;
}


//=============================================================================
//
// CAppBouyant
//
//=============================================================================

//=============================================================================
CAppBouyant::CAppBouyant ()
{
    Physics::GetContext()->NotifyRegister(this);

    // Water
    {
        const Vector2 SIZE = { 1000.0f, 400.0f };
        m_water = Aabb2(
            Point2(500.0f, 500.0f),
            SIZE * 0.5f
        );

        m_entityWater = EntityGetContext()->CreateEntity();

        // Transform
        {
            auto * transform = EnsureComponent<CTransformComponent2>(m_entityWater);
            transform->SetPosition({500, 500});
        }
        
        // Collider
        {
            using namespace Physics;
            auto * collider = EnsureComponent<IColliderComponent>(
                m_entityWater,
                Aabb2(Point2(-0.5f * SIZE), Point2(0.5f * SIZE)),
                EMaterial::Liquid
            );
        }

        // Graphics
        {
            using namespace Graphics;
            auto * primative = IPrimativeComponent::Attach(m_entityWater, SIZE);
            primative->SetColor(Color(Color::Blue, 0.5f));
        }
    }

    // Box
    {
        const Vector2 SIZE = { 100.0f, 70.0f };
        const float32 AREA_DENSITY = 0.03f;

        m_entityBox = EntityGetContext()->CreateEntity();

        // Transform
        {
            auto * transform = EnsureComponent<CTransformComponent2>(m_entityBox);
            transform->SetPosition({500, 200});
            transform->SetRotation(Radian(Degree(20.0f)));
        }

        // Collider
        {
            using namespace Physics;
            auto * collider = EnsureComponent<IColliderComponent>(m_entityBox, Aabb2(Point2(-0.5f * SIZE), Point2(0.5f * SIZE)));
        }

        // Graphics
        {
            using namespace Graphics;
            auto * primative = IImageComponent::Attach(m_entityBox, "Assets/Art/Objects/Box.png", SIZE);
        }

        // Rigid Body
        {
            using namespace Physics;
            auto * rigidBody = EnsureComponent<IRigidBodyComponent>(m_entityBox);
            rigidBody->SetMass(SIZE.x * SIZE.y * AREA_DENSITY);
        }
    }
}

//=============================================================================
CAppBouyant::~CAppBouyant ()
{
    EntityGetContext()->DestroyEntity(m_entityBox);
}

//=============================================================================
void CAppBouyant::OnPhysicsPreTick ()
{
    const float AIR_DENSITY   = 0.01f;
    const float WATER_DENSITY = 0.1f;

    //// drag
    //const float ballArea         = Math::Pi * Sq(m_ball.r);
    //const float ballCrossSection = (m_ball.r * 2);
    //const float ballDragCoeff    = 0.47f; // http://en.wikipedia.org/wiki/Drag_coefficient

    //const float   dragT       = displacement / ballArea; // Try to estimate the displaced density 
    //const float   dragDensity = Lerp(AIR_DENSITY, WATER_DENSITY, dragT);
    //const Vector2 dragForce   = -0.5f * dragDensity * Sq(m_ballVel) * ballCrossSection * ballDragCoeff; // http://en.wikipedia.org/wiki/Drag_equation

    //{
    //    SI::Acceleration gravity = SI::Acceleration(-10.0f);
    //    SI::Newton bouyancy = SI::Area(5.0f) * SI::AreaDensity(4.0f) * gravity;
    //    SI::Newton force = bouyancy;
    //    SI::Acceleration accel = bouyancy / SI::Kilogram(10.0f) + gravity;
    //    SI::Second dt = SI::Second(1.0f);
    //    SI::Velocity v = v + accel * dt;
    //    SI::Meter p = p + v * dt + SI::Unitless(0.5f) * accel * dt * dt;
    //}


    const Polygon2 waterPoly = m_water.GetPoints();

    {
        using namespace Physics;

        auto * transform = m_entityBox->Get<CTransformComponent2>();
        auto * rigidbody = m_entityBox->Get<IRigidBodyComponent>();
        auto * collider  = m_entityBox->Get<IColliderComponent>();
        
        // TODO: move these calcs into the physics system
        const auto poly = collider->GetPolygon();
        m_bouyantPoly = Polygon2::Clip(poly, waterPoly);

        Point2 centroid;
        float32 displacement;
        m_bouyantPoly.ComputeInfo(&centroid, &displacement);

        const Vector2 bouyancyForce = -displacement * WATER_DENSITY * Physics::GetContext()->GetGravity();
        rigidbody->AddForce(bouyancyForce, centroid);
        rigidbody->AddForce(-0.5 * 0.01f * Sq(rigidbody->GetVelocity()) * 15.0f * 0.4f);

        if (InputGetManager()->KeyIsDown(EKey::Left))
            rigidbody->AddForce({-100.0f, 0.0f});
        if (InputGetManager()->KeyIsDown(EKey::Right))
            rigidbody->AddForce({100.0f, 0.0f});
        if (InputGetManager()->KeyIsDown(EKey::Up))
            rigidbody->AddTorque(100000.0f);
        if (InputGetManager()->KeyIsDown(EKey::Down))
            rigidbody->AddTorque(-100000.0f);
    }
}

//=============================================================================
void CAppBouyant::Update ()
{

}

//=============================================================================
void CAppBouyant::Render ()
{
    Graphics::IRenderTarget * backbuffer = Graphics::GetContext()->Backbuffer();
    backbuffer->SetView(Matrix23::Identity);
    backbuffer->SetWorld(Matrix23::Identity);

    backbuffer->Line(m_bouyantPoly.points, Color::Magenta);
}