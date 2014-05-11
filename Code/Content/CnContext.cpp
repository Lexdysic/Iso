
#include "CnPch.h"

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
                "Error::Content Could not parse file \"%s\" at (%u:%u)\n...%s",
                fullpath.GetString().Ptr(),
                error.line,
                error.column,
                error.message.Ptr()
            );
        }
    }
}

//=============================================================================
void CContext::Uninitialize ()
{
    m_data = null;
}

//=============================================================================
IEntity * CContext::CreateEntity (const CString & name)
{
    return null;
}




//=============================================================================
IContext * GetContext ()
{
    return CContext::Get();
}

} // namespace Content