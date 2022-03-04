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

// GUItool: begin automatically generated code
AudioSynthWaveform       oscillator1;
AudioOutputI2S           dac;
AudioConnection          patchCord1(oscillator1, 0, dac, 0);
AudioConnection          patchCord2(oscillator1, 0, dac, 1);
// GUItool: end automatically generated code

#define VOLUME_PIN 40
double volume = 0.9;
double freq = 440;

void setup()
{
    AudioMemory(10);
    Serial.begin(9600);
    oscillator1.begin(WAVEFORM_SINE);
}

void loop()
{
    volume = analogRead(VOLUME_PIN) / 1023.0;
    oscillator1.frequency(freq);
    oscillator1.amplitude(volume);
}