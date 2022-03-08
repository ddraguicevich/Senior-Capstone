/*
    Basic_Testing
    Diego Draguicevich
    The purpose of this file is to test the basic functionality
    of the Teensy 4.1 and our chosen DAC by outputting a steady
    sine wave which has user-defined volume via a potentiometer
*/

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <ADC.h>
#include <MIDI.h>

// GUItool: begin automatically generated code
AudioSynthWaveform       oscillator1;
AudioAmplifier           amp1;
AudioOutputPT8211        dac;
AudioConnection          patchCord1(oscillator1, 0, amp1, 0);
AudioConnection          patchCord2(amp1, 0, dac, 0);
AudioConnection          patchCord3(amp1, 0, dac, 1);
// GUItool: end automatically generated code

#define VOLUME_PIN 40
double notes = 
[
  32.7, 34.65, 36.71, 38.89, 41.2, 43.65, 46.25, 49, 51.91, 55, 58.27, 61.74,
  65.41, 69.3, 73.42, 77.78, 82.41, 87.31, 92.5, 98, 103.83, 110, 116.54, 123.47,
  130.81, 138.59, 146.83, 155.56, 164.81, 174.61, 185, 196, 207.65, 220, 233.08, 246.94,
  261.63, 277.18, 293.66, 311.13, 329.63, 349.23, 369.99, 392, 415.3, 440, 466.16, 493.88,
  523.25, 554.37, 587.33, 622.25, 659.25, 698.46, 739.99, 783.99, 830.61, 880, 932.33, 987.77,
  1046.5, 1108.73, 1174.66, 1244.51, 1318.51, 1396.91, 1479.98, 1567.98, 1661.22, 1760, 1864.66, 1975.53
]
double freq = 440;

void setup()
{
    AudioMemory(10);
    Serial.begin(9600);
    amp1.gain(1.0);
    oscillator1.begin(WAVEFORM_TRIANGLE);
}

void loop()
{
    freq = 20 + (analogRead(VOLUME_PIN) / 1023.0) * 2000;
    oscillator1.frequency(freq);
    oscillator1.amplitude(1.0);
}