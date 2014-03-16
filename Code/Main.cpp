
#include <Windows.h> // For WinMain args
#include "AppPch.h"

//=============================================================================
void Initialize ()
{

}

//=============================================================================
void Uninitialize ()
{

}

//=============================================================================
int PLATFORM_MAIN ()
{
    DebugBreakOnAllocation(71);

    Initialize();

    CApplication app;
    app.Go();

    Uninitialize();

    return 0;
}