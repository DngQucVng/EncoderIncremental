#include <EncoderIncremental.h>

#define PIN_A 16
#define PIN_B 17
#define PULSE_PER_REVOLUTION 600

EncoderIncremental myEncoder(PIN_A, PIN_B, PULSE_PER_REVOLUTION);

unsigned long now, prePrint;
float printRate = 5;

void setup() {
  Serial.begin(115200);

  // This must always be called in setup()
  myEncoder.begin();

  // Add this when you have linear translation
  myEncoder.set_meters_per_revolution(0.005);
}

void loop() {
  now = micros();

  // This must always be called in loop() when calculating speed
  myEncoder.check();

  // Print position 5 times per second
  if (now - prePrint >= 1e6 / printRate) {
    prePrint = now;

    Serial.print("Speed (RPM): ");
    Serial.println(myEncoder.get_speed_rpm());
    Serial.print("Speed (m/s): ");
    Serial.println(myEncoder.get_speed_mps());
  }
}
