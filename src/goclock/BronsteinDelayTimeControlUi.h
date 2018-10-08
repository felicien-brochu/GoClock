
#ifndef __BronsteinDelayTimeControlUi_h__
#define __BronsteinDelayTimeControlUi_h__

#include <stdlib.h>
#include "TimeControlUi.h"
#include "BronsteinDelayTimeControl.h"

const char bronsteinDelayName[] PROGMEM = "Bronstein Delay";

const char bronsteinDelayOption1[] PROGMEM = " 2 min + 12 sec "
                                             "                ";
const char bronsteinDelayOption2[] PROGMEM = " 3 min + 12 sec "
                                             "                ";
const char bronsteinDelayOption3[] PROGMEM = " 5 min + 12 sec "
                                             "                ";
const char bronsteinDelayOption4[] PROGMEM = "10 min + 10 sec "
                                             "                ";

const char *const bronsteinDelayOptions[] PROGMEM = {
	bronsteinDelayOption1, bronsteinDelayOption2, bronsteinDelayOption3, bronsteinDelayOption4
};

const char bronsteinDelayValueLabel1[] PROGMEM = "Time";
const char bronsteinDelayValueLabel2[] PROGMEM = "Delay";

const CustomValue bronsteinDelayCustomSetup[] PROGMEM = {
	{
		bronsteinDelayValueLabel1,
		CUSTOM_VALUE_TIME,
		0L,
		CUSTOM_VALUE_TIME_MAX,
		5L * 60L
	},
	{
		bronsteinDelayValueLabel2,
		CUSTOM_VALUE_INT,
		0L,
		60L,
		12L
	}
};

const CustomValue bronsteinDelayAddTimeSetup[] PROGMEM = {
	{
		bronsteinDelayValueLabel1,
		CUSTOM_VALUE_TIME,
		0L,
		CUSTOM_VALUE_TIME_MAX,
		0L
	}
};

const char bronsteinDelayFormat[] PROGMEM = "delay %d sec";

class BronsteinDelayTimeControlUi : public TimeControlUi {
public:

	virtual const char* getName() {
		return bronsteinDelayName;
	}

	virtual uint8_t getTimeControlType() {
		return BRONSTEINDELAY_TYPE;
	}

	virtual int16_t getNumberOfOptions() {
		return 4;
	}

	virtual const char* getOption(int16_t option) {
		switch (option) {
		case 0:
			return bronsteinDelayOptions[option];

		case 1:
			return bronsteinDelayOptions[option];

		case 2:
			return bronsteinDelayOptions[option];

		case 3:
			return bronsteinDelayOptions[option];
		}
		return bronsteinDelayOptions[0];
	}

	virtual long* getDefaultOptionValues(int16_t option) {
		static long values[2];

		switch (option) {
		case 0:
			values[0] = 60L * 2L;
			values[1] = 12L;
			break;

		case 1:
			values[0] = 60L * 3L;
			values[1] = 12L;
			break;

		case 2:
			values[0] = 60L * 5L;
			values[1] = 1L;
			break;

		case 3:
			values[0] = 60L * 10L;
			values[1] = 10L;
			break;
		}
		return values;
	}

	virtual uint8_t getCustomSetupLength() {
		return 2;
	}

	virtual const CustomValue getCustomSetupValue(uint8_t index) {
		return PROGMEM_getAnything(&bronsteinDelayCustomSetup[index]);
	}

	virtual TimeControl* create(long values[]) {
		return new BronsteinDelayTimeControl(values[0] * 1000L, values[1] * 1000L);
	}

	virtual uint8_t getAddTimeLength() {
		return 1;
	}

	virtual const CustomValue getAddTimeValue(uint8_t index) {
		return PROGMEM_getAnything(&bronsteinDelayAddTimeSetup[index]);
	}

	virtual bool renderGame(GameClock *gameClock, GameClockLcd *lcd) {
		bool beep = TimeControlUi::renderGame(gameClock, lcd);

		BronsteinDelayTimeControl *bronsteinDelay = (BronsteinDelayTimeControl *)gameClock->getTimeControl();

		lcd->sPrintBottomCenter(bronsteinDelayFormat, bronsteinDelay->getDelay() / 1000L);
		return beep;
	}
};

#endif // ifndef __BronsteinDelayTimeControlUi_h__
