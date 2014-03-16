namespace WorldView
{

class CContext :
    public IContext
{
public:

    static CContext s_context;
    static CContext * Get () { return &s_context; }

public:
    ICamera * GetCamera () override { return &m_camera; }
    void Update (float32 elapsedTime) override;
    void SetScreenSize (const Vector2 & size) override;
private:

    CCamera m_camera;
};

} // namespace WorldView