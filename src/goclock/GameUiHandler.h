
#ifndef __GameUiHandler_h__
#define __GameUiHandler_h__

#include "GameClock.h"
#include "GameClockLcd.h"
#include "UiHandler.h"
#include "TimeControlUi.h"
#include "AddTimeOptionUiHandler.h"
#include "BronsteinDelayTimeControlUi.h"
#include "ByoYomiTimeControlUi.h"
#include "CanadianByoYomiTimeControlUi.h"
#include "FischerDelayTimeControlUi.h"
#include "HourGlassTimeControlUi.h"
#include "SuddenDeathTimeControlUi.h"
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

extern AddTimeOptionUiHandler addTimeOptionUiHandler;
extern UiHandler *currentUiHandler;
extern UiHandler *startingHandler;

const char gameUiHandlerPlayerOneWinnerMessage[] PROGMEM = "WINNER ";
const char gameUiHandlerPlayerTwoWinnerMessage[] PROGMEM = " WINNER";

const char gameUiHandlerExitItem[] PROGMEM = "End Game";
const char gameUiHandlerAddTimeItem[] PROGMEM = "Add time";
const char gameUiHandlerRestartItem[] PROGMEM = "Restart Game";

class GameUiHandler : public UiHandler {
	TimeControlUi *timeControlUi;
	uint8_t timeControlType;
	long *setupValues;
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
		gameClock->tick();
		buttonGestures.tick(clock);
		bool buttonPushed = buttonGestures.wasPushButtonPushed();
		int  travel       = buttonGestures.getNavigationTravel();

		if (buttonGestures.wasPushButtonLongPushed()) {
			goBackToStartingHandler(clock);
			return;
		}

		if (!started && (menuIndex == MENU_ITEM_GAME)) { // < Initialization
			if (toggleButtonState == -1) {
				lcd2.setBlinking(true, clock);
				toggleButtonState = buttonGestures.isToggleButtonDisabled();
			} else if (toggleButtonState != buttonGestures.isToggleButtonDisabled()) {
				lcd2.setBlinking(false, clock);
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
					lcd2.setBlinking(true, clock);
				} else {
					lcd2.setBlinking(false, clock);
				}
			}

			if (buttonPushed) {
				onMenuClick(clock);
			}
		} else if ((menuIndex == MENU_ITEM_GAME) && started && !gameClock->isPaused() && !gameClock->isOver()) { // < Normal play
			if (buttonPushed) {
				lcd2.setBlinking(true, clock);
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

	void setup(uint8_t timeControlType, long setupValues[], Clock *clock) {
		delete this->timeControlUi;

		// delete gameClock;

		switch (timeControlType) {
		case BRONSTEINDELAY_TYPE:
			this->timeControlUi = new BronsteinDelayTimeControlUi();
			break;

		case BYOYOMI_TYPE:
			this->timeControlUi = new ByoYomiTimeControlUi();
			break;

		case CANADIANBYOYOMI_TYPE:
			this->timeControlUi = new CanadianByoYomiTimeControlUi();
			break;

		case FISCHERDELAY_TYPE:
			this->timeControlUi = new FischerDelayTimeControlUi();
			break;

		case HOURGLASS_TYPE:
			this->timeControlUi = new HourGlassTimeControlUi();
			break;

		case SUDDENDEATH_TYPE:
			this->timeControlUi = new SuddenDeathTimeControlUi();
			break;
		}

		gameClock = new GameClock();
		gameClock->setup(clock, timeControlUi->create(setupValues));

		this->timeControlType = timeControlType;
		this->setupValues     = setupValues;
		started               = false;
		toggleButtonState     = -1;
		menuIndex             = MENU_ITEM_GAME;
	}

private:

	void goBackToStartingHandler(Clock *clock) {
		currentUiHandler = startingHandler;
		lcd2.setBlinking(false, clock);
		delete gameClock;
	}

	void onMenuClick(Clock *clock) {
		switch (menuIndex) {
		case MENU_ITEM_GAME:
			onGameClick(clock);
			break;

		case MENU_ITEM_EXIT:
			goBackToStartingHandler(clock);
			break;

		case MENU_ITEM_ADD_TIME:
			onAddTimeClick();
			break;

		case MENU_ITEM_RESTART:
			onRestartClick(clock);
			break;
		}
	}

	void onGameClick(Clock *clock) {
		if (gameClock->isOver()) {
			goBackToStartingHandler(clock);
		} else if (gameClock->isPaused()) {
			lcd2.setBlinking(false, clock);
			gameClock->resume();
		}
	}

	void onAddTimeClick() {
		addTimeOptionUiHandler.setup(timeControlUi);
		addTimeOptionUiHandler.wire(this);
		currentUiHandler = &addTimeOptionUiHandler;
	}

	void onRestartClick(Clock *clock) {
		this->setup(timeControlType, setupValues, clock);
	}
};

#endif // ifndef __GameUiHandler_h__
