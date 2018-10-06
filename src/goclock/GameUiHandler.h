
#ifndef __GameUiHandler_h__
#define __GameUiHandler_h__

#include "GameClock.h"
#include "GameClockLcd.h"
#include "UiHandler.h"
#include "TimeControlUi.h"
#include "GameButtonGestures.h"
#include "Buzzer.h"

extern GameClock gameClock;
extern GameButtonGestures buttonGestures;
extern GameClockLcd lcd2;

const char gameUiHandlerPlayerOneWinnerMessage[] PROGMEM = "WINNER ";
const char gameUiHandlerPlayerTwoWinnerMessage[] PROGMEM = " WINNER";

void (*resetFunc) (void) = 0; // declare reset function @ address 0

class GameUiHandler : public UiHandler {
	TimeControlUi *timeControlUi;

public:

	virtual void tick(Clock *clock) {
		buttonGestures.tick(clock);
		gameClock.tick();

		if (buttonGestures.wasPushButtonPushed()) {
			if (gameClock.isOver()) {
				resetFunc();
			} else if (gameClock.isPaused()) {
				lcd2.setBlinking(false);
				gameClock.resume();
			} else {
				lcd2.setBlinking(true);
				gameClock.pause();
			}
			return;
		}

		if (buttonGestures.isToggleButtonDisabled()) {
			gameClock.selectPlayerTwo();
		} else if (buttonGestures.isToggleButtonEnabled()) {
			gameClock.selectPlayerOne();
		}
	}

	virtual void render(Clock *clock) {
		lcd2.beginRender(clock);
		bool doBeep = timeControlUi->renderGame(&gameClock, &lcd2);

		if (doBeep) {
			beep();
		}

		if (gameClock.isOver()) {
			if (gameClock.playerOneWon()) {
				lcd2.printBottomLeft(gameUiHandlerPlayerOneWinnerMessage);
			} else if (gameClock.playerTwoWon()) {
				lcd2.printBottomRight(gameUiHandlerPlayerTwoWinnerMessage);
			}
		}
		lcd2.endRender();
	}

	void setTimeControlUi(TimeControlUi *timeControlUi) {
		this->timeControlUi = timeControlUi;
	}
};

#endif // ifndef __GameUiHandler_h__
