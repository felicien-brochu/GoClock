
#ifndef __SignalFilter_h__
#define __SignalFilter_h__

#include "Clock.h"

class SignalFilter {
	uint32_t threshold1;
	uint32_t threshold2;
	uint32_t changeTime;

	bool state;
	bool filtering;

public:

	SignalFilter(uint32_t threshold) : SignalFilter(threshold, threshold) {}

	SignalFilter(uint32_t threshold1, uint32_t threshold2) {
		this->threshold1 = threshold1;
		this->threshold2 = threshold2;
		state            = true;
		filtering        = false;
	}

	void tick(Clock *clock, bool currentSignal) {
		if (state == currentSignal) {
			if (isFiltering()) {
				stopFiltering();
			}
		} else {
			if (isFiltering()) {
				if (thresholdPassed(clock)) {
					changeSignal();
					stopFiltering();
				}
			} else {
				beginFiltering(clock);
			}
		}
	}

	bool isOn() {
		return !state;
	}

private:

	void beginFiltering(Clock *clock) {
		filtering  = true;
		changeTime = clock->currentTime();
	}

	void stopFiltering() {
		filtering = false;
	}

	bool isFiltering() {
		return filtering;
	}

	bool thresholdPassed(Clock *clock) {
		uint32_t threshold = state ? threshold2 : threshold1;

		return (clock->currentTime() - changeTime) > threshold;
	}

	void changeSignal() {
		state = !state;
	}
};

#endif // ifndef __SignalFilter_h__
