namespace Content
{

IComponent * MakeTransform (IEntity::Ref entity, const Json::CValue & data);
IComponent * MakeRigidBody (IEntity::Ref entity, const Json::CValue & data);
IComponent * MakeCollider (IEntity::Ref entity, const Json::CValue & data);
IComponent * MakeGraphics (IEntity::Ref entity, const Json::CValue & data);
IComponent * MakeTankControl (IEntity::Ref entity, const Json::CValue & data);
IComponent * MakeGunControl (IEntity::Ref entity, const Json::CValue & data);
IComponent * MakeScript (IEntity::Ref entity, const Json::CValue & data);

} // namespace Content