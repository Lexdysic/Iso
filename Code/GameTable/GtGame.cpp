#include "GtPch.h"

namespace Table
{

//=============================================================================
IPlayer * CGame::CreatePlayer ()
{
    auto * player = new CPlayer ();

    m_players.Add(player);

    return player;
}

//=============================================================================
IPhase * CGame::CreatePhase ()
{
    auto * phase = new CPhase ();

    m_phases.Add(phase);

    return phase;
}

//=============================================================================
void CGame::Start (IPlayer * startPlayer)
{
    ASSERT(startPlayer);
    ASSERT(!m_players.IsEmpty());
    ASSERT(!m_phases.IsEmpty());

    m_playerStart = CPlayer::From(startPlayer);
    m_playerCurr = m_playerStart;
    
    m_phaseCurr = m_phases[0];

    ASSERT(m_players.Contains(m_playerStart));
}

//=============================================================================
void CGame::Next ()
{
    const uint indexCurr = m_players.Find(m_playerCurr);
    ASSERT(indexCurr < m_players.Count());

    const uint indexNext = (indexCurr + 1) % m_players.Count();

    CPlayer * playerNext = m_players[indexNext];

    // TODO: Notify player end

    if (playerNext == m_playerStart)
    {
        const uint indexCurr = m_phases.Find(m_phaseCurr);
        ASSERT(indexCurr < m_phases.Count());

        const uint indexNext = (indexCurr + 1) % m_phases.Count();
        auto * phaseNext = m_phases[indexNext];

        // TODO: notify phase end

        m_phaseCurr = phaseNext;

        // TODO: notify phase begin
    }

     m_playerCurr = playerNext;

     // TODO: Notify player start
}

} // GameTable