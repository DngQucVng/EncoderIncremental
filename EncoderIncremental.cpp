#include "EncoderIncremental.h"

// Initialize the encoder pins with interrupts
void EncoderIncremental::begin() {
	attachInterrupt(digitalPinToInterrupt(PIN_A), std::bind(&EncoderIncremental::isr_a, this), CHANGE);
	attachInterrupt(digitalPinToInterrupt(PIN_B), std::bind(&EncoderIncremental::isr_b, this), CHANGE);
}

// Interrupt Service Routine for when the Pin A change state
void ARDUINO_ISR_ATTR EncoderIncremental::isr_a() {
	if (digitalRead(PIN_A) == digitalRead(PIN_B))
		countEdge++;
	else
		countEdge--;
}

// Interrupt Service Routine for when the Pin B change state
void ARDUINO_ISR_ATTR EncoderIncremental::isr_b() {
	if (digitalRead(PIN_A) != digitalRead(PIN_B))
		countEdge++;
	else
		countEdge--;
}

void EncoderIncremental::check() {
	static uint32_t now = 0, preCheck = 0;

	now = millis();

	// Calculate speed (default 10 times per second)
	if (now - preCheck >= 1000 / checkSpeedFreq) {
		preCheck = now;

		speedRPM = (((float)countEdge - (float)preCountEdge) * checkSpeedFreq * 60.0) / ((float)PPR * 4.0);

		preCountEdge = countEdge;
	}
}