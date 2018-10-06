
#ifndef __GameUiHandler_h__
#define __GameUiHandler_h__

#include "GameClock.h"
#include "GameClockLcd.h"
#include "UiHandler.h"
#include "TimeControlUi.h"
#include "GameButtonGestures.h"
#include "Buzzer.h"

extern GameClock *gameClock;
extern GameButtonGestures buttonGestures;
extern GameClockLcd lcd2;

extern UiHandler *currentUiHandler;
extern UiHandler *startingHandler;

const char gameUiHandlerPlayerOneWinnerMessage[] PROGMEM = "WINNER ";
const char gameUiHandlerPlayerTwoWinnerMessage[] PROGMEM = " WINNER";

class GameUiHandler : public UiHandler {
	TimeControlUi *timeControlUi;
	bool started;
	int8_t toggleButtonState;

public:

	GameUiHandler() {
		started           = false;
		toggleButtonState = -1;
	}

	virtual void tick(Clock *clock) {
		buttonGestures.tick(clock);

		if (started) {
			gameClock->tick();

			if (buttonGestures.wasPushButtonLongPushed()) {
				goBackToStartingHandler();
				return;
			}

			if (buttonGestures.wasPushButtonPushed()) {
				if (gameClock->isOver()) {
					goBackToStartingHandler();
				} else if (gameClock->isPaused()) {
					lcd2.setBlinking(false);
					gameClock->resume();
				} else {
					lcd2.setBlinking(true);
					gameClock->pause();
				}
			}

			if (buttonGestures.isToggleButtonDisabled()) {
				gameClock->selectPlayerOne();
			} else if (buttonGestures.isToggleButtonEnabled()) {
				gameClock->selectPlayerTwo();
			}
		} else {
			if ((toggleButtonState >= 0) &&
			    (toggleButtonState != buttonGestures.isToggleButtonDisabled())) {
				started = true;
			} else {
				toggleButtonState = buttonGestures.isToggleButtonDisabled();
			}
		}
	}

	virtual void render(Clock *clock) {
		lcd2.beginRender(clock);
		bool doBeep = timeControlUi->renderGame(gameClock, &lcd2);

		if (doBeep) {
			beep();
		}

		if (gameClock->isOver()) {
			if (gameClock->playerOneWon()) {
				lcd2.printBottomLeft(gameUiHandlerPlayerOneWinnerMessage);
			} else if (gameClock->playerTwoWon()) {
				lcd2.printBottomRight(gameUiHandlerPlayerTwoWinnerMessage);
			}
		}
		lcd2.endRender();
	}

	void setup(TimeControlUi *timeControlUi) {
		this->timeControlUi = timeControlUi;
		started             = false;
		toggleButtonState   = -1;
	}

private:

	void goBackToStartingHandler() {
		currentUiHandler = startingHandler;
		lcd2.setBlinking(false);
		delete gameClock;
	}
};

#endif // ifndef __GameUiHandler_h__
