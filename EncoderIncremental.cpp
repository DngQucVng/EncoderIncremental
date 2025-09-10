#include "EncoderIncremental.h"

// Constructor
EncoderIncremental::EncoderIncremental(uint8_t PinA, uint8_t PinB, uint32_t PulsePerRevolution) : PIN_A(PinA), PIN_B(PinB), PPR(PulsePerRevolution) {
	pinMode(PIN_A, INPUT);
	pinMode(PIN_B, INPUT);
}

// Initialize the encoder pins with interrupts
void EncoderIncremental::begin() {
	attachInterrupt(digitalPinToInterrupt(PIN_A), std::bind(&EncoderIncremental::isr_a, this), CHANGE);
	attachInterrupt(digitalPinToInterrupt(PIN_B), std::bind(&EncoderIncremental::isr_b, this), CHANGE);
}

#ifdef ESP32 || ESP8266 || ARDUINO_ARCH_ESP32 || ARDUINO_ARCH_ESP8266
// Interrupt Service Routine for when the Pin A change state
void ARDUINO_ISR_ATTR EncoderIncremental::isr_a() {
	if (digitalRead(PIN_A) == digitalRead(PIN_B))
		if (direction) countEdge++; else countEdge--;
	else
		if (direction) countEdge--; else countEdge++;
}

// Interrupt Service Routine for when the Pin B change state
void ARDUINO_ISR_ATTR EncoderIncremental::isr_b() {
	if (digitalRead(PIN_A) != digitalRead(PIN_B))
		if (direction) countEdge++; else countEdge--;
	else
		if (direction) countEdge--; else countEdge++;
}
#else
// Interrupt Service Routine for when the Pin A change state
void EncoderIncremental::isr_a() {
	if (digitalRead(PIN_A) == digitalRead(PIN_B))
		if (direction) countEdge++; else countEdge--;
	else
		if (direction) countEdge--; else countEdge++;
}

// Interrupt Service Routine for when the Pin B change state
void EncoderIncremental::isr_b() {
	if (digitalRead(PIN_A) != digitalRead(PIN_B))
		if (direction) countEdge++; else countEdge--;
	else
		if (direction) countEdge--; else countEdge++;
}
#endif

void EncoderIncremental::check() {
	static uint32_t now = 0, preCheck = 0;

	now = millis();

	if (now - preCheck >= 1000 / speedSampleRate) {
		preCheck = now;

		speedRPM = (((float)countEdge - (float)preCountEdge) * speedSampleRate * 60.0) / ((float)PPR * 4.0);

		preCountEdge = countEdge;
	}
}