
#include "CnPch.h"

using namespace Json;

namespace Content
{
    
//=============================================================================
float32 GetFloat32 (const CValue & value)
{
    ERROR_CONTEXT("GetFloat32");

    if (value.GetType() != EType::Number)
    {
        DebugError(EErrorLevel::Warning, "Must be a number");
        return 0.0f;
    }

    return float32(*value.As<NumberType>());
}

//=============================================================================
Point2 GetPoint2 (const CValue & value)
{
    ERROR_CONTEXT("GetPoint2");

    if (value == null)
    {
        DebugError(EErrorLevel::Warning, "Cannot be null");
        return Point2::Zero;
    }

    if (value.Count() != 2)
    {
        DebugError(EErrorLevel::Warning, "Must have exactly two elements.");
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
        DebugError(EErrorLevel::Warning, "Property type is unreadable.");
    }

    return pos;
}

//=============================================================================
Vector2 GetVector2 (const CValue & value)
{
    ERROR_CONTEXT("GetVector2");

    if (value == null)
    {
        DebugError(EErrorLevel::Warning, "Cannot be null");
        return Point2::Zero;
    }

    if (value.Count() != 2)
    {
        DebugError(EErrorLevel::Warning, "Must have exactly two elements.");
        return Point2::Zero;
    }

    Vector2 vec;
    if (value.GetType() == EType::Array)
    {
        vec.x = GetFloat32(value[0]);
        vec.y = GetFloat32(value[1]);
    }
    else if (value.GetType() == EType::Object)
    {
        vec.x = GetFloat32(value[{"x"}]);
        vec.y = GetFloat32(value[{"y"}]);
    }
    else
    {
        DebugError(EErrorLevel::Warning, "Property type is unreadable.");
    }

    return vec;
}

CPath GetPath (const Json::CValue & value)
{
    ERROR_CONTEXT("GetPath");

    if (value.GetType() != EType::String)
    {
        DebugError(EErrorLevel::Warning, "Cannot be null");
        return CPath();
    }

    const CString & str = *value.As<StringType>();
    const CPath path(str);
    return path;
}

} // namespace Content