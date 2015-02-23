#include "Utilities/Notifier.h"

namespace Table
{

interface IGame;
interface IPlayer;
interface IPhase;

enum class EGameOrder
{
    Turn,
    Phase
};

interface IGame
{
    virtual IPlayer * CreatePlayer () pure;
    virtual IPhase * CreatePhase () pure;

    virtual IPhase * CurrentPhase () pure;
    virtual IPlayer * CurrentPlayer () pure;

    virtual void SetOrder (EGameOrder order) pure;

    virtual void Start (IPlayer * startPlayer) pure;
    virtual void Next () pure;
};

class CGameNotify : TNotifyTarget<CGameNotify>
{
    virtual void OnGameTablePhaseBegin (IPhase * phase);
    virtual void OnGameTablePhaseEnd (IPhase * phase);
    virtual void OnGameTableTurnBegin (IPlayer * player);
    virtual void OnGameTableTurnEnd (IPlayer * player);
};


interface IPlayer
{
};

interface IPhase
{
};

} // namespace Table