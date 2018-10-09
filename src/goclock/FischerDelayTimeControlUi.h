
#ifndef __FischerDelayTimeControlUi_h__
#define __FischerDelayTimeControlUi_h__

#include <avr/pgmspace.h>
#include <stdlib.h>
#include "TimeControlUi.h"
#include "FischerDelayTimeControl.h"

const char fischerDelayName[] PROGMEM = "Fischer Delay";

const char fischerDelayOption1[] PROGMEM = " 10 sec + 1 sec "
                                           "                ";
const char fischerDelayOption2[] PROGMEM = " 1 min +  1 sec "
                                           "                ";
const char fischerDelayOption3[] PROGMEM = " 2 min + 12 sec "
                                           "                ";
const char fischerDelayOption4[] PROGMEM = " 3 min +  2 sec "
                                           "                ";
const char fischerDelayOption5[] PROGMEM = " 5 min +  3 sec "
                                           "                ";
const char fischerDelayOption6[] PROGMEM = "25 min + 10 sec "
                                           "                ";

const char *const fischerDelayOptions[] PROGMEM = {
	fischerDelayOption1, fischerDelayOption2, fischerDelayOption3, fischerDelayOption4, fischerDelayOption5,
	fischerDelayOption6
};

const char fischerDelayValueLabel1[] PROGMEM = "Time";
const char fischerDelayValueLabel2[] PROGMEM = "Bonus";

const CustomValue fischerDelayCustomSetup[] PROGMEM = {
	{
		fischerDelayValueLabel1,
		CUSTOM_VALUE_TIME,
		0L,
		CUSTOM_VALUE_TIME_MAX,
		2L * 60L
	},
	{
		fischerDelayValueLabel2,
		CUSTOM_VALUE_INT,
		0L,
		60L,
		12L
	}
};

const char fischerDelayFormat[] PROGMEM = "+%d sec";

class FischerDelayTimeControlUi : public TimeControlUi {
public:

	virtual const char* getName() {
		return fischerDelayName;
	}

	virtual uint8_t getTimeControlType() {
		return FISCHERDELAY_TYPE;
	}

	virtual int16_t getNumberOfOptions() {
		return 6;
	}

	virtual const char* getOption(int16_t option) {
		switch (option) {
		case 0:
			return fischerDelayOptions[option];

		case 1:
			return fischerDelayOptions[option];

		case 2:
			return fischerDelayOptions[option];

		case 3:
			return fischerDelayOptions[option];

		case 4:
			return fischerDelayOptions[option];

		case 5:
			return fischerDelayOptions[option];
		}
		return fischerDelayOptions[0];
	}

	virtual long* getDefaultOptionValues(int16_t option) {
		static long values[2];

		switch (option) {
		case 0:
			values[0] = 10L;
			values[1] = 1L;
			break;

		case 1:
			values[0] = 60L;
			values[1] = 1L;
			break;

		case 2:
			values[0] = 60L * 2L;
			values[1] = 12L;
			break;

		case 3:
			values[0] = 60L * 3L;
			values[1] = 2L;
			break;

		case 4:
			values[0] = 60L * 5L;
			values[1] = 3L;
			break;

		case 5:
			values[0] = 60L * 25L;
			values[1] = 10L;
			break;
		}
		return values;
	}

	virtual uint8_t getCustomSetupLength() {
		return 2;
	}

	virtual const CustomValue getCustomSetupValue(uint8_t index) {
		return PROGMEM_getAnything(&fischerDelayCustomSetup[index]);
	}

	virtual TimeControl* create(long values[]) {
		return new FischerDelayTimeControl(values[0] * 1000L, values[1] * 1000L);
	}

	virtual bool renderGame(GameClock *gameClock, GameClockLcd *lcd) {
		bool beep = TimeControlUi::renderGame(gameClock, lcd);

		FischerDelayTimeControl *fischerDelay = (FischerDelayTimeControl *)gameClock->getTimeControl();

		lcd->sPrintBottomCenter(fischerDelayFormat, fischerDelay->getBonus() / 1000L);
		return beep;
	}
};

#endif // ifndef __FischerDelayTimeControlUi_h__
