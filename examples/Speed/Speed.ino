#include <EncoderIncremental.h>

#define PIN_A 16
#define PIN_B 17
#define PULSE_PER_REVOLUTION 600

EncoderIncremental myEncoder(PIN_A, PIN_B, PULSE_PER_REVOLUTION);

unsigned long now, prePrint;

void setup() {
  Serial.begin(115200);

  // This must always be called in setup()
  myEncoder.begin();

  // Add this when you have linear translation
  myEncoder.set_meters_per_revolution(0.2); // 0.2m = 200mm

  // Add this to invert positive and negative direction
  myEncoder.invert_direction();

  // You can change the number of times speed is calculated in 1 second, default is 10Hz
  myEncoder.set_speed_sample_rate(20); // 20Hz
}

void loop() {
  now = millis();

  // This must always be called in loop() when calculating speed
  myEncoder.check();

  // Print speed 5 times per second
  if (now - prePrint >= 200) {
    prePrint = now;

    Serial.print("Speed (RPM): ");
    Serial.println(myEncoder.get_speed_rpm());
    Serial.print("Speed (m/s): ");
    Serial.println(myEncoder.get_speed_mps());
  }
}
