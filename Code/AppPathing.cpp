
#include "AppPch.h"


//=============================================================================
//
// Constants
//
//=============================================================================

const float32 NODE_RADIUS = 5.0f;


//=============================================================================
//
// Helpers
//
//=============================================================================

//=============================================================================
bool IsColinear (const Point2 & a, const Point2 & b, const Point2 & c)
{
    const float32 TOLERANCE = 0.01f;

    const Vector2 & ab = Normalize(b - a);
    const Vector2 & bc = Normalize(c - b);

    return Abs(Dot(ab, bc)) > 1.0f - TOLERANCE;
}

//=============================================================================
bool IsDistanceNegligable (const Point2 & a, const Point2 & b, const Point2 & c)
{
    const float32 TOLERANCE = 20.0f;

    const float32 distAB = Distance(a, b);
    const float32 distBC = Distance(b, c);
    const float32 distAC = Distance(a, c);

    return distAB + distBC - distAC < TOLERANCE;
}


//=============================================================================
//
// TestNode
//
//=============================================================================

//=============================================================================
float TestNode::GetDistance (Pathing::INode * a, Pathing::INode * b)
{
    TestNode * aData = (TestNode *)a->GetData();
    TestNode * bData = (TestNode *)b->GetData();

    return Distance(aData->pos, bData->pos);
}

//=============================================================================
float TestNode::GetHeuristic (Pathing::INode * a, Pathing::INode * b)
{
    TestNode * aData = (TestNode *)a->GetData();
    TestNode * bData = (TestNode *)b->GetData();

    return Distance(aData->pos, bData->pos);
}



//=============================================================================
//
// CAppPathing
//
//=============================================================================

//=============================================================================
CAppPathing::CAppPathing () :
    m_nodeStart(null),
    m_nodeEnd(null),
    m_query(null)
{
    Random rand;

    // Create blockers
    for (uint i = 0; i < 50; ++i)
    {
        const Point2 pos(
            rand.Range(0.0f, 1200.0f),
            rand.Range(0.0f, 1000.0f)
        );

        const float s = rand.Range(25.0f, 50.0f);
        const Vector2 size(s, s);

        m_boxes.Add(Aabb2(pos, size));
    }

    // Create Graph
    using namespace Pathing;

    m_graph = GetContext()->GraphCreate();


    const float   SIZE = 30.0f;
    const Vector2 OFFSET(SIZE, SIZE);
    const float   HALF_SIZE = SIZE * 0.5f;
    const uint    WIDTH = 50;
    const uint    HEIGHT = 30;

    INode * nodes[WIDTH][HEIGHT];
    for (uint y = 0; y < HEIGHT; ++y)
    {
        for (uint x = 0; x < WIDTH; ++x)
        {
            const Point2 point(
                x * SIZE + rand.Range(-HALF_SIZE, HALF_SIZE) + OFFSET.y,
                y * SIZE + rand.Range(-HALF_SIZE, HALF_SIZE) + OFFSET.x
            );

            INode * node = Blocked(point) ? null : m_graph->NodeAdd(new TestNode(point));
            nodes[x][y] = node;
        }
    }

    for (uint y = 1; y < HEIGHT; ++y)
    {
        for (uint x = 1; x < WIDTH; ++x)
        {
            INode * nodeA = nodes[x][y];
            INode * nodeB = nodes[x-1][y];
            INode * nodeC = nodes[x][y-1];

            AddEdge(nodeA, nodeB);
            AddEdge(nodeA, nodeC);
        }
    }

    for (uint x = 1; x < WIDTH; ++x)
    {
        INode * nodeA = nodes[x][0];
        INode * nodeB = nodes[x-1][0];
        AddEdge(nodeA, nodeB);
    }
        
    for (uint y = 1; y < HEIGHT; ++y)
    {
        INode * nodeA = nodes[0][y];
        INode * nodeB = nodes[0][y-1];
        AddEdge(nodeA, nodeB);
    }

    Pathing::GetContext()->NotifyAdd(this);


    m_entity = EntityGetContext()->CreateEntity();
    EnsureComponent<Physics::IRigidBodyComponent>(m_entity);
}

//=============================================================================
CAppPathing::~CAppPathing ()
{
    Pathing::GetContext()->GraphDestroy(m_graph);
    Pathing::GetContext()->QueryDestroy(m_query);
}

