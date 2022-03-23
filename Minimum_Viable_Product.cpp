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
#include <algorithm>
#include <vector>

// GUItool: begin automatically generated code
AudioSynthWaveform       oscillator1;
AudioSynthWaveform       oscillator2;
AudioSynthWaveform       oscillator3;
AudioSynthWaveform       oscillator4;
AudioSynthWaveform       oscillator5;
AudioSynthWaveform       oscillator6;
AudioSynthWaveform       oscillator7;
AudioSynthWaveform       oscillator8;
AudioSynthWaveform       oscillator9;
AudioSynthWaveform       oscillator10;
AudioSynthWaveform       oscillator11;
AudioSynthWaveform       oscillator12;
AudioSynthWaveform       oscillator13;
AudioSynthWaveform       oscillator14;
AudioSynthWaveform       oscillator15;
AudioSynthWaveform       oscillator16;
AudioSynthWaveform       oscillator17;
AudioSynthWaveform       oscillator18;
AudioEffectEnvelope      envelope1;
AudioEffectEnvelope      envelope2;
AudioEffectEnvelope      envelope3;
AudioEffectEnvelope      envelope4;
AudioEffectEnvelope      envelope5;
AudioEffectEnvelope      envelope6;
AudioMixer4              mix1;
AudioMixer4              mix2;
AudioMixer4              mix3;
AudioMixer4              mix4;
AudioMixer4              mix5;
AudioMixer4              mix6;
AudioMixer4              mix7;
AudioMixer4              mix8;
AudioAmplifier           amp1;
AudioOutputPT8211        dac;
AudioConnection          patchCord1(oscillator1, 0, mix1, 0);
AudioConnection          patchCord2(oscillator2, 0, mix1, 1);
AudioConnection          patchCord3(oscillator3, 0, mix1, 2);
AudioConnection          patchCord4(oscillator4, 0, mix2, 0);
AudioConnection          patchCord5(oscillator5, 0, mix2, 1);
AudioConnection          patchCord6(oscillator6, 0, mix2, 2);
AudioConnection          patchCord7(oscillator7, 0, mix3, 0);
AudioConnection          patchCord8(oscillator8, 0, mix3, 1);
AudioConnection          patchCord9(oscillator9, 0, mix3, 2);
AudioConnection          patchCord10(oscillator10, 0, mix4, 0);
AudioConnection          patchCord11(oscillator11, 0, mix4, 1);
AudioConnection          patchCord12(oscillator12, 0, mix4, 2);
AudioConnection          patchCord13(oscillator13, 0, mix5, 0);
AudioConnection          patchCord14(oscillator14, 0, mix5, 1);
AudioConnection          patchCord15(oscillator15, 0, mix5, 2);
AudioConnection          patchCord16(oscillator16, 0, mix6, 0);
AudioConnection          patchCord17(oscillator17, 0, mix6, 1);
AudioConnection          patchCord18(oscillator18, 0, mix6, 2);
AudioConnection          patchCord19(mix1, 0, envelope1, 0);
AudioConnection          patchCord20(mix2, 0, envelope2, 0);
AudioConnection          patchCord21(mix3, 0, envelope3, 0);
AudioConnection          patchCord22(mix4, 0, envelope4, 0);
AudioConnection          patchCord23(mix5, 0, envelope5, 0);
AudioConnection          patchCord24(mix6, 0, envelope6, 0);
AudioConnection          patchCord25(envelope1, 0, mix7, 0);
AudioConnection          patchCord26(envelope2, 0, mix7, 1);
AudioConnection          patchCord27(envelope3, 0, mix7, 2);
AudioConnection          patchCord28(envelope4, 0, mix8, 0);
AudioConnection          patchCord29(envelope5, 0, mix8, 1);
AudioConnection          patchCord30(envelope6, 0, mix8, 2);
AudioConnection          patchCord31(mix7, 0, mix8, 3);
AudioConnection          patchCord32(mix8, 0, amp1, 0);
AudioConnection          patchCord33(amp1, 0, dac, 0);
AudioConnection          patchCord34(amp1, 0, dac, 1);
// GUItool: end automatically generated code

double attack, decay, sustain, release;
bool note_on = false;
bool new_note = false;
int type, note, velocity, channel, d1, d2;

unsigned int potentiometers[]
{
  39, 40
};

unsigned int note_pins[]
{
  27, 28, 29, 30
};

