class CCustomState : public CGameState
{
	protected:
		CSnakeGameInfo *info;

	public:
		CCustomState(CSnakeGameInfo *_info)
		{
			info = _info;
		}
};

#include "states/state_menu.h"
#include "states/state_options.h"
#include "states/state_startinglevel.h"
#include "states/state_pause.h"
#include "states/state_lostlife.h"
#include "states/state_playing.h"
#include "states/state_lostlastlife.h"
#include "states/state_exitrequested.h"
#include "states/state_help.h"



