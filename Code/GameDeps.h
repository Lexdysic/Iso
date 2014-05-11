
//=============================================================================
//
// Dependencies
//
//=============================================================================

#ifdef USES_GAME_WORLD_VIEW
#   define USES_ENGINE_TIME
#endif

#ifdef USES_GAME_TABLE
#   define USES_ENGINE_UTILITY_NOTIFIER
#endif

#ifdef USES_GAME_CONTENT
#   define USES_ENGINE_ENTITY
#endif



//=============================================================================
//
// Include the headers
//
//=============================================================================

#include "EngineDeps.h"

#ifdef USES_GAME_WIDGETS
#   include "Widgets\Widgets.h"
#endif

#ifdef USES_GAME_CONTENT
#   include "Content\Content.h"
#endif

#ifdef USES_GAME_WORLD_VIEW
#   include "WorldView\WorldView.h"
#endif

#ifdef USES_GAME_MAP
#   include "Map\Map.h"
#endif

#ifdef USES_GAME_TABLE
#   include "GameTable\GameTable.h"
#endif