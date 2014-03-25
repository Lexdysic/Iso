
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
TArray<Point2> OverlappedPolygon (const TArray<Point2> & clipPoly, const TArray<Point2> & subjectPoly)
{
    // http://en.wikipedia.org/wiki/Sutherland%E2%80%93Hodgman_algorithm

    ASSERT(clipPoly.Count() >= 3);
    ASSERT(subjectPoly.Count() >= 3);

    TArray<Point2> outputList = subjectPoly;

    Point2 clipB = *clipPoly.Top();
    for (const auto & clipA : clipPoly)
    {
        const Plane2 clipPlane(clipA, clipB);

        TArray<Point2> inputList = std::move(outputList);

        Point2 subjectB = *inputList.Top();
        for (const auto & subjectA : inputList)
        {
            using namespace Geometry;

            const Line2 subjectLine(subjectA, subjectB);

            IntersectInfo2 info;
            if (Intersect(info, subjectLine, clipPlane))
                outputList.Add(info.point);

            if (Test(subjectA, clipPlane) == TestResult::Inside)
                outputList.Add(subjectA);

            subjectB = subjectA;
        }

        if (outputList.IsEmpty())
            break;

        clipB = clipA;
    }

    ASSERT(outputList.Count() >= 3);
    return outputList;
}

//=============================================================================
float32 ComputeArea (const TArray<Point2> & poly)
{
    float totalArea = 0.0f;

    using namespace Geometry;

    EWinding groupWinding = EWinding::Invalid;

    const Point2 & a = poly[0];
    for (uint i = 2; i < poly.Count(); ++i)
    {
        const Point2 & c = poly[i];
        const Point2 & b = poly[i-1];
        
        const Triangle2 tri(a, b, c);

        const EWinding winding = tri.ComputeWiding();
        ASSERT(winding != EWinding::Invalid);
        ASSERT(groupWinding == EWindow::Invalid || winding == groupWinding); // Can only handle convex polygons

        totalArea += tri.ComputeArea();
    }

    return totalArea;
}



//=============================================================================
//
// CAppBouyant
//
//=============================================================================

//=============================================================================
CAppBouyant::CAppBouyant ()
{
    m_water.min = {0, 400};
    m_water.max = {1000, 600};

    m_ball.center = {500, 300};
    m_ball.radius = 20.0;
    m_ballVel = Vector2::Zero;
    m_ballAccel = Vector2::Zero;

    const Radian angle = Radian(1.0f);
    const Vector2 v = Vector2(Cos(angle), Sin(angle)) * 10.0f ;
    m_box = Obb2({500, 395}, v, Perpendicular(v));
}

//=============================================================================
CAppBouyant::~CAppBouyant ()
{

}

//=============================================================================
void CAppBouyant::Update ()
{    
    const Vector2 GRAVITY       = Vector2::UnitY * 10.0f; // m/s^2
    const float   AIR_DRAG      = 0.001f;
    const float   AIR_DENSITY   = 0.01f;
    const float   WATER_DRAG    = 0.5f;
    const float   WATER_DENSITY = 2.0;
    const float   BALL_MASS     = 1000.0f;

    // bouyancy
    const float displacement = OverlappedArea(m_ball, m_water);
    const Vector2 bouyancyForce = -displacement * WATER_DENSITY * GRAVITY;

    // gravity
    const Vector2 gravityForce = BALL_MASS * GRAVITY;

    // drag
    const float ballArea         = Math::Pi * Sq(m_ball.r);
    const float ballCrossSection = (m_ball.r * 2);
    const float ballDragCoeff    = 0.47f; // http://en.wikipedia.org/wiki/Drag_coefficient

    const float   dragT       = displacement / ballArea; // Try to estimate the displaced density 
    const float   dragDensity = Lerp(AIR_DENSITY, WATER_DENSITY, dragT);
    const Vector2 dragForce   = -0.5f * dragDensity * Sq(m_ballVel) * ballCrossSection * ballDragCoeff; // http://en.wikipedia.org/wiki/Drag_equation
    
    // Itegrate
    
    const float dt = Time::GetGameDelta().GetSeconds();
    const Vector2 totalForce = gravityForce + bouyancyForce + dragForce;
    m_ballAccel = totalForce / BALL_MASS;
    m_ballVel += m_ballAccel * dt;
    m_ball.center += m_ballVel * dt + m_ballAccel * Sq(dt);
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

        const Matrix23 boxWorld = m_box.GetMatrix();
        backbuffer->SetWorld(boxWorld);
        backbuffer->Rectangle({-1, -1}, {1, 1}, Color::White);

        backbuffer->SetWorld(Matrix23::Identity);
        auto boxPoints = m_box.GetPoints();
        auto waterPoints = m_water.GetPoints();

        auto clippedPoints = OverlappedPolygon(waterPoints, boxPoints);

        auto clippedArea = ComputeArea(clippedPoints);

        backbuffer->Line(clippedPoints, Color::Magenta, 1.0f, Graphics::ELoop::Closed);
    }
}

//=============================================================================
float CAppBouyant::ComputeDisplacement()
{
    return 0.0f;
}