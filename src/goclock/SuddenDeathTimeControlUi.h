
#ifndef __SuddenDeathTimeControlUi_h__
#define __SuddenDeathTimeControlUi_h__

#include <stdlib.h>

#include "TimeControlUi.h"
#include "SuddenDeathTimeControl.h"

const char suddenDeathName[] PROGMEM = "Sudden Death";

const char suddenDeathOption1[] PROGMEM = "     30 sec     "
                                          "                ";
const char suddenDeathOption2[] PROGMEM = "      1 min     "
                                          "                ";
const char suddenDeathOption3[] PROGMEM = "      5 min     "
                                          "                ";
const char suddenDeathOption4[] PROGMEM = "     10 min     "
                                          "                ";
const char suddenDeathOption5[] PROGMEM = "     20 min     "
                                          "                ";
const char suddenDeathOption6[] PROGMEM = "     30 min     "
                                          "                ";
const char suddenDeathOption7[] PROGMEM = "      1 hour    "
                                          "                ";

const char *const suddenDeathOptions[] PROGMEM = {
	suddenDeathOption1, suddenDeathOption2, suddenDeathOption3, suddenDeathOption4, suddenDeathOption5,
	suddenDeathOption6, suddenDeathOption7
};

const char suddenDeathValueLabel1[] PROGMEM = "Time";

const CustomValue suddenDeathCustomSetup[] PROGMEM = {
	{
		suddenDeathValueLabel1,
		CUSTOM_VALUE_TIME,
		0L,
		CUSTOM_VALUE_TIME_MAX,
		10L * 60L
	}
};

class SuddenDeathTimeControlUi : public TimeControlUi {
public:

	virtual const char* getName() {
		return suddenDeathName;
	}

	virtual uint8_t getTimeControlType() {
		return SUDDENDEATH_TYPE;
	}

	virtual int16_t getNumberOfOptions() {
		return 7;
	}

	virtual const char* getOption(int16_t option) {
		switch (option) {
		case 0:
			return suddenDeathOptions[option];

		case 1:
			return suddenDeathOptions[option];

		case 2:
			return suddenDeathOptions[option];

		case 3:
			return suddenDeathOptions[option];

		case 4:
			return suddenDeathOptions[option];

		case 5:
			return suddenDeathOptions[option];

		case 6:
			return suddenDeathOptions[option];
		}
		return suddenDeathOptions[0];
	}

	virtual long* getDefaultOptionValues(int16_t option) {
		static long values[1];

		switch (option) {
		case 0:
			values[0] = 30L;
			break;

		case 1:
			values[0] = 60L * 1L;
			break;

		case 2:
			values[0] = 60L * 5L;
			break;

		case 3:
			values[0] = 60L * 10L;
			break;

		case 4:
			values[0] = 60L * 20L;
			break;

		case 5:
			values[0] = 60L * 30L;
			break;

		case 6:
			values[0] = 60L * 60L * 1L;
			break;
		}
		return values;
	}

	virtual uint8_t getCustomSetupLength() {
		return 1;
	}

	virtual const CustomValue getCustomSetupValue(uint8_t index) {
		return PROGMEM_getAnything(&suddenDeathCustomSetup[index]);
	}

	virtual TimeControl* create(long values[]) {
		return new SuddenDeathTimeControl(values[0] * 1000L);
	}
};

#endif // ifndef __SuddenDeathTimeControlUi_h__
