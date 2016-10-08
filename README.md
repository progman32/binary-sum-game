# Binary sum game

Choose from 5 random numbers and 3 fixed numbers (1, 3, 7)
to sum to 255. 8 switches control which numbers are summed together.
Reset switch is required. It is used to generate a new set of random
numbers for the next round. Note that not all games are winnable -
part of the challenge.

* LED 1 is D16
* LED 2 is D15
* LED 3 is D14
* LED 4 is D8
* LED 5 is D9
* LED 6 is D10
* LED 7 is D11
* LED 8 is D13

D0-D7 are switches. Pullups are enabled, so no external pullups required.

Reset switch generates new set of numbers (and powers back on if the device goes to sleep).

Device runs on 3 AAAs and goes to sleep (~20uA) after about 60s.
