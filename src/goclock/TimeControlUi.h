
#ifndef __TimeControlUi_h__
#define __TimeControlUi_h__

#include "GameClock.h"
#include "GameClockLcd.h"
#include "progmemReader.h"

#define UI_BUFFER_SIZE 32

#define BRONSTEINDELAY_TYPE  0
#define BYOYOMI_TYPE         1
#define CANADIANBYOYOMI_TYPE 2
#define FISCHERDELAY_TYPE    3
#define HOURGLASS_TYPE       4
#define SUDDENDEATH_TYPE     5

#define CUSTOM_VALUE_TIME    0
#define CUSTOM_VALUE_MIN_SEC 1
#define CUSTOM_VALUE_INT     2
#define CUSTOM_VALUE_TIME_MAX    (99L * 60L * 60L + 59L * 60L)
#define CUSTOM_VALUE_MIN_SEC_MAX (59L * 60L)
#define CUSTOM_VALUE_INT_MAX     100000L


struct CustomValue {
	const char *label;
	uint8_t     type;
	long        min;
	long        max;
	long        def;
};

class TimeControlUi {
public:

	virtual const char      * getName()                              = 0;
	virtual uint8_t           getTimeControlType()                   = 0;
	virtual int16_t           getNumberOfOptions()                   = 0;
	virtual const char      * getOption(int16_t option)              = 0;
	virtual long            * getDefaultOptionValues(int16_t option) = 0;
	virtual uint8_t           getCustomSetupLength()                 = 0;
	virtual const CustomValue getCustomSetupValue(uint8_t index)     = 0;
	virtual TimeControl     * create(long values[])                  = 0;

	virtual bool              renderGame(GameClock *gameClock, GameClockLcd *lcd) {
		TimeControl *timeControl = gameClock->getTimeControl();

		lcd->printTopLeftTime(timeControl->getPlayerOneTime(gameClock->getClock()),
		                      gameClock->isPlayerOnePlaying());
		lcd->printTopRightTime(timeControl->getPlayerTwoTime(gameClock->getClock()),
		                       gameClock->isPlayerTwoPlaying());

		return false;
	}
};

#endif // ifndef __TimeControlUi_h__
