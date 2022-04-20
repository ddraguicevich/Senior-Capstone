# Senior Capstone
 Senior Capstone Code for SASS

-----------------
Table of Contents
-----------------
 Sections:
 
 1.0 Overview
 
  1.1 Basic Functionality
  
 2.0 Setup on a Teensy 4.1
 
  2.1 Required Software
  
  2.2 Instructions
  
 3.0 Explanation of Constants
 
  3.1 Hardware Constants
  
  3.2 Software Constants

----------------------
1.0 Overview
----------------------

1.1 Basic Functionality

The code is broken down into the following targets:

Basic Testing: Play a sound with user-controlled frequency (via potentiometer in this case)

Single Note MIDI: Get input from a MIDI keyboard and play a single note with some user-defined envelope

Minimum Viable Product: MIDI input with 6-note polyphony and user-defined volume, 3x waveforms, and 6 note polyphony

--------------------------
2.0 Setup on a Teensy 4.1
--------------------------

2.1 Required Software

The Teensy 4.1 use a custom uploader running on top of Arduino's IDE.
The Arduino IDE can currently be found at https://www.arduino.cc/en/software
Teensy's Bootloader can currently be found at https://www.pjrc.com/teensy/loader.html

2.2 Instructions

- Install the Arduino IDE found above
- Install Teensy's Bootlarder found above
- Connect the Teensy to your computer with a micro USB cable
- In the top-left, select Tools->Boards->Teensy 4.1 to select the correct bootloader
- Select a test program (such as LED blink) and press "Upload" (Arrow, top-right)
- Once the program compiles, a small window should pop up showing the Teensy is being programmed
- If the window does not show up, reinstall Teensy's Bootloader
- If the window does show up but does not go through a process ending with "Reboot OK" then press the button once on the Teensy 4.1 to manually flash (next to the Micro SD slot)
- It should work

----------------------------
3.0 Explanation of Constants
-----------------------------
 
3.1 Hardware Constants

The Teensy 4.1 has an ADC which has 7 bits of effective input, giving a range of values 0-1023. As such, the max value for any given potentiometer is 1023, which shows up in several calculations for the slider potentiometers.
 
3.2 Software Constants
 
The range for envelope control, specifically attack, decay, and release, is very large (0-11.8 seconds!). The problem is that the first few miliseconds  are much more important for precision than later time differences. The method I used to make this usable is to split the range in half, and have the first half of the potentiometer's range be 0-100ms, and the rest be 100-11800ms. Further, this is not done with a linear curve , but rather an exponential curve. Since calculating 10^1023 would be simply not feasible, instead the code uses 1-log10(1023), which is much easier but requires the range to be backwards: Max resistance on the potentiometer corresponds to 0, and min resistance corresponds to max duration. The basic idea can be seen in the following equation:

duration(ms) = (1 - log(1+x)/log(2))*11800

Where x is potentiometer value with range [0, 1]. However, this curves the whole 11.8 second range, so it isn't super usefule. To get the exponential curve to work properly with two (2) equations, some fancy maths were required. The main equations are the following nastiness:

Let x be normalised resistance for the potentiometer with range [0, 1]

For first 100ms: duration(ms) = [((log(2)/(log(4)-log(3)) - ((log(1+x)/(log(4)-log(3)))] * 100

For remaining duration: duration(ms) = (1 - ((log(1+x)/(log(4)-log(3))*11.7/(11.8*((log(3)-log(2))/(log(4)-log(3))))) * 11800

As we can see, this is a truly ugly mess. The logic behind it is actually simple enough, however. For the first 100ms we need

a - log(2)/log(b) = 0

a - log(1.5)/log(b) = 1

From which we get the constants

a = log(2)/(log(4/3)) = log(2)/(log(4)-log(3))

b = log(4/3) = log(4)-log(3)

The same is true for the second equation:

c - (log(1)/b)*d = 1
c - (log(1.5)/b)*d = 1/118

So we are left with

c = 1

To simplify all this in the final code, the following constants are used:

const1 = log(4) - log(3) = b

const2 = log(2)/const1 = a

const3 = (log(3)-log(2))/const1

const4 = 1/(11.8*const3) = d

And we multiply const4 and the output of equation one by (11.8 - x) to get x seconds as a divider between the two halves of the envelope controller.

For the values used by the mixers, the inputs on a mixer cannot total more than 0.5. I don't know why this is the case, it should be 1.0 since it's attenuating but it's being doubled somewhere so two inputs would be 0.25 each, three would be 0.166 each, and four would be 0.125 each. I set the dummy inputs to 0 to be safe even though they aren't connected to anything.
