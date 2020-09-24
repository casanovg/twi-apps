/*
 *  BLINK NB-USITWISL SLAVE for ATmega microcontrollers
 *  Author: Gustavo Casanova / Nicebots
 *  ...................................................
 *  File: blink-sl-twisl.h (Application headers)
 *  ...................................................
 *  Version: 1.0 / 2020-09-24
 *  gustavo.casanova@nicebots.com
 *  ...................................................
 */

#ifndef BLINK_SL_TWISL_H
#define BLINK_SL_TWISL_H

// #ifndef __AVR_ATtiny85__
// #define __AVR_ATtiny85__
// #endif

// Includes
//#include <Arduino.h>
//#include <Wire.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <nb-twi-cmd.h>
#include <nb-usitwisl.h>
#include <stdbool.h>
#include <util/delay.h>

#define TWI_ADDR 25
#define LONG_DELAY 0x3FFFF

#define LED_PIN PB1 // Digispark ATtinyX5, ATtinyX4
#define LED_DDR DDRB
#define LED_PORT PORTB

void RequestEvent(void);
void ReceiveEvent(uint8_t received_bytes);
void ClrScr(void);

#endif  // BLINK_SL_TWISL_H