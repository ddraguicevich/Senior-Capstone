/*
    Basic_Testing
    Diego Draguicevich
    The purpose of this file is to implement SASS's minimum viable product
    of 6-note polyphony, MIDI input, envelope control, and waveform control
*/

#include <Audio.h>        //PJRC's audio library
#include <Wire.h>         //Basic GPIO
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <ADC.h>          //For reading in potentiometers
#include <MIDI.h>         //MIDI library for MIDI input 
#include <Bounce.h>       //Debouncing buttons
#include <USBHost_t36.h>  //Necessary to connect things to the USB host

// From https://www.pjrc.com/teensy/gui/
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
AudioMixer4              mix9;
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
AudioConnection          patchCord31(mix7, 0, mix9, 0);
AudioConnection          patchCord32(mix8, 0, mix9, 1);
AudioConnection          patchCord33(mix9, 0, amp1, 0);
AudioConnection          patchCord34(amp1, 0, dac, 0);
AudioConnection          patchCord35(amp1, 0, dac, 1);
// GUItool: end automatically generated code

double attack, decay, sustain, release;
int type, velocity, channel, d1, d2;
int waveform;
unsigned int volume_val = 7;  //The control indicator for volume input on the keyboard
const double const1 = log10(4) - log10(3);            //Used for envelope equations. See README.md section 3.2
const double const2 = log10(2) / const1;              //continued
const double const3 = (log10(3) - log10(2)) / const1; //continued
const double const4 = 11.7 / (11.8 * const3);         //continued
const double analog_range = 1023.0; //Max value for analog inputs
const double envelope_max = 11800.0; //Max duration of envelope attack, decay, and release

//Identify Potentiometer Pins for later
unsigned int potentiometers[]
{
  41, 38, 39, 40
};

//Used for buttons - not in final MIDI code
unsigned int note_pins[]
{
  27, 28, 29, 30
};

//Debouncing buttons
Bounce b1(27, 10);
Bounce b2(28, 10);
Bounce b3(29, 10);
Bounce b4(30, 10);
Bounce buttons[4]
{
  b1, b2, b3, b4
};

//Setup for actual note frequencies since MIDI only gives 0-255 int
//but we need frequency as a double
double notes[]
{
  16.35, 17.23, 18.35, 19.45, 20.60, 21.83, 23.21, 24.50, 25.96, 27.50, 29.14, 30.87,
  32.7, 34.65, 36.71, 38.89, 41.2, 43.65, 46.25, 49, 51.91, 55, 58.27, 61.74,
  65.41, 69.3, 73.42, 77.78, 82.41, 87.31, 92.5, 98, 103.83, 110, 116.54, 123.47,
  130.81, 138.59, 146.83, 155.56, 164.81, 174.61, 185, 196, 207.65, 220, 233.08, 246.94,
  261.63, 277.18, 293.66, 311.13, 329.63, 349.23, 369.99, 392, 415.3, 440, 466.16, 493.88,
  523.25, 554.37, 587.33, 622.25, 659.25, 698.46, 739.99, 783.99, 830.61, 880, 932.33, 987.77,
  1046.5, 1108.73, 1174.66, 1244.51, 1318.51, 1396.91, 1479.98, 1567.98, 1661.22, 1760, 1864.66, 1975.53,
  2093.00, 2217.46, 2349.32, 2489.02, 2637.02, 2793.83, 2959.96, 3135.96, 3322.44, 3520.00, 3729.31, 3951.07,
  4186.01, 4434.92, 4698.64, 4978.03, 5274.04, 5587.65, 5919.91, 6271.93, 5544.88, 7040.00, 7458.62, 7902.13,
  8372.02, 8869.84, 9397.27, 9956.06, 10548.08, 11175.30, 11839.82, 12543.86, 13289.75, 14080.00, 14917.24, 15804.26
};

//Makes setting up mixers easier
AudioMixer4 * mixers[8]
{
  &mix1, &mix2, &mix3, &mix4, &mix5, &mix6, &mix7, &mix8
};

//Setting up for polyphony by creating an array of pointers
//to all the envelopes which we can then cycle through
AudioEffectEnvelope * envelopes[6]
{
  &envelope1, &envelope2, &envelope3, &envelope4, &envelope5, &envelope6
};

//Determine which envelope plays which note
int playing[255];

//Makes sorting out note frequency easier,
//as well as polyphony
AudioSynthWaveform * oscillators[18]
{
  &oscillator1, &oscillator2, &oscillator3,
  &oscillator4, &oscillator5, &oscillator6,
  &oscillator7, &oscillator8, &oscillator9,
  &oscillator10, &oscillator11, &oscillator12,
  &oscillator13, &oscillator14, &oscillator15,
  &oscillator16, &oscillator17, &oscillator18
};

//For determining which envelope is available for polyphony
bool available [6]
{
  true, true, true, true, true, true
};
//For which note each envelope is playing
unsigned int notes_played[6];

//Setup for the USB host
USBHost usb;
//No idea why these hubs are needed but removing them breaks it
USBHub hub1(usb);
USBHub hub2(usb);
MIDIDevice midi1(usb);

//Playing notes with polyphony
void play_note(const unsigned int note);
void stop_note(const unsigned int note);

