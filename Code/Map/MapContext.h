
namespace Map
{



class CContext :
    public IContext
{
public:

    static CContext s_context;
    static CContext * Get () { return &s_context; }

public:

    CContext ();
    ~CContext ();

    TileSet * FindTileSet (const Content::TileSetDef * tileset);

private:

    void Initialize () override;
    void Uninitialize () override;
    
    void Generate () override;

    void Render () override;

    Point2s Pick (const Point2 & worldPos) override;

    void DrawOutline (const Point2s & cellPos) override;

    ICell * GetCell (const Point2s & cellPos) override;

    bool CellIsValid (const Point2s & cellPos) override;
    void CellRotate (const Point2s & tilePos) override;
    void CellSetTileSet (const Point2s & cellPos, const Content::TileSetDef * tileSet) override;
    void CellSetHeight (const Point2s & cellPos, float32 height) override;


private:



    Random              m_random;
    TArray<TileSet>     m_tileSets;
    TArray<Cell>        m_cells;
    Matrix22            m_tileToIso;
    Matrix22            m_isoToTile;
    float32             m_tileThickness;

    Cell * FindCell (const Point2s & tilePos);
    
    void DrawCell (const Cell & cell);
    void SetTileSize (float32 width, float32 thick);
    void ResortCells ();
};

} // namespace Map