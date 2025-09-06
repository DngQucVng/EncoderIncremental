# Documentation

## A Library for Micro-controllers using Arduino IDE

This project is an attempt to implement C++ class with pin interrupt to read signal from encoders. It can do the following:

* Count the number of edges when rotating clockwise and anti-clockwise
* Convert edges to pulses, degrees, revolutions and meters
* Calculated speed in RPM and m/s

## Installation

Here are the steps to install the library:

1. Download ZIP of the project
2. Open Arduino IDE
3. Navigate to Sketch -> Include Library -> Add .ZIP Library...
4. Browse for the downloaded .ZIP file

## Examples

### Position
```cpp
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
}

void loop() {
  now = millis();

  // Print position 5 times per second
  if (now - prePrint >= 200) {
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

```

### Speed
```cpp
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

```