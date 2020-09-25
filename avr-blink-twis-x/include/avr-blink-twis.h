/*
 *  AVR BLINK TWIS
 *  Author: Gustavo Casanova / Nicebots
 *  ...........................................
 *  File: header.h (Application headers)
 *  ........................................... 
 *  Version: 1.2 / 2020-09-24
 *  gustavo.casanova@gmail.com
 *  ...........................................
 */

#ifndef AVR_BLINK_TWIS_H
#define AVR_BLINK_TWIS_H

#ifndef __AVR_ATtiny85__
#define __AVR_ATtiny85__
#endif

// Includes
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <nb-twi-cmd.h>
#include <nb-usitwisl.h>
#include <stdbool.h>
#include <util/delay.h>

#define TWI_ADDR 46
#define LONG_DELAY 0x7FFF

#define LED_PIN PB1  // Digispark ATtinyX5, ATtinyX4
#define LED_DDR DDRB
#define LED_PORT PORTB

void RequestEvent(void);
void ReceiveEvent(uint8_t received_bytes);
void EnableSlowOps(void);
void SetCPUSpeed1MHz(void);
void SetCPUSpeed8MHz(void);
void ClrScr(void);

#endif  // AVR_BLINK_TWIS_H
