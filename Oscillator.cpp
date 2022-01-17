#include <iostream>
#include <chrono>
#include <sys/time.h>
#include <ctime>
#include <cmath>
#include "Oscillator.hpp"

int Oscillator::set_wave(char wave)
{
    this->wave = wave;
    if (this->wave == wave)
    {
        return 0;
    } else
    {
        return 1;
    }
}

int Oscillator::set_freq(cloat freq)
{
    this->freq = freq;
    if (this->freq == freq)
    {
        return 0;
    } else
    {
        return 1;
    }
}

int Oscillator::set_volume(float volume)
{
    this->volume = volume;
    if (this->volume == volume)
    {
        return 0;
    } else
    {
        return 1;
    }
}

void Oscillator::update(void)
{
    auto now = std::chrono::system_clock::now();
    auto delta = now - this->last_time;
    int temp;
    this->theta = modf(delta / this->freq + this->theta, &temp);
    switch(this->wave)
    {
        case s:
            this->amplitude = sin(2 * M_PI * this->theta);
            break;
        case q:
            this->amplitude = this->theta > 0.5 ? 1.0 : -1.0;
            break;
        case t:
            this->amplitude = this->theta > 0.5 ? 4 * this->theta - 3 : -4 * this->theta + 1;
            break;
        case w:
            this->amplitude = 2 * this->theta - 1;
            break;
    }
}