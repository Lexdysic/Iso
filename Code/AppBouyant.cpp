
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
Polygon2 OverlappedPolygon (const Polygon2 & clipPoly, const Polygon2 & subjectPoly)
{
    // http://en.wikipedia.org/wiki/Sutherland%E2%80%93Hodgman_algorithm

    ASSERT(clipPoly.points.Count() >= 3);
    ASSERT(subjectPoly.points.Count() >= 3);

    Polygon2 outputList = subjectPoly;

    Point2 clipB = *clipPoly.points.Top();
    for (const auto & clipA : clipPoly.points)
    {
        const Plane2 clipPlane(clipA, clipB);

        Polygon2 inputList;
        inputList.points = std::move(outputList.points);

        Point2 subjectB = *inputList.points.Top();
        for (const auto & subjectA : inputList.points)
        {
            using namespace Geometry;

            const Line2 subjectLine(subjectA, subjectB);

            IntersectInfo2 info;
            if (Intersect(info, subjectLine, clipPlane))
                outputList.points.Add(info.point);

            if (Test(subjectA, clipPlane) == TestResult::Inside)
                outputList.points.Add(subjectA);

            subjectB = subjectA;
        }

        if (outputList.points.IsEmpty())
            break;

        clipB = clipA;
    }

    return outputList;
}



//=============================================================================
//
// CAppBouyant
//
//=============================================================================

//=============================================================================
CAppBouyant::CAppBouyant ()
{
    m_water.min = { 0, 400 };
    m_water.max = { 1000, 600 };

    m_ball.center = { 500, 300 };
    m_ball.radius = 20.0;
    m_ballVel     = Vector2::Zero;
    m_ballAccel   = Vector2::Zero;

    const Radian angle = Radian(Degree(10.0f));
    const Vector2 v = Vector2(Cos(angle), Sin(angle)) * 10.0f ;
    m_box = Obb2({500, 390}, v, Perpendicular(v));


    {
        const Vector2 SIZE = { 50.0f, 20.0f };

        m_entity = EntityGetContext()->CreateEntity();

        // Transform
        {
            auto * transform = EnsureComponent<CTransformComponent2>(m_entity);
            transform->SetPosition({500, 300});
            transform->SetRotation(Radian(Degree(20.0f)));
        }

        // Rigid Body
        {
            using namespace Physics;
            auto * rigidBody = EnsureComponent<IRigidBodyComponent>(m_entity);
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
CAppBouyant::~CAppBouyant ()
{
    EntityGetContext()->DestroyEntity(m_entity);
}

//=============================================================================
void CAppBouyant::Update ()
{
    const float   AIR_DRAG      = 0.001f;
    const float   AIR_DENSITY   = 0.01f;
    const float   WATER_DRAG    = 0.5f;
    const float   WATER_DENSITY = 0.01;

    //// drag
    //const float ballArea         = Math::Pi * Sq(m_ball.r);
    //const float ballCrossSection = (m_ball.r * 2);
    //const float ballDragCoeff    = 0.47f; // http://en.wikipedia.org/wiki/Drag_coefficient

    //const float   dragT       = displacement / ballArea; // Try to estimate the displaced density 
    //const float   dragDensity = Lerp(AIR_DENSITY, WATER_DENSITY, dragT);
    //const Vector2 dragForce   = -0.5f * dragDensity * Sq(m_ballVel) * ballCrossSection * ballDragCoeff; // http://en.wikipedia.org/wiki/Drag_equation




    const Polygon2 waterPoly = m_water.GetPoints();

    {
        using namespace Physics;

        auto * transform = m_entity->Get<CTransformComponent2>();
        auto * rigidbody = m_entity->Get<IRigidBodyComponent>();
        auto * collider  = m_entity->Get<IColliderComponent>();

        const auto poly = collider->GetPolygon();
        const auto clippedPoly = OverlappedPolygon(waterPoly, poly);

        Point2 centroid;
        float32 displacement;
        clippedPoly.ComputeInfo(&centroid, &displacement);

        const Vector2 bouyancyForce = -displacement * WATER_DENSITY * Physics::GetContext()->GetGravity();
        rigidbody->AddForce(bouyancyForce, centroid);
    }
}

//=============================================================================
void CAppBouyant::Render ()
{
    Graphics::IRenderTarget * backbuffer = Graphics::GetContext()->Backbuffer();
    backbuffer->SetView(Matrix23::Identity);
    backbuffer->SetWorld(Matrix23::Identity);

    // Water
    {
        backbuffer->Rectangle(m_water.min, m_water.max, Color::Blue);
    }

    // Ball
    {
        //backbuffer->Circle(m_ball.center, m_ball.radius, Color::Red);
    }

    // Box
    {

        //const Matrix23 boxWorld = m_box.GetMatrix();
        //backbuffer->SetWorld(boxWorld);
        //backbuffer->Rectangle({-1, -1}, {1, 1}, Color::White);

        //backbuffer->SetWorld(Matrix23::Identity);
        //auto boxPoints = m_box.GetPoints();
        //auto waterPoints = m_water.GetPoints();

        //auto clippedPoly = OverlappedPolygon(waterPoints, boxPoints);

        //Point2 centroid;
        //float32 area;
        //clippedPoly.ComputeInfo(&centroid, &area);

        //backbuffer->Line(clippedPoly.points, Color::Magenta, 1.0f, Graphics::ELoop::Closed);

        //backbuffer->Circle(centroid, 0.5f, Color::Aqua);
        //backbuffer->Circle(m_box.center, 0.5f, Color::Black);
    }
}