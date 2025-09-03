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

  // Print position 5 times per second
  if (now - prePrint >= 1e6 / printRate) {
    prePrint = now;

    Serial.print("Number of pulses: ");            Serial.println(myEncoder.get_position_pulse());
    Serial.print("Number of edges: ");             Serial.println(myEncoder.get_position_edge());
    Serial.print("Number of degrees: ");           Serial.println(myEncoder.get_position_degree());
    Serial.print("Number of revolutions: ");       Serial.println(myEncoder.get_position_revolution());
    Serial.print("Number of meters translated: "); Serial.println(myEncoder.get_position_meter());
  }

  // Listen for Serial data to reset position to zero
  if (Serial.available()) {
    String data = Serial.readStringUntil('\n');

    if (data == "1") {
      myEncoder.reset_position();
    }
  }
}
