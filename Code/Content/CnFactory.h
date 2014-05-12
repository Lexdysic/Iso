namespace Content
{

IComponent * MakeTransform (const Json::CValue & data);
IComponent * MakeRigidBody (const Json::CValue & data);
IComponent * MakeCollider (const Json::CValue & data);
IComponent * MakeGraphics (const Json::CValue & data);
IComponent * MakeTankControl (const Json::CValue & data);
IComponent * MakeGunControl (const Json::CValue & data);
IComponent * MakeScript (const Json::CValue & data);

} // namespace Content