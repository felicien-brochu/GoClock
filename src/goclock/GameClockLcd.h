
#ifndef __GameClockLcd_h__
#define __GameClockLcd_h__

#include <LiquidCrystal.h>

#define GAME_CLOCK_LCD_BUFFER_SIZE 17

class GameClockLcd {
	enum Alignment { Left, Center, Right };

	LiquidCrystal lcd;
	char topBufferA[GAME_CLOCK_LCD_BUFFER_SIZE];
	char topBufferB[GAME_CLOCK_LCD_BUFFER_SIZE];
	char bottomBufferA[GAME_CLOCK_LCD_BUFFER_SIZE];
	char bottomBufferB[GAME_CLOCK_LCD_BUFFER_SIZE];

	uint32_t blinkToggleTime;
	bool blink, blinkToggle;
	char *topBuffer;
	char *bottomBuffer;

public:

	GameClockLcd(uint8_t rs, uint8_t enable, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3) : lcd(rs,
		                                                                                       enable,
		                                                                                       d0,
		                                                                                       d1,
		                                                                                       d2,
		                                                                                       d3) {
		topBuffer       = topBufferA;
		bottomBuffer    = bottomBufferA;
		blinkToggleTime = 0;
		blink           = false;
		blinkToggle     = true;
	}

	void init() {
		lcd.begin(16, 2);
	}

	void beginRender(Clock *clock) {
		swapBuffers();
		clearBuffer(topBuffer);
		clearBuffer(bottomBuffer);
		updateBlinkState(clock);
	}

	void printTopLeft(const char *str) {
		printAligned(str, topBuffer, Left);
	}

	void printTopCenter(const char *str) {
		printAligned(str, topBuffer, Center);
	}

	void printTopRight(const char *str) {
		printAligned(str, topBuffer, Right);
	}

	void printBottomLeft(const char *str) {
		printAligned(str, bottomBuffer, Left);
	}

	void printBottomCenter(const char *str) {
		printAligned(str, bottomBuffer, Center);
	}

	void printBottomRight(const char *str) {
		printAligned(str, bottomBuffer, Right);
	}

	void printWholeScreen(const char *str) {
		if (notBlinkingOrBlinkOnFase()) {
			memcpy_P(topBuffer,    str,                                  GAME_CLOCK_LCD_BUFFER_SIZE - 1);
			memcpy_P(bottomBuffer, &str[GAME_CLOCK_LCD_BUFFER_SIZE - 1], GAME_CLOCK_LCD_BUFFER_SIZE - 1);
		}
	}

	void printTopLeftTime(int32_t time, bool blink) {
		formatTime(time, topBuffer, blink);
	}

	void printTopRightTime(int32_t time, bool blink) {
		formatTime(time, &topBuffer[11], blink);
	}

	void sPrintBottomLeft(const char *format, ...) {
		va_list body;

		va_start(body, format);
		sPrint(bottomBuffer, Left, format, body);
		va_end(body);
	}

	void sPrintBottomCenter(const char *format, ...) {
		va_list body;

		va_start(body, format);
		sPrint(bottomBuffer, Center, format, body);
		va_end(body);
	}

	void sPrintBottomRight(const char *format, ...) {
		va_list body;

		va_start(body, format);
		sPrint(bottomBuffer, Right, format, body);
		va_end(body);
	}

	void setBlinking(bool blinking) {
		blink = blinking;
	}

	void endRender() {
		if (isBufferDirty(topBufferA, topBufferB)) {
			lcd.setCursor(0, 0);
			lcd.print(topBuffer);
		}

		if (isBufferDirty(bottomBufferA, bottomBufferB)) {
			lcd.setCursor(0, 1);
			lcd.print(bottomBuffer);
		}
	}

private:

	void clearBuffer(char *buffer) {
		int16_t lastPosition = GAME_CLOCK_LCD_BUFFER_SIZE - 1;

		memset(buffer, ' ', lastPosition);
		buffer[lastPosition] = '\0';
	}

	void printAligned(const char *str, char *screenBuffer, Alignment aligment) {
		char buffer[GAME_CLOCK_LCD_BUFFER_SIZE];

		strcpy_P(buffer, str);
		printAlignedUsingLocalBuffer(buffer, screenBuffer, aligment);
	}

	void printAlignedUsingLocalBuffer(const char *buffer, char *screenBuffer, Alignment aligment) {
		int16_t length = strlen(buffer);
		int16_t index  = 0;

		if (aligment == Center) {
			index = (GAME_CLOCK_LCD_BUFFER_SIZE - length) / 2;
		} else if (aligment == Right) {
			index = GAME_CLOCK_LCD_BUFFER_SIZE - 1 - length;
		}

		if (notBlinkingOrBlinkOnFase()) {
			memcpy(&screenBuffer[index], buffer, length);
		}
	}

	void formatTime(int32_t time, char *buffer, bool blink) {
		if (time <= 0) {
			formatTimeNibble(0, 0, buffer, blink);
			return;
		}
		int32_t hours   = time / (1000L * 60L * 60L);
		int32_t minutes = (time % (1000L * 60L * 60L)) / (1000L * 60L);
		int32_t seconds = ((time % (1000L * 60L * 60L)) % (1000L * 60L)) / 1000L;

		// Serial.print( "Time: " );Serial.print( time );Serial.print( " hours: " );Serial.print( hours );Serial.print( " minutes: " );Serial.print( minutes );Serial.print( " seconds: " );Serial.println( seconds );
		if (hours > 0) {
			formatTimeNibble(hours, minutes, buffer, blink);
		} else {
			formatTimeNibble(minutes, seconds, buffer, blink);
		}
	}

	void formatTimeNibble(int32_t left, int32_t right, char *buffer, bool blink) {
		if (notBlinkingOrBlinkOnFase()) {
			buffer[0] = (char)('0' + (left / 10));
			buffer[1] = (char)('0' + (left % 10));

			if (!blink || blink && blinkToggle) {
				buffer[2] = ':';
			}
			buffer[3] = (char)('0' + (right / 10));
			buffer[4] = (char)('0' + (right % 10));
		}
	}

	void sPrint(char *screenBuffer, Alignment aligment, const char *format, va_list body) {
		char buffer[GAME_CLOCK_LCD_BUFFER_SIZE];

		vsprintf_P(buffer, format, body);
		printAlignedUsingLocalBuffer(buffer, screenBuffer, aligment);
	}

	void swapBuffers() {
		topBuffer    = topBuffer != topBufferA ? topBufferA : topBufferB;
		bottomBuffer = bottomBuffer != bottomBufferA ? bottomBufferA : bottomBufferB;
	}

	bool isBufferDirty(char *bufferA, char *bufferB) {
		for (int16_t i = 0; i < GAME_CLOCK_LCD_BUFFER_SIZE; ++i) {
			if (bufferA[i] != bufferB[i]) {
				return true;
			}
		}
		return false;
	}

	void updateBlinkState(Clock *clock) {
		uint32_t currentTime = clock->currentTime();

		if (currentTime > blinkToggleTime) {
			blinkToggleTime = currentTime + 250;
			blinkToggle     = !blinkToggle;
		}
	}

	bool notBlinkingOrBlinkOnFase() {
		return !blink || (blink && blinkToggle);
	}
};

#endif // ifndef __GameClockLcd_h__
