#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

void setup()
{
  AudioSynthWaveform       waveform1;
  AudioSynthWaveform       waveform2;
  AudioSynthWaveform       waveform3;
  AudioMixer4              mixer;
  AudioEffectEnvelope      envelope;
  AudioEffectChorus        chorus;
  AudioAmplifier           amp;
  AudioOutputAnalog        dac;
  AudioConnection          patchCord1(waveform1, 0, mixer1, 0);
  AudioConnection          patchCord2(waveform2, 0, mixer1, 1);
  AudioConnection          patchCord3(waveform3, 0, mixer1, 2);
  AudioConnection          patchCord4(mixer1, envelope1);
  AudioConnection          patchCord5(envelope1, chorus1);
  AudioConnection          patchCord6(chorus1, amp1);
  AudioConnection          patchCord7(amp1, dac1);

}

void loop() 
{
  float dummy; //stand in for user input
  //Get input into dummy
  amp.gain(dummy);
}
