
#ifndef __ByoYomiTimeControl_h__
#define __ByoYomiTimeControl_h__

#include "TimeControlBase.h"

#define BYO_YOMI_SETUP_MAX_PERIODS  10

struct ByoYomiPeriod {
	uint16_t numberOfPlays;
	uint32_t time;

	ByoYomiPeriod() {
		numberOfPlays = 0;
		time          = 0;
	}
};

struct ByoYomiSetup {
	uint32_t      time;
	uint32_t      periodTime;
	uint16_t      numberOfPeriods;
	uint16_t      numberOfPlays;
	ByoYomiPeriod periods[BYO_YOMI_SETUP_MAX_PERIODS];
};


class ByoYomiTimeControl : public TimeControlBase {
	class PlayerState;

public:

	ByoYomiTimeControl(ByoYomiSetup byoYomiSetup) : TimeControlBase(byoYomiSetup.time), setup(byoYomiSetup),
		playerOneState(&setup), playerTwoState(&setup) {}

	virtual ~ByoYomiTimeControl() {}

	// <TimeControl>

	virtual void onPlayerOnePlayed() {
		onPlayerPlayed(playerOne, &playerOneState);
	}

	virtual void onPlayerOneTimeExpired() {
		onPlayerTimeExpired(playerOne, &playerOneState);
	}

	virtual void onPlayerTwoPlayed() {
		onPlayerPlayed(playerTwo, &playerTwoState);
	}

	virtual void onPlayerTwoTimeExpired() {
		onPlayerTimeExpired(playerTwo, &playerTwoState);
	}

	virtual void addTime(int32_t time, uint8_t addTimeType, Clock *clock) {
		time = time * 1000L;

		if ((addTimeType == ADD_TIME_OPTION_LEFT) || (addTimeType == ADD_TIME_OPTION_BOTH)) {
			addPlayerTime(time, playerOne, &playerOneState, clock);
		}

		if ((addTimeType == ADD_TIME_OPTION_RIGHT) || (addTimeType == ADD_TIME_OPTION_BOTH)) {
			addPlayerTime(time, playerTwo, &playerTwoState, clock);
		}
	}

	void addPlayerTime(int32_t time, TimeTracker *player, PlayerState *playerState, Clock *clock) {
		if (playerState->isOnByoYomi()) {
			int32_t currentTime = player->getTime(clock);
			int32_t timeLeft    = playerState->addTime(time, currentTime);
			player->addTime(timeLeft - currentTime);
		} else {
			player->addTime(time);
		}
	}

	// </TimeControl>

	bool isPlayerOneInNormalTime() {
		return !isPlayerOnByoYomi(&playerOneState);
	}

	uint16_t getPlayerOneRemainingByoYomiPeriods() {
		return setup.numberOfPeriods - playerOneState.getPeriod();
	}

	uint16_t getPlayerOneRemainingNumberOfPlays() {
		return setup.periods[playerOneState.getPeriod()].numberOfPlays - playerOneState.getNumberOfPlays();
	}

	bool isPlayerTwoInNormalTime() {
		return !isPlayerOnByoYomi(&playerTwoState);
	}

	uint16_t getPlayerTwoRemainingByoYomiPeriods() {
		return setup.numberOfPeriods - playerTwoState.getPeriod();
	}

	uint16_t getPlayerTwoRemainingNumberOfPlays() {
		return setup.periods[playerTwoState.getPeriod()].numberOfPlays - playerTwoState.getNumberOfPlays();
	}

private:

	ByoYomiTimeControl() : TimeControlBase(0), setup(), playerOneState(nullptr), playerTwoState(nullptr) {}

	void onPlayerPlayed(TimeTracker *player, PlayerState *playerState) {
		if (!isPlayerOnByoYomi(playerState)) {
			return;
		}
		playerIncrementNumberOfPlays(playerState);

		if (hasPlayerReachedTargetNumberOfPlays(playerState)) {
			playerRenewByoYomiPeriod(player, playerState);
		}
	}

	bool isPlayerOnByoYomi(PlayerState *playerState) {
		return playerState->isOnByoYomi();
	}

	void playerIncrementNumberOfPlays(PlayerState *playerState) {
		playerState->incrementNumberOfPlays();
	}

	bool hasPlayerReachedTargetNumberOfPlays(PlayerState *playerState) {
		return playerState->hasReachedNumberOfPlays();
	}

	void playerRenewByoYomiPeriod(TimeTracker *player, PlayerState *playerState) {
		playerState->resetNumberOfPlays();
		setPlayerTime(player, playerState);
	}

	void onPlayerTimeExpired(TimeTracker *player, PlayerState *playerState) {
		if (!isPlayerOnByoYomi(playerState)) {
			playerNormalPeriodEnded(playerState);
		} else {
			playerMoveToNextByoYomiPeriod(playerState);
		}
		playerBeginByoYomiPeriodOrEndGame(player, playerState);
	}

	void playerNormalPeriodEnded(PlayerState *playerState) {
		playerState->beginByoYomi();
	}

	void playerMoveToNextByoYomiPeriod(PlayerState *playerState) {
		playerState->nextByoYomi();
	}

	void playerBeginByoYomiPeriodOrEndGame(TimeTracker *player, PlayerState *playerState) {
		if (playerState->isInValidByoYomiPeriod()) {
			setPlayerTime(player, playerState);
		} else {
			if (playerOne == player) {
				playerTwoWon = true;
			} else {
				playerOneWon = true;
			}
		}
	}

	void setPlayerTime(TimeTracker *player, PlayerState *playerState) {
		player->setTime(playerState->getTime());
	}

	class PlayerState {
		bool onByoYomi;
		int period;
		int numberOfPlays;
		ByoYomiSetup *setup;

public:

		PlayerState(ByoYomiSetup *setup) {
			this->setup   = setup;
			onByoYomi     = false;
			period        = 0;
			numberOfPlays = 0;
		}

		int32_t addTime(int32_t addTime, int32_t currentTime) {
			int32_t timeLeft   = addTime;
			int     periodPlus = 0;

			periodPlus = ((currentTime + addTime) / setup->periodTime);

			if (periodPlus > period) {
				periodPlus = period;
			}

			period = period - periodPlus;


			if (addTime + currentTime > (periodPlus + 1) * setup->periodTime) {
				timeLeft      = addTime + currentTime - (periodPlus + 1) * setup->periodTime;
				onByoYomi     = false;
				numberOfPlays = 0;
			} else {
				timeLeft      = (addTime + currentTime) % setup->periodTime;
				numberOfPlays = (setup->periodTime - timeLeft) * setup->numberOfPlays /
				                setup->periodTime;
			}

			return timeLeft;
		}

		bool isOnByoYomi() {
			return onByoYomi;
		}

		void incrementNumberOfPlays() {
			numberOfPlays++;
		}

		bool hasReachedNumberOfPlays() {
			return numberOfPlays == setup->periods[period].numberOfPlays;
		}

		void resetNumberOfPlays() {
			numberOfPlays = 0;
		}

		uint32_t getTime() {
			return setup->periods[period].time;
		}

		void beginByoYomi() {
			onByoYomi = true;
		}

		void nextByoYomi() {
			period++;
			resetNumberOfPlays();
		}

		bool isInValidByoYomiPeriod() {
			return period < setup->numberOfPeriods;
		}

		uint16_t getNumberOfPlays() {
			return numberOfPlays;
		}

		uint16_t getPeriod() {
			return period;
		}
	};

	ByoYomiSetup setup;
	PlayerState playerOneState, playerTwoState;
};

#endif // ifndef __ByoYomiTimeControl_h__
