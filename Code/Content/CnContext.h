
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

    IEntity::Ptr CreateEntity (const CString & name) override;

private:
    typedef IComponent * (*FFactory)(IEntity::Ref entity, const Json::CValue & data);

    TDictionary<CString, Json::ObjectType> m_objects;
    TDictionary<CString, FFactory> m_factories;

    void SetupFactories ();
    void ReadContentFiles ();
};

} // namespace Content