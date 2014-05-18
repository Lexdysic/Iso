#include "CnPch.h"

using namespace Json;


namespace Content
{

//=============================================================================
IComponent * MakeTransform (IEntity::Ref entity, const Json::CValue & data)
{
    ERROR_CONTEXT("MakeTransform");

    if (data.GetType() != EType::Object)
        return null;

    auto * transform = CTransformComponent2::Attach(entity);

    if (const CValue & value = data[{"position"}])
    {
        const Point2 & pos = GetPoint2(value);
        transform->SetPosition(pos);
    }

    if (const CValue & value = data[{"rotation"}])
    {
        const Degree rot = Degree(GetFloat32(value));
        transform->SetRotation(Radian(rot));
    }

    return transform;
}

//=============================================================================
IComponent * MakeRigidBody (IEntity::Ref entity, const Json::CValue & data)
{
    ERROR_CONTEXT("MakeRigidBody");

    if (data.GetType() != EType::Object)
        return null;

    auto * rigidbody = Physics::IRigidBodyComponent::Attach(entity);

    if (const CValue & value = data[{"mass"}])
    {
        const float32 mass = GetFloat32(value);
        rigidbody->SetMass(mass);
    }

    return rigidbody;
}

//=============================================================================
IComponent * MakeCollider (IEntity::Ref entity, const Json::CValue & data)
{
    ERROR_CONTEXT("MakeCollider");

    if (data.GetType() != EType::Object)
        return null;

    // Position
    Point2 pos = Point2::Zero;
    if (const CValue & value = data[{"position"}])
        pos = GetPoint2(value);

    const CValue & type = data[{"type"}];
    if (!type)
    {
        return null; // TODO: warning
    }
    else if (type == "circle")
    {
        const CValue & radius = data[{"size"}];
        if (!radius)
            return null;

        Circle circle(pos, GetFloat32(radius));
        return Physics::IColliderComponent::Attach(entity, circle);
    }
    else if (type == "box")
    {
        const CValue & size = data[{"size"}];
        if (!size)
            return null;

        Aabb2 box(pos, GetVector2(size) * 0.5f);
        return Physics::IColliderComponent::Attach(entity, box);
    }

    return null;
}

//=============================================================================
IComponent * MakeGraphics (IEntity::Ref entity, const Json::CValue & data)
{
    ERROR_CONTEXT("MakeGraphics");

    if (data.GetType() != EType::Object)
    {
        DebugError(EErrorLevel::Error, "Must have object defition.");
        return null;
    }

    // Position
    Point2 pos = Point2::Zero;
    if (const CValue & value = data[{"position"}])
        pos = GetPoint2(value);

    const CValue & type = data[{"type"}];
    if (!type)
    {
        DebugError(EErrorLevel::Error, "Must specify a type.");
        return null;
    }
    else if (type == "circle")
    {
        const CValue & radius = data[{"size"}];
        if (!radius)
        {
            DebugError(EErrorLevel::Error, "Must specify size.");
            return null;
        }

        return Graphics::IPrimativeComponent::Attach(entity, GetFloat32(radius));
    }
    else if (type == "box")
    {
        const CValue & size = data[{"size"}];
        if (!size)
        {
            DebugError(EErrorLevel::Error, "Must specify size.");
            return null;
        }

        return Graphics::IPrimativeComponent::Attach(entity, GetVector2(size) * 0.5f);
    }
    else if (type == "image")
    {
        const CValue & pathValue = data[{"filename"}];
        if (!pathValue)
        {
            DebugError(EErrorLevel::Error, "Must specify filename.");
            return null;
        }
        CPath path = GetPath(pathValue);
        
        Vector2 size = Vector2::Zero;
        if (const CValue & value = data[{"size"}])
            size = GetVector2(value);

        return Graphics::IImageComponent::Attach(entity, path, size);
    }

    return null;
}

//=============================================================================
IComponent * MakeTankControl (IEntity::Ref entity, const Json::CValue & data)
{
    ERROR_CONTEXT("MakeTankControl");

    return null;
}

//=============================================================================
IComponent * MakeGunControl (IEntity::Ref entity, const Json::CValue & data)
{
    ERROR_CONTEXT("MakeGunControl");

    return null;
}

//=============================================================================
IComponent * MakeScript (IEntity::Ref entity, const Json::CValue & data)
{
    ERROR_CONTEXT("MakeScript");

    return null;
}


} // namespace Content