//MIDI Input Setup
void myNoteOn(byte channel, byte note, byte velocity);
void myNoteOff(byte channel, byte note, byte velocity);
void volume_control(byte channel, byte control, byte value);

void setup()
{
    
  // Wait 1.5 seconds before turning on USB Host.  If connected USB devices
  // use too much power, Teensy at least completes USB enumeration, which
  // makes isolating the power issue easier.
  //delay(1500);
  usb.begin();
  
  AudioMemory(18); //Need to designate audio memory or nothing works
  Serial.begin(9600); //For testing
  //Serial.println("Began monitoring USB host port");
  amp1.gain(1.0);

  //MIDI setup
  midi1.setHandleNoteOn(play_note);
  midi1.setHandleNoteOff(stop_note);
  midi1.setHandleControlChange(volume_control);

  //Prevent clipping with mixers
  for (unsigned int i = 0; i < 8; ++i)
  {
    mixers[i]->gain(0, 0.16);
    mixers[i]->gain(1, 0.16);
    mixers[i]->gain(2, 0.16);
    mixers[i]->gain(3, 0);
  }
  mix9.gain(0, 0.25);
  mix9.gain(1, 0.25);
  mix9.gain(2, 0);
  mix9.gain(3, 0);

  //Initialize oscillators
  for (unsigned int i = 0; i < 18; ++i)
  {
    oscillators[i]->begin(WAVEFORM_SINE);
  }

  //Initialize buttons for testing
  for (unsigned int i = 0; i < sizeof(note_pins) / sizeof(int); ++i)
  {
    pinMode(note_pins[i], INPUT);
  }
}

void loop()
{
  usb.Task();
  midi1.read();

  for (unsigned int i = 0; i < sizeof(note_pins) / sizeof(int); ++i)
  {
    if (buttons[i].update())
    {
      if (buttons[i].risingEdge())
      { 
        switch (note_pins[i])
        {
          case 27:
            waveform = WAVEFORM_SINE;
            break;
          case 28:
            waveform = WAVEFORM_SQUARE;
            break;
          case 29:
            waveform = WAVEFORM_TRIANGLE;
            break;
          case 30:
            waveform = WAVEFORM_SAWTOOTH;
            break;
        }
      }
    }
  }
}

//Implement polyphony via parallel arrays
void play_note(byte channel, byte note, byte velocity)
{
  //Serial.println(notes[note]); //For testing
  //Serial.println(velocity, DEC); //For testing
  //Update envelope control. See README.md section 3.2
  decay = analogRead(potentiometers[0]);
  //Serial.println(decay);
  if (decay > (1023.0 / 2.0))
  {
    decay = (const2 - (log10(1 + decay / analog_range) / const1)) * 100.0;
  } else
  {
    decay = (1 - (log10(1 + decay / analog_range) / const1) * const4) * envelope_max;
  }
  sustain = analogRead(potentiometers[1]) / analog_range;
  //Serial.println(sustain);
  attack = analogRead(potentiometers[2]);
//  Serial.println("Read attack raw value: ");
//  Serial.println(attack);
  if (attack > (1023.0 / 2.0))
  {
    attack = (const2 - (log10(1 + attack / analog_range) / const1)) * 100.0;
  } else
  {
    attack = (1 - (log10(1 + attack / analog_range) / const1) * const4) * envelope_max;
  }
  release = analogRead(potentiometers[3]);
//  Serial.println("Read release raw value: ");
//  Serial.println(release);
  if (release > (1023.0 / 2.0))
  {
    release = (const2 - (log10(1 + release / analog_range) / const1)) * 100.0;
  } else
  {
    release = (1 - (log10(1 + release / analog_range) / const1) * const4) * envelope_max;
  }
//  Serial.println("Calculated Attack (ms): ");
//  Serial.println(attack);
//  Serial.println("Calculated Release (ms): ");
//  Serial.println(release);

  unsigned int i = 0;
  while (i < 6)
  {
    if (!envelopes[i]->isActive())
    {
      available[i] = true;
      notes_played[i] = 0;
    }
    if (available[i] || notes_played[i] == note)
    {
      double v = velocity / 127.0; //MIDI velocity is a signed int for some reason
      //Serial.println(v);
      for (unsigned int j = 3 * i; j < 3 * i + 3; ++j)
      {
        oscillators[j]->begin(waveform);
        oscillators[j]->frequency(notes[note]);
        oscillators[j]->amplitude(v);
      }
      envelopes[i]->attack(attack);
      envelopes[i]->decay(decay);
      envelopes[i]->sustain(sustain);
      envelopes[i]->release(release);
      envelopes[i]->noteOn();
      //Update Polyphony
      notes_played[i] = note;
      available[i] = false;
      playing[note] = i;
      break;
    }
    ++i;
  }
}

//Stop playing a note. Corresponds to play_note() above
void stop_note(byte channel, byte note, byte velocity)
{
  //Serial.println(notes[note]); //For testing
  unsigned int envelope = playing[note];
  envelopes[envelope]->noteOff();
}

//Set volume from keyboard
void volume_control(byte channel, byte control, byte value)
{
//  Serial.print("Control Change, ch=");
//  Serial.print(channel, DEC);
//  Serial.print(", control=");
//  Serial.print(control, DEC);
//  Serial.print(", value=");
//  Serial.println(value, DEC);
  if (control == volume_val) amp1.gain(value / 127.0);
}
