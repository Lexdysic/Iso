#include <Windows.h>
#include <numeric>

#include "Ferrite.h"
#include "Basics/Time/Time.h"
#include "Basics/Geometry/Geometry.h"
#include "Basics/Thread/Thread.h"
#include "Systems/Entity/Entity.h"
#include "Systems/Physics/Physics.h"
#include "Systems/Graphics/Graphics.h"
#include "Systems/Pathing/Pathing.h"
#include "Systems/UserInterface/UserInterface.h"
#include "Systems/Window/Window.h"

#include "Widgets/Widgets.h"
#include "Map/Map.h"
#include "WorldView/WorldView.h"
#include "Content/Content.h"

#include "AppFrameTimer.h"
#include "Application.h"
#include "AppIso.h"
#include "AppPathing.h"
#include "AppBattle.h"
#include "AppPowerGraph.h"
#include "AppBouyant.h"
#include "AppMachi.h"