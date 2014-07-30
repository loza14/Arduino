
#ifndef PWMFrequency
#define PWMFrequency

#if ARDUINO >= 100
#include <Arduino.h> // Arduino 1.0
#else
#include <WProgram.h> // Arduino 0022
#endif

#if defined __AVR_ATmega32U4__

/**
 * Divides a given PWM pin frequency by a divisor.
 *
 * Arduino Leonardo AtMega 32u4 specific
 *
 * Sets the Prescaler (Divisor) for a given PWM pin. The resulting frequency 
 * is equal to the base frequency divided by the given divisor:
 *   - Base frequencies:
 *      o The base frequency for pins 3, and 11 is 64,500 Hz.
 *      o The base frequency for pins 5, 9, and 10 is 31,250 Hz.
 *      o The base frequency for pins 6 and 13 is 125,000 Hz.
 *   - Divisors:
 *      o The divisors available on pins 3, 5, 9, 10 and 11 are: 1, 8, 64, 256, and 1024.
 *      o The divisors available on pins 6 and 13 are all powers of two between 1 and 16384
 * 
 * PWM frequencies are tied together in pairs of pins. If one in a
 * pair is changed, the other is also changed to match:
 *   - Pins 3 and 11 are paired on timer0
 *   - Pins 9 and 10 are paired on timer1
 *   - Pins 6 and 13 are paired on timer4 
 *   - Pins 5 is exclusivly on timer3
 * 
 * Note: Pins 3 and 11 operate on Timer 0 changes this pins will 
 * affect the user of the main time millis() functions
 */ 
void setPWMPrescaler(uint8_t pin, uint8_t prescale) 
{ 
  if(pin == 3 || pin == 5 || pin == 9 || pin == 10 || pin == 11) { 
    byte mode;
    switch(prescale) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break; 
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if(pin == 3 || pin == 11) {
      TCCR0B = TCCR1B & 0b11111000 | mode;
    } else if (pin == 9 || pin == 10) {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    } else {
      TCCR3B = TCCR3B & 0b11111000 | mode;
    }
  } else if(pin == 3 || pin == 11) {
    byte mode; 
    switch(prescale) {
      case 1: mode = 0x01; break;
      case 2: mode = 0x02; break;
      case 4: mode = 0x03; break;
      case 8: mode = 0x04; break;
      case 16: mode = 0x05; break;
      case 32: mode = 0x06; break;
      case 64: mode = 0x07; break;
      case 128: mode = 0x08; break;
      case 256: mode = 0x09; break;
      case 512: mode = 0x0A; break;
      case 1024: mode = 0x0B; break;
      case 2048: mode = 0x0C; break;
      case 4096: mode = 0x0D; break;
      case 8192: mode = 0x0E; break;
      case 16384: mode = 0x0F; break;
      default: return;
    }
    TCCR4B = TCCR4B & 0b11110000 | mode;
  }
}

#else

/**
 * Divides a given PWM pin frequency by a divisor.
 * 
 * Sets the Prescaler (Divisor) for a given PWM pin. The resulting frequency 
 * is equal to the base frequency divided by the given divisor:
 *   - Base frequencies:
 *      o The base frequency for pins 3, 9, 10, and 11 is 31250 Hz.
 *      o The base frequency for pins 5 and 6 is 62500 Hz.
 *   - Divisors:
 *      o The divisors available on pins 5, 6, 9 and 10 are: 1, 8, 64, 256, and 1024.
 *      o The divisors available on pins 3 and 11 are: 1, 8, 32, 64, 128, 256, and 1024.
 * 
 * PWM frequencies are tied together in pairs of pins. If one in a
 * pair is changed, the other is also changed to match:
 *   - Pins 5 and 6 are paired on timer0
 *   - Pins 9 and 10 are paired on timer1
 *   - Pins 3 and 11 are paired on timer2
 * 
 * Note that this function will have side effects on anything else
 * that uses timers:
 *   - Changes on pins 3, 5, 6, or 11 may cause the delay() and
 *     millis() functions to stop working. Other timing-related
 *     functions may also be affected.
 *   - Changes on pins 9 or 10 will cause the Servo library to function
 *     incorrectly.
 * 
 * Thanks to macegr of the Arduino forums for his documentation of the
 * PWM frequency divisors. His post can be viewed at:
 *
 *   http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1235060559/0#4
 */
void setPWMPrescaler(uint8_t pin, uint8_t prescale) {
  byte mode;
  if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch(prescale) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if(pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } else if(pin == 3 || pin == 11) {
    switch(prescale) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x7; break;
      default: return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}

#endif

#endif