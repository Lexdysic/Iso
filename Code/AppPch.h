
#include <deque>
#include <numeric>

#define USES_ENGINE_GRAPHICS
#define USES_ENGINE_USER_INTERFACE
#define USES_ENGINE_TIME
#define USES_ENGINE_PATHING
#define USES_ENGINE_GEOMETRY
#define USES_ENGINE_ENTITY
#define USES_ENGINE_PHYSICS
#define USES_ENGINE_WINDOW
#define USES_ENGINE_THREAD

#define USES_GAME_WIDGETS
#define USES_GAME_MAP
#define USES_GAME_WORLD_VIEW
#define USES_GAME_CONTENT

#include "GameDeps.h"

#include "AppFrameTimer.h"
#include "Application.h"
#include "AppIso.h"
#include "AppPathing.h"
#include "AppBattle.h"
#include "AppPowerGraph.h"
#include "AppBouyant.h"
#include "AppMachi.h"