#ifndef __ByoYomiTimeControlUi_h__
#define __ByoYomiTimeControlUi_h__

#include "TimeControlUi.h"
#include "ByoYomiTimeControl.h"

const char byoYomiName[] PROGMEM = "Byo Yomi";

const char byoYomiOption1[] PROGMEM = " 25m + (5 x 30s)"
                                      "                ";
const char byoYomiOption2[] PROGMEM = " 10m + (5 x 30s)"
                                      "                ";
const char byoYomiOption3[] PROGMEM = "  1m + (5 x 20s)"
                                      "                ";

const char *const byoYomiOptions[] PROGMEM = {
	byoYomiOption1, byoYomiOption2, byoYomiOption3
};

const char byoYomiValueLabel1[] PROGMEM = "Main time";
const char byoYomiValueLabel2[] PROGMEM = "Byo yomi time";
const char byoYomiValueLabel3[] PROGMEM = "Number of period";

const CustomValue byoYomiCustomSetup[] PROGMEM = {
	{
		byoYomiValueLabel1,
		CUSTOM_VALUE_TIME,
		0L,
		CUSTOM_VALUE_TIME_MAX,
		10L * 60L
	},
	{
		byoYomiValueLabel2,
		CUSTOM_VALUE_MIN_SEC,
		0L,
		CUSTOM_VALUE_MIN_SEC_MAX,
		30L
	},
	{
		byoYomiValueLabel3,
		CUSTOM_VALUE_INT,
		0L,
		BYO_YOMI_SETUP_MAX_PERIODS,
		5L
	}
};

const char byoYomiUiNormalTime[] PROGMEM = "normal";
const char byoYomiUiFormat[] PROGMEM = "%d";

class ByoYomiTimeControlUi : public TimeControlUi {
public:

	virtual const char* getName() {
		return byoYomiName;
	}

	virtual int16_t getNumberOfOptions() {
		return 3;
	}

	virtual const char* getOption(int16_t option) {
		switch (option) {
		case 0:
			return byoYomiOptions[option];

		case 1:
			return byoYomiOptions[option];

		case 2:
			return byoYomiOptions[option];
		}
		return byoYomiOptions[0];
	}

	virtual TimeControl* create(int16_t option) {
		uint32_t byoYomiTime;
		ByoYomiSetup byoYomiSetup;

		switch (option) {
		case 0:
			byoYomiSetup.time                     = 1000L * 60L * 25L;
			byoYomiSetup.numberOfPeriods          = 5;
			byoYomiTime                           = 30L * 1000L;
			byoYomiSetup.periods[0].numberOfPlays = 1;
			byoYomiSetup.periods[0].time          = byoYomiTime;
			byoYomiSetup.periods[1].numberOfPlays = 1;
			byoYomiSetup.periods[1].time          = byoYomiTime;
			byoYomiSetup.periods[2].numberOfPlays = 1;
			byoYomiSetup.periods[2].time          = byoYomiTime;
			byoYomiSetup.periods[3].numberOfPlays = 1;
			byoYomiSetup.periods[3].time          = byoYomiTime;
			byoYomiSetup.periods[4].numberOfPlays = 1;
			byoYomiSetup.periods[4].time          = byoYomiTime;
			break;

		case 1:
			byoYomiSetup.time                     = 1000L * 60L * 10L;
			byoYomiSetup.numberOfPeriods          = 5;
			byoYomiTime                           = 30L * 1000L;
			byoYomiSetup.periods[0].numberOfPlays = 1;
			byoYomiSetup.periods[0].time          = byoYomiTime;
			byoYomiSetup.periods[1].numberOfPlays = 1;
			byoYomiSetup.periods[1].time          = byoYomiTime;
			byoYomiSetup.periods[2].numberOfPlays = 1;
			byoYomiSetup.periods[2].time          = byoYomiTime;
			byoYomiSetup.periods[3].numberOfPlays = 1;
			byoYomiSetup.periods[3].time          = byoYomiTime;
			byoYomiSetup.periods[4].numberOfPlays = 1;
			byoYomiSetup.periods[4].time          = byoYomiTime;
			break;

		case 2:
			byoYomiSetup.time                     = 1000L * 60L * 1L;
			byoYomiSetup.numberOfPeriods          = 3;
			byoYomiTime                           = 20L * 1000L;
			byoYomiSetup.periods[0].numberOfPlays = 1;
			byoYomiSetup.periods[0].time          = byoYomiTime;
			byoYomiSetup.periods[1].numberOfPlays = 1;
			byoYomiSetup.periods[1].time          = byoYomiTime;
			byoYomiSetup.periods[2].numberOfPlays = 1;
			byoYomiSetup.periods[2].time          = byoYomiTime;
			break;
		}

		return new ByoYomiTimeControl(byoYomiSetup);
	}

	virtual uint8_t getCustomSetupLength() {
		return 3;
	}

	virtual const CustomValue getCustomSetupValue(uint8_t index) {
		return PROGMEM_getAnything(&byoYomiCustomSetup[index]);
	}

	virtual TimeControl* createCustom(long customValues[]) {
		uint32_t byoYomiTime;
		ByoYomiSetup byoYomiSetup;

		byoYomiSetup.time            = customValues[0] * 1000L;
		byoYomiTime                  = customValues[1] * 1000L;
		byoYomiSetup.numberOfPeriods = (int)customValues[2];

		for (int i = 0; i < byoYomiSetup.numberOfPeriods; i++) {
			byoYomiSetup.periods[i].numberOfPlays = 1;
			byoYomiSetup.periods[i].time          = byoYomiTime;
		}
		return new ByoYomiTimeControl(byoYomiSetup);
	}

	virtual bool renderGame(GameClock *gameClock, GameClockLcd *lcd) {
		bool beep                   = TimeControlUi::renderGame(gameClock, lcd);
		ByoYomiTimeControl *byoYomi = (ByoYomiTimeControl *)gameClock->getTimeControl();

		if (byoYomi->isPlayerOneInNormalTime()) {
			lcd->printBottomLeft(byoYomiUiNormalTime);
		} else {
			lcd->sPrintBottomLeft(byoYomiUiFormat, byoYomi->getPlayerOneRemainingByoYomiPeriods());
		}

		if (byoYomi->isPlayerTwoInNormalTime()) {
			lcd->printBottomRight(byoYomiUiNormalTime);
		} else {
			lcd->sPrintBottomRight(byoYomiUiFormat, byoYomi->getPlayerTwoRemainingByoYomiPeriods());
		}

		// Beep on 5 last seconds
		if (!gameClock->isPaused()) {
			uint32_t time = 100000;

			if (gameClock->isPlayerOnePlaying()) {
				time = byoYomi->getPlayerOneTime(gameClock->getClock());
			} else if (gameClock->isPlayerTwoPlaying()) {
				time = byoYomi->getPlayerTwoTime(gameClock->getClock());
			}
			beep |= (time <= 5020) && ((time % 1000 <= 20) || (time % 1000 >= 980));
		}

		return beep;
	}
};

#endif // ifndef __ByoYomiTimeControlUi_h__
