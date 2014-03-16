namespace WorldView
{

class CCamera :
    public ICamera
{
public:
    CCamera ();
    ~CCamera ();

    void Update ();
    void SetSize (const Vector2 & size) { m_size = size; }

public:

    Point2 Project (const Point2 & worldPos) const override { return m_worldToScreen * worldPos;}
    Point2 Unproject (const Point2 & screenPos) const override { return m_screenToWorld * screenPos;}
    const Matrix23 & GetWorldToScreen () const override { return m_worldToScreen; }
    const Matrix23 & GetScreenToWorld () const override { return m_screenToWorld; }

    void Move (const Vector2 & delta) override;
    void Zoom (float32 factor) override;
    float32 GetZoom () const override { return m_zoom; }

private:
    Point2      m_pos;
    float32     m_zoom;
    Vector2     m_size;
    Matrix23    m_worldToScreen;
    Matrix23    m_screenToWorld;
};

} // namespace WorldView