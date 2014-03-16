

struct TestNode :
    Pathing::IData
{
    TestNode (const Point2 & p) : pos(p) {}

    Point2 pos;

    static float GetDistance (Pathing::INode * a, Pathing::INode * b);
    static float GetHeuristic (Pathing::INode * a, Pathing::INode * b);
};


class CAppPathing :
    public IApp,
    public Pathing::CContextNotify
{
public:

    CAppPathing ();
    ~CAppPathing ();

public: // IApp

    void OnWindowKeyDown (EKey key) override;
    void OnWindowMouseDown (EMouse button, const Point2s & pos) override;

    void Render () override;
    void Update () override;

public: // Pathing::CContextNotify

    void OnPathingQueryFinished (Pathing::IQuery * query) override;

private:

    typedef TArray<Point2> Path;

    // Data
    Pathing::IGraph *  m_graph;
    Pathing::IQuery *  m_query;
    Pathing::INode *   m_nodeStart;
    Pathing::INode *   m_nodeEnd;
    TArray<Aabb2>      m_boxes;
    Path               m_pathOriginal;
    Path               m_pathSmooth;
    Path               m_pathOptimized;

    LinearPath2        m_pathFollow;
    IEntity *          m_entity;

    // Helpers
    bool Blocked (const Point2 & pos) const;
    bool Blocked (const Segment2 & line) const;
    bool AddEdge (Pathing::INode * a, Pathing::INode * b);
    void DrawPath (const Path & path, const Color & color);
    void Optimize (Path * path);
    void Smooth (Path * path);
};