
#ifndef __TimeControlUi_h__
#define __TimeControlUi_h__

#include "GameClock.h"
#include "GameClockLcd.h"

#define UI_BUFFER_SIZE 32

class TimeControlUi {
public:

	virtual const char * getName()                 = 0;
	virtual int16_t      getNumberOfOptions()      = 0;
	virtual const char * getOption(int16_t option) = 0;
	virtual TimeControl* create(int16_t option)    = 0;

	virtual void         renderGame(GameClock *gameClock, GameClockLcd *lcd) {
		TimeControl *timeControl = gameClock->getTimeControl();

		lcd->printTopLeftTime(timeControl->getPlayerOneTime(gameClock->getClock()),
		                      gameClock->isPlayerOnePlaying());
		lcd->printTopRightTime(timeControl->getPlayerTwoTime(gameClock->getClock()),
		                       gameClock->isPlayerTwoPlaying());
	}
};

#endif // ifndef __TimeControlUi_h__
