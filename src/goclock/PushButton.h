
#ifndef __PushButton_h__
#define __PushButton_h__

#include <Arduino.h>

#include "base.h"
#include "SignalFilter.h"

class PushButton {
	SignalFilter filter;
	SignalFilter longFilter;
	uint8_t pin;
	bool wasDown;
	bool waitUp;

public:

	PushButton(uint8_t pin) : filter(20), longFilter(20, 2000) {
		this->pin = pin;
		wasDown   = false;
		waitUp    = false;
	}

	void init() {
		pinMode(pin, INPUT);
		digitalWrite(pin, HIGH);
	}

	void tick(Clock *clock) {
		filter.tick(clock, digitalRead(pin));

		if (!wasDown && isDown()) {
			wasDown = true;
		}

		longFilter.tick(clock, !filter.isOn());
	}

	bool isUp() {
		return !isDown();
	}

	bool isDown() {
		return filter.isOn();
	}

	bool wasPushed() {
		if (wasDown) {
			if (isUp()) {
				wasDown = false;

				if (waitUp) {
					waitUp = false;
					return false;
				}
			}
			return !wasDown;
		}
		return false;
	}

	bool wasLongPushed() {
		if (longFilter.isOn() && !waitUp) {
			waitUp = true;
			return true;
		}
		return false;
	}
};

#endif // ifndef __PushButton_h__
