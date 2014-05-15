
#include "CnPch.h"

using namespace Json;

namespace Content
{

float32 GetFloat32 (const CValue & value)
{
    if (value == null)
        return 0.0f;

    if (value.GetType() != EType::Number)
        return 0.0f;

    return float32(*value.As<NumberType>());
}

Point2 GetPoint2 (const CValue & value)
{
    if (value == null)
        return Point2::Zero;

    if (value.Count() != 2)
    {
        DebugMsg("Content::Warning] Point2 properties must have exactly two elements.");
        return Point2::Zero;
    }

    Point2 pos;
    if (value.GetType() == EType::Array)
    {
        pos.x = GetFloat32(value[0]);
        pos.y = GetFloat32(value[1]);
    }
    else if (value.GetType() == EType::Object)
    {
        pos.x = GetFloat32(value[{"x"}]);
        pos.y = GetFloat32(value[{"y"}]);
    }
    else
    {
        DebugMsg("Content::Warning] Tranform with position property has unreadable type.");
    }

    return pos;
}

} // namespace Content