//=============================================================================
void CAppPathing::Update ()
{
    if (m_pathFollow.IsValid())
    {
        using namespace Physics;

        const float32 SPEED = 100.0f;
        const float32 LOOK_AHEAD = 100.0f;

        Physics::IRigidBodyComponent * rigidBody = m_entity->Get<IRigidBodyComponent>();
        CTransformComponent2 * transform = m_entity->Get<CTransformComponent2>();

        const float32 pathDist = m_pathFollow.Closest(transform->GetPosition());
        const Point2 & lookAhead = m_pathFollow.Eval(pathDist + LOOK_AHEAD);

        float dirLength;
        const Vector2 & direction = Normalize(lookAhead - transform->GetPosition(), &dirLength);

        rigidBody->SetVelocity(direction * SPEED * (dirLength / LOOK_AHEAD));
    }
}

//=============================================================================
void CAppPathing::Render ()
{
    Graphics::IRenderTarget * backbuffer = Graphics::GetContext()->Backbuffer();
    backbuffer->SetView(Matrix23::Identity);

    for (const Aabb2 & box : m_boxes)
    {
        backbuffer->Rectangle(
            box.min,
            box.max,
            Color(1.0f, 0.0f, 0.0f, 1.0f),
            Graphics::EDrawStyle::Outline
        );
    }

    using namespace Pathing;

    const TArray<INode *> & nodes = m_graph->GetNodes();
    for (INode * node : nodes)
    {
        TestNode * nodeData = node->GetData<TestNode>();

        const TArray<INode *> & neighbors = node->GetNeighbors();
        for (INode * neighbor : neighbors)
        {
            TestNode * neighborData = neighbor->GetData<TestNode>();
            backbuffer->Line(
                nodeData->pos,
                neighborData->pos,
                Color(0.3f, 0.4f, 0.8f)
            );
        }
    }

    DrawPath(m_pathOriginal, Color(0.4f, 1.0f, 0.5f, 0.5f));
    DrawPath(m_pathSmooth, Color(1.0f, 0.85f, 0.0f, 0.5f));
    DrawPath(m_pathOptimized, Color(1.0f, 1.0f, 1.0f));

    if (m_pathFollow.IsValid())
    {
        using namespace Physics;
        Physics::IRigidBodyComponent * rigidBody = m_entity->Get<IRigidBodyComponent>();
        CTransformComponent2 * transform = m_entity->Get<CTransformComponent2>();

        const Point2 & position = transform->GetPosition();
        const Vector2 & velocity = rigidBody->GetVelocity();

        const Vector2 SIZE = Vector2::One * 5.0f;

        backbuffer->Rectangle(
            position - SIZE,
            position + SIZE,
            Color(0.0f, 1.0f, 0.0f)
        );
        backbuffer->Arrow(
            position, 
            position + velocity,
            Color(0.0f, 1.0f, 0.0f)
        );
    }
}

//=============================================================================
void CAppPathing::OnWindowKeyDown (EKey key)
{

}

//=============================================================================
void CAppPathing::OnWindowMouseDown (EMouse button, const Point2s & pos)
{
    WorldView::ICamera * camera = WorldView::GetContext()->GetCamera();

    const Point2 cursorPos((float32)pos.x, (float32)pos.y);

    Pathing::INode * nodeBest = null;
    float            distSqBest = Math::Infinity;

    const TArray<Pathing::INode *> & nodes = m_graph->GetNodes();
    for (Pathing::INode * node : nodes)
    {
        TestNode * nodeData = node->GetData<TestNode>();
        const float distSq = DistanceSq(nodeData->pos, cursorPos);
        if (distSq < distSqBest)
        {
            nodeBest = node;
            distSqBest = distSq;
        }
    }

    if (!nodeBest)
        return;

    if (button == EMouse::Primary)
        m_nodeStart = nodeBest;
    else if (button == EMouse::Secondary)
        m_nodeEnd = nodeBest;

    // Clear the old query and start a new one
    if (m_query)
        Pathing::GetContext()->QueryDestroy(m_query);

    if (m_nodeStart && m_nodeEnd)
    {
        m_query = Pathing::GetContext()->QueryCreate(
            m_graph,
            m_nodeStart,
            m_nodeEnd,
            TestNode::GetDistance,
            TestNode::GetHeuristic
        );
    }
}

