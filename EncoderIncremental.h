#include <Arduino.h>
#include <stdint.h>

#ifdef ARDUINO_ARCH_STM32
	#include <WInterrupts.h>
#elifdef ESP32 || ESP8266 || ARDUINO_ARCH_ESP32 || ARDUINO_ARCH_ESP8266
	#include <FunctionalInterrupt.h>
#endif

class EncoderIncremental {
	public:
		// Constructor
		EncoderIncremental(uint8_t PinA, uint8_t PinB, uint32_t PulsePerRevolution);

		// Destructor
		~EncoderIncremental() {
			detachInterrupt(digitalPinToInterrupt(PIN_A));
			detachInterrupt(digitalPinToInterrupt(PIN_B));
		}

		// Initialize the encoder pins with interrupts
		void begin();

		// Calculate speed
		void check();

		// Return the current position of the encoder in edges, could be positive or negative for clockwise and anti-clockwise
		int32_t get_position_edge() const { return countEdge; }

		// Return the current position of the encoder in pulses, could be positive or negative for clockwise and anti-clockwise
		int32_t get_position_pulse() const { return countEdge / 4; }

		// Return the current position of the encoder in degrees, could be positive or negative for clockwise and anti-clockwise
		float get_position_degree() const { return ((float)countEdge * 360.0) / ((float)PPR * 4.0); }

		// Return the current position of the encoder in revolutions, could be positive or negative for clockwise and anti-clockwise
		float get_position_revolution() const { return ((float)countEdge) / ((float)PPR * 4.0); }

		// Return the current position of the encoder in meters, could be positive or negative for clockwise and anti-clockwise
		float get_position_meter() const { return ((float)countEdge * linearTranslationPerRevolution) / (PPR * 4.0); }

		// Return the speed of encoder in revolutions per minute (RPM), could be positive or negative for clockwise and anti-clockwise
		float get_speed_rpm() const { return speedRPM; }

		// Return the speed of encoder in meters per second (m/s), could be positive or negative for clockwise and anti-clockwise
		float get_speed_mps() const { return (speedRPM * linearTranslationPerRevolution) / (60.0); }

		// Invert the positive and negative direction (clockwise and anti-clockwise)
		void invert_direction() { direction = !direction; }

		// Set the pulse counter to zero
		void reset_position() { countEdge = preCountEdge = 0; }

		// Set how many meters is translated when one revolution is rotated
		void set_meters_per_revolution(float Meters) { linearTranslationPerRevolution = Meters; }

		// Set how many times speed is calculated in one second, default is 10
		void set_speed_sample_rate(float Frequency) { speedSampleRate = Frequency; }

	private:
		const uint8_t PIN_A, PIN_B;
		const uint32_t PPR;

		volatile int32_t countEdge, preCountEdge = 0;

		float speedRPM = 0.0, speedSampleRate = 10.0, linearTranslationPerRevolution = 0.0;
		bool direction = false;

		#ifdef ESP32 || ESP8266 || ARDUINO_ARCH_ESP32 || ARDUINO_ARCH_ESP8266
			void ARDUINO_ISR_ATTR isr_a();
			void ARDUINO_ISR_ATTR isr_b();
		#else
			void isr_a();
			void isr_b();
		#endif
};