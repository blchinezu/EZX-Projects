#include <string>

#include "./CLASS_NAME.h"
#include "../StateModule/GameStateManager.h"
#include "../GfxModule/GfxModule.h"
#include "../InputModule/Input.h"
#include "../Logger/Logger.h"

using namespace std;

using namespace GfxModule;
using namespace SoundModule;
using namespace GameStateModule;
using namespace InputModule;
using namespace LogModule;


CLASS_NAME::CLASS_NAME(void)
{

}

CLASS_NAME::~CLASS_NAME(void)
{

}

void CLASS_NAME::PushInitialise(
    GfxModuleWrapper* AGfxModule,
    SoundModuleWrapper* ASoundModule
)
{

}

void CLASS_NAME::PushCleanup(
    GfxModuleWrapper* AGfxModule,
    SoundModuleWrapper* ASoundModule
)
{

}

void CLASS_NAME::PopInitialise(
    GfxModuleWrapper* AGfxModule,
    SoundModuleWrapper* ASoundModule
)
{

}

void CLASS_NAME::PopCleanup(
    GfxModuleWrapper* AGfxModule,
    SoundModuleWrapper* ASoundModule
)
{

}

void CLASS_NAME::Update(
    GfxModuleWrapper* AGfxModule,
    SoundModuleWrapper* ASoundModule,
    Input* AInputModule,
    GameStateManager* AGameStateManager
)
{

}

/*
History
=======
2006-07-13: Created file and base functionality.
*/
