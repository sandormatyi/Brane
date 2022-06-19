#include <Arduino.h>
#include <MIDI.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, MIDI_OUT_S2);
PulseOximeter pox;

void onPulseDetected()
{
  for (int i = 0; i < 16; ++i) {
    MIDI_OUT_S2.sendClock();
    delay(1);
  }

  Serial.println("Pulse detected");
  Serial.printf("BPM: %f\n", pox.getHeartRate());
  Serial.printf("SPO2: %u\n", uint32_t(pox.getSpO2()));
}

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 5000)
    ;

  MIDI_OUT_S2.begin(MIDI_CHANNEL_OMNI);

  if (!pox.begin()) {
    Serial.println("ERROR: Failed to initialize pulse oximeter");
    while(true)
      ;
  }

pox.setIRLedCurrent(LEDCurrent::MAX30100_LED_CURR_50MA);
  pox.setOnBeatDetectedCallback(onPulseDetected);
}

void loop() {
  pox.update();
  // Serial.println("Note on");
  // MIDI_OUT_S2.sendNoteOn(36, 0xff, 1);
  // delay(1000);
  // Serial.println("Note off");
  // MIDI_OUT_S2.sendNoteOff(36, 0xff, 1);
  // delay(1000);
}