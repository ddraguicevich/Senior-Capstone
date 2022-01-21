#ifndef OSCILLATOR_H
#define OSCILLATOR_H

#include <ctime>

#define WAVE_SIN 's'
#define WAVE_SQ 'q'
#define WAVE_TRI 't'
#define WAVE_SAW 'w'

class Oscillator
{
    private:
        float theta;
        float volume;
        float amplitude;
        float freq;
        char wave;
        auto last_time;
    public:
        int set_wave(char wave);
        int set_freq(float freq);
        int set_volume(float volume);
        void update(void);
}

#endif