//=============================================================================
void CAppPathing::OnPathingQueryFinished (Pathing::IQuery * query)
{
    if (m_query != query)
        return;

    const TArray<Pathing::INode *> & nodePath = query->GetPath();

    m_pathOriginal.Clear();
    m_pathOriginal.Reserve(nodePath.Count());

    for (Pathing::INode * node : nodePath)
        m_pathOriginal.Add(node->GetData<TestNode>()->pos);

    m_pathSmooth = m_pathOriginal;
    Smooth(&m_pathSmooth);

    m_pathOptimized = m_pathSmooth;
    Optimize(&m_pathOptimized);

    // Reset the agent
    m_pathFollow.Set(m_pathSmooth);

    CTransformComponent2 * transform = m_entity->Get<CTransformComponent2>();
    transform->SetPosition(m_pathFollow.Eval(0.0f));
}

//=============================================================================
bool CAppPathing::Blocked (const Point2 & pos) const
{
    for (const Aabb2 & box : m_boxes)
    {
        if (Contains(box, pos))
            return true;
    }

    return false;
}

//=============================================================================
bool CAppPathing::Blocked (const Segment2 & line) const
{
    for (const Aabb2 & box : m_boxes)
    {
        if (Overlap(box, line))
            return true;
    }

    return false;
}

//=============================================================================
bool CAppPathing::AddEdge (Pathing::INode * a, Pathing::INode * b)
{
    if (!a || !b)
        return false;

    TestNode * aData = a->GetData<TestNode>();
    TestNode * bData = b->GetData<TestNode>();
    Segment2 line(aData->pos, bData->pos);

    if (Blocked(line))
        return false;

    m_graph->EdgeAddSymetric(a, b);

    return false;
}

//=============================================================================
void CAppPathing::DrawPath (const Path & path, const Color & color)
{
    Graphics::IRenderTarget * backbuffer = Graphics::GetContext()->Backbuffer();

    const Point2 * prev = path.Ptr();
    const Point2 * curr = prev + 1;
    const Point2 * term = path.Term();
    for ( ; curr < term; prev = curr, ++curr)
    {
        backbuffer->Arrow(
            *prev,
            *curr,
            color,
            2.0f
        );
    }
}

//=============================================================================
void CAppPathing::Smooth (Path * path)
{
    TArray<Point2> adjustedPath;

    bool isFinished;
    do
    {
        isFinished = true;

        adjustedPath = *path;

        Point2 * readPrev = path->Ptr() + 0;
        Point2 * readCurr = path->Ptr() + 1;
        Point2 * readNext = path->Ptr() + 2;
        Point2 * readTerm = path->Term();
        Point2 * writeCurr = adjustedPath.Ptr() + 1;
        for ( ; readNext < readTerm; readPrev = readCurr, readCurr = readNext, readNext++, writeCurr++)
        {
            Point2 closest = Closest(*readCurr, Segment2(*readPrev, *readNext));
            Point2 newPos = Lerp(*readCurr, closest, 0.5f);

            if (DistanceSq(newPos, *readCurr) < Sq(4.0f))
                continue;

            if (Blocked(Segment2(*readPrev, newPos)))
                continue;

            if (Blocked(Segment2(*readNext, newPos)))
                continue;

            isFinished = false;
            *writeCurr = newPos;
        }

        *path = adjustedPath;
    }
    while (!isFinished);
}

//=============================================================================
void CAppPathing::Optimize (Path * path)
{
    ASSERT(path);

    bool isFinished;
    do
    {
        isFinished = true;

        Point2 * prev = path->Ptr();
        Point2 * curr = path->Ptr() + 1;
        Point2 * next = path->Ptr() + 2;
        Point2 * term = path->Term();
        for ( ; next < term; prev = curr, curr = next, next++)
        {
            const Point2 & a = *prev;
            const Point2 & b = *curr;
            const Point2 & c = *next;

            Segment2 lineAC(a, c);
            if (Blocked(lineAC))
                continue;

            if (IsDistanceNegligable(a, b, c) || IsColinear(a, b, c))
            {
                const uint index = path->Index(curr);
                path->RemoveOrdered(index);
                isFinished = false;
                break;
            }
        }
    }
    while (!isFinished);
}
