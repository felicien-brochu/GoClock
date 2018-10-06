
#ifndef __GameUiHandler_h__
#define __GameUiHandler_h__

#include "GameClock.h"
#include "GameClockLcd.h"
#include "UiHandler.h"
#include "TimeControlUi.h"
#include "GameButtonGestures.h"
#include "Buzzer.h"

#define MENU_ITEM_GAME 0
#define MENU_ITEM_EXIT 1
#define MENU_ITEM_ADD_TIME 2
#define MENU_ITEM_RESTART 3

#define MENU_ITEM_COUNT 4

extern GameClock *gameClock;
extern GameButtonGestures buttonGestures;
extern GameClockLcd lcd2;

extern UiHandler *currentUiHandler;
extern UiHandler *startingHandler;

const char gameUiHandlerPlayerOneWinnerMessage[] PROGMEM = "WINNER ";
const char gameUiHandlerPlayerTwoWinnerMessage[] PROGMEM = " WINNER";

const char gameUiHandlerExitItem[] PROGMEM = "End Game";
const char gameUiHandlerAddTimeItem[] PROGMEM = "Add time";
const char gameUiHandlerRestartItem[] PROGMEM = "Restart Game";

class GameUiHandler : public UiHandler {
	TimeControlUi *timeControlUi;
	bool started;
	int8_t toggleButtonState;
	int menuIndex;

public:

	GameUiHandler() {
		started           = false;
		toggleButtonState = -1;
		menuIndex         = MENU_ITEM_GAME;
	}

	virtual void tick(Clock *clock) {
		buttonGestures.tick(clock);
		bool buttonPushed = buttonGestures.wasPushButtonPushed();
		int  travel       = buttonGestures.getNavigationTravel();

		if (buttonGestures.wasPushButtonLongPushed()) {
			goBackToStartingHandler();
			return;
		}

		if (!started && (menuIndex == MENU_ITEM_GAME)) { // < Initialization
			if (toggleButtonState == -1) {
				lcd2.setBlinking(true);
				toggleButtonState = buttonGestures.isToggleButtonDisabled();
			} else if (toggleButtonState != buttonGestures.isToggleButtonDisabled()) {
				lcd2.setBlinking(false);
				started = true;
			}
		}

		if (!started || gameClock->isPaused() || gameClock->isOver()) { // < Menu
			if (travel != 0) {
				menuIndex = (menuIndex + travel) % MENU_ITEM_COUNT;

				if (menuIndex < 0) {
					menuIndex = MENU_ITEM_COUNT + menuIndex;
				}

				if ((menuIndex == MENU_ITEM_GAME) && (gameClock->isPaused() || !started)) {
					lcd2.setBlinking(true);
				} else {
					lcd2.setBlinking(false);
				}
			}

			if (buttonPushed) {
				onMenuClick();
			}
		} else if ((menuIndex == MENU_ITEM_GAME) && started && !gameClock->isPaused() && !gameClock->isOver()) { // < Normal play
			gameClock->tick();

			if (buttonPushed) {
				lcd2.setBlinking(true);
				gameClock->pause();
			}

			if (buttonGestures.isToggleButtonDisabled()) {
				gameClock->selectPlayerOne();
			} else if (buttonGestures.isToggleButtonEnabled()) {
				gameClock->selectPlayerTwo();
			}
		}
	}

	virtual void render(Clock *clock) {
		lcd2.beginRender(clock);

		switch (menuIndex) {
		case MENU_ITEM_GAME:
			renderGame(clock);
			break;

		case MENU_ITEM_EXIT:
			renderExitItem(clock);
			break;

		case MENU_ITEM_ADD_TIME:
			renderAddTimeItem(clock);
			break;

		case MENU_ITEM_RESTART:
			renderRestartItem(clock);
			break;
		}

		lcd2.endRender();
	}

	void renderGame(Clock *clock) {
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
	}

	void renderExitItem(Clock *clock) {
		lcd2.printTopCenter(gameUiHandlerExitItem);
	}

	void renderAddTimeItem(Clock *clock) {
		lcd2.printTopCenter(gameUiHandlerAddTimeItem);
	}

	void renderRestartItem(Clock *clock) {
		lcd2.printTopCenter(gameUiHandlerRestartItem);
	}

	void setup(TimeControlUi *timeControlUi) {
		this->timeControlUi = timeControlUi;
		started             = false;
		toggleButtonState   = -1;
		menuIndex           = MENU_ITEM_GAME;
	}

private:

	void goBackToStartingHandler() {
		currentUiHandler = startingHandler;
		lcd2.setBlinking(false);
		delete gameClock;
	}

	void onMenuClick() {
		switch (menuIndex) {
		case MENU_ITEM_GAME:
			onGameClick();
			break;

		case MENU_ITEM_EXIT:
			goBackToStartingHandler();
			break;

		case MENU_ITEM_ADD_TIME:
			onAddTimeClick();
			break;

		case MENU_ITEM_RESTART:
			onRestartClick();
			break;
		}
	}

	void onGameClick() {
		if (gameClock->isOver()) {
			goBackToStartingHandler();
		} else if (gameClock->isPaused()) {
			lcd2.setBlinking(false);
			gameClock->resume();
		}
	}

	void onAddTimeClick() {}

	void onRestartClick() {}
};

#endif // ifndef __GameUiHandler_h__
