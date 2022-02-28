#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <ADC.h>

// GUItool: begin automatically generated code
AudioSynthWaveform       oscillator1;      
AudioOutputI2S           dac;           
AudioConnection          patchCord1(oscillator1, 0, i2s1, 0);
AudioConnection          patchCord2(oscillator1, 0, i2s1, 1);
// GUItool: end automatically generated code

int main(void)
{
    double volume = 0.5;
    
    oscillator1.begin(WAVEFORM_SINE);
    while(1)
    {
        volume = analogRead(40) / 1024.0;
        oscillator1.amplitude(volume);
    }
    return 0;
}