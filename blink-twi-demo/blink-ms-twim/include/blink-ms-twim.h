/*
 *  Blink TWI Master for Arduino (PlatformIO)
 *  Author: Gustavo Casanova
 *  ...........................................
 *  File: blink-ms-twim.h (Header)
 *  ........................................... 
 *  Version: 1.0 / 2020-05-29
 *  gustavo.casanova@gmail.com
 *  ...........................................
 *  This is an I2C master-side serial commander to control a
 *  general-purpose application running on a slave microcontroller.
 *  It runs on Arduino compatible microcontroller and uses the
 *  NB libraries as an upper communication layer on top of the
 *  Arduino Wire library.
 */

#ifndef BLINK_MS_TWIM_H
#define BLINK_MS_TWIM_H

#include <Arduino.h>
#include <NbMicro.h>
#include <TwiBus.h>
#include <nb-twi-cmd.h>
#include <stdio.h>

// Serial display settings
#define USE_SERIAL Serial
#define SERIAL_BPS 115200

// I2C pins
#define SDA 2  // I2C SDA pin - ESP8266 2 - ESP32 21
#define SCL 0  // I2C SCL pin - ESP8266 0 - ESP32 22

// #define DEBUG_LEVEL 1

typedef byte uint8_t;

// Prototypes
void ReadChar(void);
//uint16_t ReadWord(void);
void ClrScr(void);
void ShowHeader(void);
void ShowMenu(void);
void ListTwiDevices(uint8_t sda, uint8_t scl);
uint8_t FindSlave(void);
//void resetFunc(void);
void (*resetFunc)(void) = 0;

#endif  // BLINK_MS_TWIM_H