//Setup for actual note frequencies since MIDI only gives 0-255
double notes[]
{
  32.7, 34.65, 36.71, 38.89, 41.2, 43.65, 46.25, 49, 51.91, 55, 58.27, 61.74,
  65.41, 69.3, 73.42, 77.78, 82.41, 87.31, 92.5, 98, 103.83, 110, 116.54, 123.47,
  130.81, 138.59, 146.83, 155.56, 164.81, 174.61, 185, 196, 207.65, 220, 233.08, 246.94,
  261.63, 277.18, 293.66, 311.13, 329.63, 349.23, 369.99, 392, 415.3, 440, 466.16, 493.88,
  523.25, 554.37, 587.33, 622.25, 659.25, 698.46, 739.99, 783.99, 830.61, 880, 932.33, 987.77,
  1046.5, 1108.73, 1174.66, 1244.51, 1318.51, 1396.91, 1479.98, 1567.98, 1661.22, 1760, 1864.66, 1975.53
};

//Setting up for polyphony
AudioEffectEnvelope * envelopes[6]
{
  &envelope1, &envelope2, &envelope3, &envelope4, &envelope5, &envelope6
};
AudioSynthWaveform * oscillators[18]
{
  &oscillator1, &oscillator2, &oscillator3,
  &oscillator4, &oscillator5, &oscillator6,
  &oscillator7, &oscillator8, &oscillator9,
  &oscillator10, &oscillator11, &oscillator12,
  &oscillator13, &oscillator14, &oscillator15,
  &oscillator16, &oscillator17, &oscillator18
};
bool available [6]
{
  true, true, true, true, true, true
};
int playing [72];

//Playing notes with polyphony
void play_note(const unsigned int note);
void stop_note(const unsigned int note);

void setup()
{
  AudioMemory(18);
  Serial.begin(9600);
  amp1.gain(1.0);

  for (unsigned int i = 0; i < 6; ++i)
  {
    oscillators[i]->begin(1.0, 440.0, WAVEFORM_TRIANGLE);
  }
  
  for (unsigned int i = 0; i < sizeof(note_pins) / sizeof(int); ++i)
  {
    pinMode(note_pins[i], INPUT);
  }
}

void loop()
{
  //MIDI stuff, falls under TODO
  //  if (usbMIDI.read())
  //  {
  //    byte type = usbMIDI.getType();
  //    switch (type)
  //    {
  //      case midi::NoteOn:
  //        note = usbMIDI.getData1();
  //        velocity = usbMIDI.getData2();
  //        channel = usbMIDI.getChannel();
  //        if (velocity > 0)
  //        {
  //          Serial.println(String("Note On: ") + note + String(", velocity: ") + velocity);
  //        } else
  //        {
  //          Serial.println(String("Note Off: ") + note + String(", velocity: ") + velocity);
  //        }
  //        break;
  //      case midi::NoteOff:
  //        note = usbMIDI.getData1();
  //        velocity = usbMIDI.getData2();
  //        channel = usbMIDI.getChannel();
  //        Serial.println(String("Note Off: ") + note + String(", velocity: ") + velocity);
  //        break;
  //      default:
  //        d1 = usbMIDI.getData1();
  //        d2 = usbMIDI.getData2();
  //        Serial.println(String("Got non-note data: ") + d1 + String(", ") + d2);
  //        break;
  //    }
  //  }

  for (unsigned int i = 0; i < sizeof(note_pins) / sizeof(int); ++i)
  {
    switch (note_pins[i])
    {
      case 27:
        note = 29;
        break;
      case 28:
        note = 31;
        break;
      case 29:
        note = 33;
        break;
      case 30:
        note = 36;
        break;
    }
    if (digitalRead(note_pins[i]))
    {
      if (!playing[note])
      {
        attack = pow(analogRead(potentiometers[0]), 3) / pow(1023, 3) * 11800.0;
        release = pow(analogRead(potentiometers[1]), 3) / pow(1023, 3) * 11800.0;
        for (unsigned int i = 0; i < 6; ++i)
        {
          envelopes[i]->attack(attack);
          envelopes[i]->release(release);
        }
        play_note(note);
      }
    } else if (playing[note])
    {
      stop_note(note);
    }
  }
}

//Implement polyphony via a circular array in a vector
void play_note(const unsigned int note)
{
  Serial.println(String("Playing note ") + note);
  unsigned int i = 0;
  while (i < 6)
  {
    if (available[i])
    {
      for (unsigned int j = 3 * i; j < 3 * i + 3; ++j)
      {
        oscillators[j]->frequency(notes[note]);
      }
      playing[note] = i + 1;
      envelopes[i]->noteOn();
      available[i] = false;
      break;
    }
    ++i;
  }
}
void stop_note(const unsigned int note)
{
  unsigned int envelope = playing[note] - 1;
  envelopes[envelope]->noteOff();
  available[envelope] = true;
  playing[note] = 0;
}
