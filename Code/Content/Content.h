
#include "Systems/Entity.h"
#include "CnContent.h"

namespace Content
{

interface IContext
{
    virtual void Initialize () pure;
    virtual void Uninitialize () pure;

    virtual IEntity::Ptr CreateEntity (const CString & name) pure;
};

IContext * GetContext();

} // namespace Content