namespace Table
{

class CGame : public IGame
{

private: // IGame

    IPlayer * CreatePlayer () override;
    IPhase * CreatePhase () override;

    IPhase * CurrentPhase () override { return m_phaseCurr; }
    IPlayer * CurrentPlayer () override { return m_playerCurr; }

    void SetOrder (EGameOrder order) override { m_order = order; }

    void Start (IPlayer * startPlayer) override;
    void Next () override;

private: // Data

    EGameOrder        m_order;
    CPlayer *         m_playerCurr;
    CPlayer *         m_playerStart;
    CPhase *          m_phaseCurr;
    TArray<CPlayer *> m_players;
    TArray<CPhase *>  m_phases;
};

}