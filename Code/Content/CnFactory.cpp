#include "CnPch.h"

using namespace Json;


namespace Content
{

//=============================================================================
IComponent * MakeTransform (IEntity::Ref entity, const Json::CValue & data)
{
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
    return null;
}

//=============================================================================
IComponent * MakeGraphics (IEntity::Ref entity, const Json::CValue & data)
{
    return null;
}

//=============================================================================
IComponent * MakeTankControl (IEntity::Ref entity, const Json::CValue & data)
{
    return null;
}

//=============================================================================
IComponent * MakeGunControl (IEntity::Ref entity, const Json::CValue & data)
{
    return null;
}

//=============================================================================
IComponent * MakeScript (IEntity::Ref entity, const Json::CValue & data)
{
    return null;
}


} // namespace Content