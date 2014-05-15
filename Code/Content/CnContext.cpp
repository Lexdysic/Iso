
#include "CnPch.h"

using namespace Json;

namespace Content
{

//=============================================================================
CContext CContext::s_context;

//=============================================================================
CContext::CContext ()
{
}

//=============================================================================
CContext::~CContext ()
{
}

//=============================================================================
void CContext::Initialize ()
{
    ReadContentFiles();
    SetupFactories();

    IEntity::Ptr test = CreateEntity("Bullet");
}

//=============================================================================
void CContext::Uninitialize ()
{
    m_objects.Clear();
}

//=============================================================================
IEntity::Ptr CContext::CreateEntity (const CString & name)
{
    const auto * def = m_objects.Find(name);
    if (!def)
        return null;

    const auto * componentsValue = def->Find("Components");
    if (!componentsValue || componentsValue->GetType() != EType::Object)
        return null;
    
    IEntity::Ptr entity = EntityGetContext()->CreateEntity();

    const auto & components = *componentsValue->As<ObjectType>();
    for (auto kv : components)
    {
        const auto & name = kv.first;
        const auto & data = kv.second;

        FFactory createComponent = m_factories.Find(name);
        if (!createComponent)
            continue; // TODO: show warning

        IComponent * component = createComponent(entity, data);
        if (!component)
            continue; // TODO: show warning
    }

    return entity;
}

//=============================================================================
void CContext::SetupFactories ()
{
    m_factories.Set("Transform",    MakeTransform);
    m_factories.Set("RigidBody",    MakeRigidBody);
    //m_factories.Set("Collider",     MakeCollider);
    //m_factories.Set("Graphics",     MakeGraphics);
    //m_factories.Set("TankControl",  MakeTankControl);
    //m_factories.Set("GunControl",   MakeGunControl);
    //m_factories.Set("Script",       MakeScript);
}

//=============================================================================
void CContext::ReadContentFiles()
{
    const CPath basePath("P:\\Projects\\Iso\\Assets\\Entities\\");

    for (auto filename : Path::CListing(basePath))
    {
        const CPath fullpath = basePath + filename;

        Json::CDocument doc;
        doc.Parse(fullpath);
        if (!doc.IsValid())
        {
            const auto error = doc.GetError();
            DebugMsg(
                "Error::Content] Could not parse file \"%s\" at (%u:%u)\n...%s",
                fullpath.GetString().Ptr(),
                error.line,
                error.column,
                error.message.Ptr()
            );
            continue;
        }

        // Has name?
        const auto & object = doc.GetValue();
        const auto * nameValue = object.Find("Name");
        if (!nameValue)
        {
            DebugMsg(
                "Error::Content] Object in file \"%s\" does not have a name",
                fullpath.GetString().Ptr()
            );
            continue;
        }

        // Name is string?
        const auto * name = nameValue->As<StringType>();
        if (!name)
        {
            DebugMsg(
                "Error::Content] Objects name field must be a string; file \"%s\"",
                fullpath.GetString().Ptr()
            );
            continue;
        }

        m_objects.Set(*name, object);
    }
}




//=============================================================================
IContext * GetContext ()
{
    return CContext::Get();
}

} // namespace Content