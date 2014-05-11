
#include "CnContent.h"

namespace Content
{

interface IContext
{
    virtual void Initialize () pure;
    virtual void Uninitialize () pure;

    virtual IEntity * CreateEntity (const CString & name) pure;
};

IContext * GetContext();

} // namespace Content