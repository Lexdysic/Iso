#include "Basics/Time/Time.h"

namespace WorldView
{

interface ICamera
{
    virtual Point2 Project (const Point2 & worldPos) const pure;
    virtual Point2 Unproject (const Point2 & screenPos) const pure;
    virtual const Matrix23 & GetWorldToScreen () const pure;
    virtual const Matrix23 & GetScreenToWorld () const pure;

    virtual void Move (const Vector2 & delta) pure;
    virtual void Zoom (float32 factor) pure;
    virtual float32 GetZoom () const pure;
};

interface IContext
{
    virtual ICamera * GetCamera () pure;

    virtual void Update (const Time::Delta deltaTime) pure;
    
    virtual void SetScreenSize (const Vector2 & size) pure;
};

IContext * GetContext ();

} // namespace WorldView