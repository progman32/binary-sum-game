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

#define RANDOM_MAX 100
byte value0;
byte value1;
byte value2;
byte value3;
byte value4;
byte value5;
byte value6;
byte value7;

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
void setup() {
  for(byte i = 8; i < 17; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i-8, HIGH);
  }

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
  
  value0 = random(RANDOM_MAX) + 1;
  value1 = random(RANDOM_MAX) + 1;
  value2 = random(RANDOM_MAX) + 1;
  value3 = random(RANDOM_MAX) + 1;
  value4 = 1;
  value5 = 3;
  value6 = 7;
  value7 = random(RANDOM_MAX) + 1;
}

byte lastSum;
byte timeSumChanged;
byte now;

void loop() {
  byte i;
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

  if (sum != lastSum) {
    timeSumChanged = now;
    lastSum = sum;
  }

  if (now - timeSumChanged > 60) {
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
    PORTA = 0;
    PORTB = 0;
    PORTD = 0;
    DDRB = 0;
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    cli();
    sleep_mode();
  }

  if (sum == 255) {
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
    writeZZZ(sum);
  }
}




