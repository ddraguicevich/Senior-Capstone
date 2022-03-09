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
AudioEffectEnvelope      envelope1;
AudioAmplifier           amp1;
AudioOutputPT8211        dac;
AudioConnection          patchCord1(oscillator1, 0, envelope1, 0);
AudioConnection          patchCord2(envelope1, 0, amp1, 0);
AudioConnection          patchCord3(amp1, 0, dac, 0);
AudioConnection          patchCord4(amp1, 0, dac, 1);
// GUItool: end automatically generated code

double freq = 440;
bool note_on = false;
bool new_note = false;

int potentiometers[]
{
  39, 40
};

int note_pins[]
{
  27, 28, 29, 30
};

double notes[]
{
  32.7, 34.65, 36.71, 38.89, 41.2, 43.65, 46.25, 49, 51.91, 55, 58.27, 61.74,
  65.41, 69.3, 73.42, 77.78, 82.41, 87.31, 92.5, 98, 103.83, 110, 116.54, 123.47,
  130.81, 138.59, 146.83, 155.56, 164.81, 174.61, 185, 196, 207.65, 220, 233.08, 246.94,
  261.63, 277.18, 293.66, 311.13, 329.63, 349.23, 369.99, 392, 415.3, 440, 466.16, 493.88,
  523.25, 554.37, 587.33, 622.25, 659.25, 698.46, 739.99, 783.99, 830.61, 880, 932.33, 987.77,
  1046.5, 1108.73, 1174.66, 1244.51, 1318.51, 1396.91, 1479.98, 1567.98, 1661.22, 1760, 1864.66, 1975.53
};

void setup()
{
  AudioMemory(10);
  Serial.begin(9600);
  amp1.gain(1.0);
  oscillator1.begin(WAVEFORM_TRIANGLE);
  oscillator1.frequency(freq);
  oscillator1.amplitude(1.0);
  envelope1.sustain(0.5);
  for (int i = 0; i < sizeof(note_pins) / sizeof(int); ++i)
  {
    pinMode(note_pins[i], INPUT);
  }
}

void loop()
{
  note_on = false;
  for (int i = 0; i < sizeof(note_pins) / sizeof(int); ++i)
  {
    if(digitalRead(note_pins[i]))
    {
      switch (note_pins[i])
      {
        case 27:
          freq = notes[29];
          break;
        case 28:
          freq = notes[31];
          break;
        case 29:
          freq = notes[33];
          break;
        case 30:
          freq = notes[36];
          break;
      }
      note_on = true;
      if (!new_note)
      {
        new_note = true;
        envelope1.attack(pow(analogRead(potentiometers[0]), 3) / pow(1023, 3) * 11800.0);
        envelope1.release(pow(analogRead(potentiometers[1]), 3) / pow(1023, 3) * 11800.0);
        envelope1.noteOn();
      }
    }
  }
  if (!note_on)
  {
    new_note = false;
    envelope1.noteOff();
  }
  oscillator1.frequency(freq);
}