// Binary sum game. Choose from 5 random numbers and 3 fixed numbers (1, 3, 7)
// to sum to 255. 8 switches control which numbers are summed together.
// Reset switch is required. It is used to generate a new set of random
// numbers for the next round. Note that not all games are winnable -
// part of the challenge.

#include <EEPROM.h>
#include <avr/sleep.h>

int seed;
// LED 1 is D16
// LED 2 is D15
// LED 3 is D14
// LED 4 is D8
// LED 5 is D9
// LED 6 is D10
// LED 7 is D11
// LED 8 is D13

// D0-D7 are switches. Pullups are enabled, so no
// external pullups required.

// Maximum possible value of random number associated
// with a particular switch.
#define RANDOM_MAX 100

// Values associated with each switch.
byte value0;
byte value1;
byte value2;
byte value3;
byte value4;
byte value5;
byte value6;
byte value7;

void setup() {
  for(byte i = 8; i < 17; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i-8, HIGH);
  }

  // Fake source of entropy. Makes sure a different
  // set of numbers is generated each bootup.
  EEPROM.get(0, seed);
  randomSeed(seed);
  EEPROM.write(0, seed + 1);

  if (seed & 1) {
    writeZZZ(0b00010000);
    writeZZZ(0b00111000);
    writeZZZ(0b01101100);
    writeZZZ(0b11000011);
    writeZZZ(0b10000001);
    writeZZZ(0b11000011);
    writeZZZ(0b01101100);
    writeZZZ(0b00111000);
    writeZZZ(0b00010000);
  } else {
    for(byte i=0; i< 8; i++) {
      writeZZZ(0b11001100);
      writeZZZ(0b00110011);
    }
  }
  
  // Generate numbers to assign to each switch.
  value0 = random(RANDOM_MAX) + 1;
  value1 = random(RANDOM_MAX) + 1;
  value2 = random(RANDOM_MAX) + 1;
  value3 = random(RANDOM_MAX) + 1;
  value4 = 1;
  value5 = 3;
  value6 = 7;
  value7 = random(RANDOM_MAX) + 1;
}

// Last value of sum. Used to detect user actions for
// figuring out when to time out and go to sleep.
byte lastSum;

// Time lastSum changed.
byte timeSumChanged;

// Time right now.
byte now;

void loop() {
  byte i;

  // Read switches, sum all numbers associated
  // with switches that are on.
  byte sum =
    ((digitalRead(0) == HIGH) ?  value0 : 0) +
    ((digitalRead(1) == HIGH) ?  value1 : 0) +
    ((digitalRead(2) == HIGH) ?  value2 : 0) +
    ((digitalRead(3) == HIGH) ?  value3 : 0) +
    ((digitalRead(4) == HIGH) ?  value4 : 0) +
    ((digitalRead(5) == HIGH) ?  value5 : 0) +
    ((digitalRead(6) == HIGH) ?  value6 : 0) +
    ((digitalRead(7) == HIGH) ?  value7 : 0);

  now = millis() >> 10;

  // Figure out if sum changed since last loop.
  if (sum != lastSum) {
    timeSumChanged = now;
    lastSum = sum;
  }

  if (now - timeSumChanged > 60) {
    // Been a while since the sum changed, we should go to sleep.
    // Animation.
    writeZZZ(0b11111111);
    writeZZZ(0b01111110);
    writeZZZ(0b00111100);
    writeZZZ(0b00011000);
    writeZZZ(0b00011000);
    writeZZZ(0b00010000);
    writeZZZ(0b00010000);
    writeZZZ(0b00010000);
    writeZZZ(0b00010000);
    writeZZZ(0b00010000);
    // Turn off LEDs and pullups (save power).
    PORTA = 0;
    PORTB = 0;
    PORTD = 0;
    // All pins as inputs (save more power).
    DDRB = 0;

    // Power down. User must hit reset button to bring
    // us back.
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    cli();
    sleep_mode();
  }

  // Did the user win?
  if (sum == 255) {
    // Celebrate!
    writeZZZ(1 << 1);
    writeZZZ(1 << 2);
    writeZZZ(1 << 3);
    writeZZZ(1 << 4);
    writeZZZ(1 << 5);
    writeZZZ(1 << 6);
    writeZZZ(1 << 5);
    writeZZZ(1 << 4);
    writeZZZ(1 << 3);
    writeZZZ(1 << 2);
  } else {
    // Nope, just write the sum to the LEDs.
    writeZZZ(sum);
  }
}

// Write the given byte to the LEDs, then wait for 100ms.
void writeZZZ(byte i) {
  byte bit0;
  byte bit1;
  byte bit2;
  byte bit3;
  byte bit4;
  byte bit5;
  byte bit6;
  byte bit7;
  bit7 = i & 1;
  bit6 = i & 0b10;
  bit5 = i & 0b100;
  bit4 = i & 0b1000;
  bit3 = i & 0b10000;
  bit2 = i & 0b100000;
  bit1 = i & 0b1000000;
  bit0 = i & 0b10000000;
  digitalWrite(16, bit0);
  digitalWrite(15, bit1);
  digitalWrite(14, bit2);
  digitalWrite(8, bit3);
  digitalWrite(9, bit4);
  digitalWrite(10, bit5);
  digitalWrite(11, bit6);
  digitalWrite(13, bit7);
  delayMicroseconds(100*1000);
}

