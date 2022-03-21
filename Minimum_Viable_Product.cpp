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
AudioEffectEnvelope      envelope1;
AudioEffectEnvelope      envelope2;
AudioEffectEnvelope      envelope3;
AudioEffectEnvelope      envelope4;
AudioEffectEnvelope      envelope5;
AudioEffectEnvelope      envelope6;
AudioMixer4              mix1;
AudioMixer4              mix2;
AudioAmplifier           amp1;
AudioOutputPT8211        dac;
AudioConnection          patchCord1(oscillator1, 0, envelope1, 0);
AudioConnection          patchCord2(oscillator1, 0, envelope2, 0);
AudioConnection          patchCord3(oscillator1, 0, envelope3, 0);
AudioConnection          patchCord4(oscillator1, 0, envelope4, 0);
AudioConnection          patchCord5(oscillator1, 0, envelope5, 0);
AudioConnection          patchCord6(oscillator1, 0, envelope6, 0);
AudioConnection          patchCord7(envelope1, 0, mix1, 0);
AudioConnection          patchCord8(envelope2, 0, mix1, 1);
AudioConnection          patchCord9(envelope3, 0, mix1, 2);
AudioConnection          patchCord10(envelope4, 0, mix2, 0);
AudioConnection          patchCord11(envelope5, 0, mix2, 1);
AudioConnection          patchCord12(envelope6, 0, mix2, 2);
AudioConnection          patchCord13(mix1, 0, mix2, 3);
AudioConnection          patchCord14(mix2, 0, amp1, 0);
AudioConnection          patchCord15(amp1, 0, dac, 0);
AudioConnection          patchCord16(amp1, 0, dac, 1);
// GUItool: end automatically generated code

double freq = 440;
double attack, release;
bool note_on = false;
bool new_note = false;
int type, note, velocity, channel, d1, d2;
std::vector<AudioEffectEnvelope> envelopes;

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

//Setting up for easier polyphony
AudioEffectEnvelope * envelope_used[72];

bool playing [72];

//Playing notes with polyphony
void play_note(const unsigned int note);
void stop_note(const unsigned int note);

void setup()
{
  AudioMemory(18);
  Serial.begin(9600);
  amp1.gain(1.0);
  oscillator1.begin(WAVEFORM_TRIANGLE);
  oscillator1.frequency(freq);
  oscillator1.amplitude(1.0);
  envelopes.push_back(envelope1);
  envelopes.push_back(envelope2);
  envelopes.push_back(envelope3);
  envelopes.push_back(envelope4);
  envelopes.push_back(envelope5);
  envelopes.push_back(envelope6);
  for (auto it = envelopes.begin(); it < envelopes.end(); ++it)
  {
    it->sustain(0.5);
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
        freq = notes[29];
        note = 29;
        break;
      case 28:
        freq = notes[31];
        note = 31;
        break;
      case 29:
        freq = notes[33];
        note = 33;
        break;
      case 30:
        freq = notes[36];
        note = 36;
        break;
    }
    if (digitalRead(note_pins[i]))
    {
      if (!playing[note])
      {
        playing[note] = true;
        attack = pow(analogRead(potentiometers[0]), 3) / pow(1023, 3) * 11800.0;
        release = pow(analogRead(potentiometers[1]), 3) / pow(1023, 3) * 11800.0;
        for (auto it = envelopes.begin(); it < envelopes.end(); ++it)
        {
          it->attack(attack);
          it->release(release);
        }
        play_note(note);
      }
    } else if (playing[note])
    {
      stop_note(note);
      playing[note] = false;
    }
  }
  oscillator1.frequency(freq);
}

//Implement polyphony via a circular array in a vector
void play_note(const unsigned int note)
{
  Serial.println(String("Playing note ") + note);
  std::rotate(envelopes.begin(), envelopes.begin() + 1, envelopes.end());
  AudioEffectEnvelope envelope = envelopes.back();
  envelope_used[note] = &envelope;
  envelope.noteOn();
}
void stop_note(const unsigned int note)
{
  Serial.println(String("Stopping note ") + note);
  AudioEffectEnvelope * envelope = envelope_used[note];
  envelope->noteOff();
}
