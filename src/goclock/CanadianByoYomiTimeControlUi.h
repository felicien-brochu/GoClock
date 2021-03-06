
#ifndef __CanadianByoYomiTimeControlUi_h__
#define __CanadianByoYomiTimeControlUi_h__

#include <stdlib.h>

#include "TimeControlUi.h"
#include "ByoYomiTimeControl.h"

const char canadianByoYomiName[] PROGMEM = "Canadian ByoYomi";

const char canadianByoYomiOption1[] PROGMEM = "1m +25mov in 15m"
                                              "                ";
const char canadianByoYomiOption2[] PROGMEM = "1m +25mov in 10m"
                                              "                ";
const char canadianByoYomiOption3[] PROGMEM = "1m +25mov in  7m"
                                              "                ";
const char canadianByoYomiOption4[] PROGMEM = "1m +25mov in  5m"
                                              "                ";

const char *const canadianByoYomiOptions[] PROGMEM = {
	canadianByoYomiOption1, canadianByoYomiOption2, canadianByoYomiOption3, canadianByoYomiOption4
};

const char canadianByoYomiValueLabel1[] PROGMEM = "Main time";
const char canadianByoYomiValueLabel2[] PROGMEM = "Period time";
const char canadianByoYomiValueLabel3[] PROGMEM = "Number of moves";

const CustomValue canadianByoYomiCustomSetup[] PROGMEM = {
	{
		canadianByoYomiValueLabel1,
		CUSTOM_VALUE_TIME,
		1L,
		CUSTOM_VALUE_TIME_MAX,
		1L * 60L
	},
	{
		canadianByoYomiValueLabel2,
		CUSTOM_VALUE_TIME,
		1L,
		CUSTOM_VALUE_TIME_MAX,
		15L * 60L
	},
	{
		canadianByoYomiValueLabel3,
		CUSTOM_VALUE_INT,
		1L,
		CUSTOM_VALUE_INT_MAX,
		25L
	}
};

const char canadianByoYomiUiNormalTime[] PROGMEM = "normal";
const char canadianByoYomiUiFormat[] PROGMEM = "%d";

class CanadianByoYomiTimeControlUi : public TimeControlUi {
public:

	virtual const char* getName() {
		return canadianByoYomiName;
	}

	virtual uint8_t getTimeControlType() {
		return CANADIANBYOYOMI_TYPE;
	}

	virtual int16_t getNumberOfOptions() {
		return 4;
	}

	virtual const char* getOption(int16_t option) {
		switch (option) {
		case 0:
			return canadianByoYomiOptions[option];

		case 1:
			return canadianByoYomiOptions[option];

		case 2:
			return canadianByoYomiOptions[option];

		case 3:
			return canadianByoYomiOptions[option];
		}
		return canadianByoYomiOptions[0];
	}

	virtual long* getDefaultOptionValues(int16_t option) {
		static long values[3];

		values[0] = 60L;
		values[2] = 25L;

		switch (option) {
		case 0:
			values[1] = 60L * 15L;
			break;

		case 1:
			values[1] = 60L * 10L;
			break;

		case 2:
			values[1] = 60L * 7L;
			break;

		case 3:
			values[1] = 60L * 5L;
			break;
		}

		return values;
	}

	virtual uint8_t getCustomSetupLength() {
		return 3;
	}

	virtual const CustomValue getCustomSetupValue(uint8_t index) {
		return PROGMEM_getAnything(&canadianByoYomiCustomSetup[index]);
	}

	virtual TimeControl* create(long values[]) {
		uint32_t byoYomiTime;
		ByoYomiSetup byoYomiSetup;

		byoYomiSetup.time            = values[0] * 1000L;
		byoYomiTime                  = values[1] * 1000L;
		byoYomiSetup.periodTime      = byoYomiTime;
		byoYomiSetup.numberOfPeriods = 1;
		byoYomiSetup.numberOfPlays   = values[2];

		for (int i = 0; i < byoYomiSetup.numberOfPeriods; i++) {
			byoYomiSetup.periods[i].numberOfPlays = values[2];
			byoYomiSetup.periods[i].time          = byoYomiTime;
		}
		return new ByoYomiTimeControl(byoYomiSetup);
	}

	virtual bool renderGame(GameClock *gameClock, GameClockLcd *lcd) {
		bool beep = TimeControlUi::renderGame(gameClock, lcd);

		ByoYomiTimeControl *canadian = (ByoYomiTimeControl *)gameClock->getTimeControl();

		if (canadian->isPlayerOneInNormalTime()) {
			lcd->printBottomLeft(canadianByoYomiUiNormalTime);
		} else {
			lcd->sPrintBottomLeft(canadianByoYomiUiFormat, canadian->getPlayerOneRemainingNumberOfPlays());
		}

		if (canadian->isPlayerTwoInNormalTime()) {
			lcd->printBottomRight(canadianByoYomiUiNormalTime);
		} else {
			lcd->sPrintBottomRight(canadianByoYomiUiFormat, canadian->getPlayerTwoRemainingNumberOfPlays());
		}

		// Beep on 5 last seconds
		if (!gameClock->isPaused()) {
			uint32_t time = 100000;

			if (gameClock->isPlayerOnePlaying()) {
				time = canadian->getPlayerOneTime(gameClock->getClock());
			} else if (gameClock->isPlayerTwoPlaying()) {
				time = canadian->getPlayerTwoTime(gameClock->getClock());
			}
			beep |= (time <= 6020) && ((time % 1000 <= 20) || (time % 1000 >= 980));
		}

		return beep;
	}
};

#endif // ifndef __CanadianByoYomiTimeControlUi_h__
