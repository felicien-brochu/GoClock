
#ifndef __HourGlassTimeControlUi_h__
#define __HourGlassTimeControlUi_h__

#include "TimeControlUi.h"
#include "HourGlassTimeControl.h"

const char hourGlassName[] PROGMEM = "Hour Glass";

const char hourGlassOption1[] PROGMEM = "     30 sec     "
                                        "                ";
const char hourGlassOption2[] PROGMEM = "      1 min     "
                                        "                ";
const char hourGlassOption3[] PROGMEM = "      2 min     "
                                        "                ";
const char hourGlassOption4[] PROGMEM = "      5 min     "
                                        "                ";
const char hourGlassOption5[] PROGMEM = "     10 min     "
                                        "                ";

const char *const hourGlassOptions[] PROGMEM = {
	hourGlassOption1, hourGlassOption2, hourGlassOption3, hourGlassOption4, hourGlassOption5
};

const char hourGlassValueLabel1[] PROGMEM = "Time";

const CustomValue hourGlassCustomSetup[] PROGMEM = {
	{
		hourGlassValueLabel1,
		CUSTOM_VALUE_TIME,
		0L,
		CUSTOM_VALUE_TIME_MAX,
		5L * 60L
	}
};

const char HOURGLASS_FORMAT[] PROGMEM = "%c %d sec";

class HourGlassTimeControlUi : public TimeControlUi {
public:

	virtual const char* getName() {
		return hourGlassName;
	}

	virtual uint8_t getTimeControlType() {
		return HOURGLASS_TYPE;
	}

	virtual int16_t getNumberOfOptions() {
		return 5;
	}

	virtual const char* getOption(int16_t option) {
		switch (option) {
		case 0:
			return hourGlassOptions[option];

		case 1:
			return hourGlassOptions[option];

		case 2:
			return hourGlassOptions[option];

		case 3:
			return hourGlassOptions[option];

		case 4:
			return hourGlassOptions[option];
		}
		return hourGlassOptions[0];
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
			values[0] = 60L * 2L;
			break;

		case 3:
			values[0] = 60L * 5L;
			break;

		case 4:
			values[0] = 60L * 10L;
			break;
		}
		return values;
	}

	virtual uint8_t getCustomSetupLength() {
		return 1;
	}

	virtual const CustomValue getCustomSetupValue(uint8_t index) {
		return PROGMEM_getAnything(&hourGlassCustomSetup[index]);
	}

	virtual TimeControl* create(long values[]) {
		return new HourGlassTimeControl(values[0] * 1000L);
	}

	virtual bool renderGame(GameClock *gameClock, GameClockLcd *lcd) {
		bool beep = TimeControlUi::renderGame(gameClock, lcd);

		HourGlassTimeControl *hourGlass = (HourGlassTimeControl *)gameClock->getTimeControl();

		if (hourGlass->getLastTransferedTime() != 0) {
			lcd->sPrintBottomCenter(HOURGLASS_FORMAT,
			                        (hourGlass->wasTimeTransferredFromPlayerOne() ? '>' : '<'),
			                        (hourGlass->getLastTransferedTime() / 1000L));
		}

		return beep;
	}
};

#endif // ifndef __HourGlassTimeControlUi_h__
