
#include <LiquidCrystal.h>

#include "base.h"

#include "GameClock.h"
#include "PushButton.h"
#include "RotaryEncoder.h"
#include "ToggleButton.h"
#include "Buzzer.h"
#include "GameClockLcd.h"
#include "ArduinoClock.h"

#include "SelectTimeControlUiHandler.h"
#include "SelectTimeControlOptionUiHandler.h"
#include "CustomSetupUiHandler.h"
#include "GameUiHandler.h"
#include "AddTimeOptionUiHandler.h"

#include "ByoYomiTimeControlUi.h"
#include "CanadianByoYomiTimeControlUi.h"
#include "FischerDelayTimeControlUi.h"
#include "BronsteinDelayTimeControlUi.h"
#include "HourGlassTimeControlUi.h"
#include "SuddenDeathTimeControlUi.h"

// Hardware:
uint8_t rs = 7, enable = 6, d0 = 5, d1 = 4, d2 = 3, d3 = 2;
GameClockLcd  lcd2(rs, enable, d0, d1, d2, d3);
RotaryEncoder rotaryEncoder(9, 8, 10);
ToggleButton  toggleButton(18, 19);
Buzzer buzzer(12);

// GameClock...
ArduinoClock clock;
GameClock   *gameClock;

// TimeControls:
TimeControlUi *timeControls[] = {
	new ByoYomiTimeControlUi(),
	new CanadianByoYomiTimeControlUi(),
	new FischerDelayTimeControlUi(),
	new BronsteinDelayTimeControlUi(),
	new SuddenDeathTimeControlUi(),
	new HourGlassTimeControlUi(),
	nullptr
};

// User Interface ...
UiHandler *currentUiHandler;
UiHandler *startingHandler;
SelectTimeControlUiHandler selectTimeControlUiHandler;
SelectTimeControlOptionUiHandler selectTimeControlOptionUiHandler;
CustomSetupUiHandler customSetupUiHandler;
GameUiHandler gameUiHandler;
AddTimeOptionUiHandler addTimeOptionUiHandler;
GameButtonGestures     buttonGestures;

void setup() {
	// *
	Serial.begin(9600);
	Serial.println("Hello!");

	// */

	lcd2.init();
	toggleButton.init();
	rotaryEncoder.init();

	selectTimeControlOptionUiHandler.wire(&selectTimeControlUiHandler);
	startingHandler  = &selectTimeControlUiHandler;
	currentUiHandler = startingHandler;
}

void loop() {
	currentUiHandler->tick(&clock);
	buzzer.tick();
	currentUiHandler->render(&clock);
}
