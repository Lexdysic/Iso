
namespace Content
{

class CContext :
    public IContext
{
public:

    CContext ();
    ~CContext ();

public:

    static CContext s_context;
    static CContext * Get () { return &s_context; }

private:

    void Initialize () override;
    void Uninitialize () override;

    IEntity * CreateEntity (const CString & name) override;

private:

    TDictionary<CString, Json::CValue> m_objects;
    Json::CValue m_data;
};

} // namespace Content