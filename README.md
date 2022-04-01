# Senior Capstone
 Senior Capstone Code for SASS

-----------------
Table of Contents
-----------------
 Sections:
 1.0 Overview
  1.1 Basic Functionality
  1.2 Breakdown of Code Sections
  1.3 Additional Notes
 2.0 Setup on a Teensy 4.1
  2.1 Required Software
  2.2 Instructions
 3.0 Explanation of Constants
  3.1 Hardware Constants
  3.2 Software Constants

----------------------
1.0 Overview
----------------------

--------------------------
2.0 Setup on a Teensy 4.1
--------------------------


----------------------------
3.0 Explanation of Constants
-----------------------------
 
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
d = (11.7/11.8)*log(3/2)/log(4/3) = (11.7/11.8)*(log(3)-log(2))/(log(4)-log(3))

To simplify all this in the final code, the following constants are used:

const1 = log(4) - log(3) = b
const2 = log(2)/const1 = a
const3 = (log(3)-log(2))/const1
const4 = (11.7/11.8)*const3 = d
