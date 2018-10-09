
#ifndef __TimeControlBase_h__
#define __TimeControlBase_h__

#include "TimeControl.h"

class TimeControlBase : public TimeControl {
protected:

	TimeTracker *playerOne, *playerTwo;
	uint32_t playersInitialTime;
	bool playerOneWon, playerTwoWon;

	TimeControlBase(uint32_t playersInitialTime) {
		playerOne                = playerTwo = nullptr;
		this->playersInitialTime = playersInitialTime;
		playerOneWon             = playerTwoWon = false;
	}

public:

	virtual ~TimeControlBase() {}

	virtual void setup(TimeTracker *playerOne, TimeTracker *playerTwo) {
		this->playerOne = playerOne;
		this->playerTwo = playerTwo;

		playerOne->setTime(playersInitialTime);
		playerTwo->setTime(playersInitialTime);
	}

	virtual void onPlayerOneBeganToPlay() {}

	virtual void onPlayerOnePlayed()      {}

	virtual void onPlayerOneTimeExpired() {
		playerTwoWon = true;
	}

	virtual void onPlayerTwoBeganToPlay() {}

	virtual void onPlayerTwoPlayed()      {}

	virtual void onPlayerTwoTimeExpired() {
		playerOneWon = true;
	}

	virtual void addTime(int32_t time, uint8_t addTimeType, Clock *clock) {
		time = time * 1000L;

		switch (addTimeType) {
		case ADD_TIME_OPTION_LEFT:
			playerOne->addTime(time);
			break;

		case ADD_TIME_OPTION_RIGHT:
			playerTwo->addTime(time);
			break;

		case ADD_TIME_OPTION_BOTH:
			playerOne->addTime(time);
			playerTwo->addTime(time);
			break;
		}
	}

	virtual int32_t getPlayerOneTime(Clock *clock) {
		return playerOne->getTime(clock);
	}

	virtual int32_t getPlayerTwoTime(Clock *clock) {
		return playerTwo->getTime(clock);
	}

	virtual bool isOver() {
		return playerOneWon || playerTwoWon;
	}
};

#endif // ifndef __TimeControlBase_h__
