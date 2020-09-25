/*
 *  BLINK NB-USITWISL SLAVE for ATtiny microcontrollers
 *  Author: Gustavo Casanova / Nicebots
 *  ...................................................
 *  File: blink-sl-twisl.cpp (Blink application)
 *  ................................................... 
 *  Version: 1.0 / 2020-09-24
 *  gustavo.casanova@nicebots.com
 *  ...................................................
 */

/* This a led blink test program compatible with
   the NB command-set through TWI (I2C).

   Available NB TWI commands:
   ------------------------------
   a - (SETIO1_1) Start blinking
   s - (SETIO1_0) Stop blinking
   x - (RESETMCU) Reset device
*/

// Includes
#include "blink-sl-twisl.h"

// Global variables
uint8_t command[32] = {0}; /* I2C Command received from master  */
uint8_t commandLength = 0; /* I2C Command number of bytes  */

bool reset_now = false;
bool slow_ops_enabled = false;
volatile bool blink = true;
volatile uint32_t toggle_delay = LONG_DELAY;

// Prototypes
void DisableWatchDog(void);
void ReceiveEvent(uint8_t);
void EnableSlowOps(void);
void ResetMCU(void);

// Main function
int main(void) {
    /* ___________________
      |                   | 
      |    Setup Block    |
      |___________________|
    */
    DisableWatchDog(); // Disable watchdog to avoid continuous loop after reset
    SetCPUSpeed1MHz(); // Set prescaler = 1 (System clock / 1)
    // Set output pins
    LED_DDR |= (1 << LED_PIN);   // Set led control pin Data Direction Register for output
    LED_PORT &= ~(1 << LED_PIN); // Turn led off
    _delay_ms(250);              // Delay to allow programming at 1 MHz after power on
    SetCPUSpeed8MHz();           // Set the CPU prescaler for 8 MHz

    UsiTwiDriverInit(TWI_ADDR);
    p_receive_event = ReceiveEvent;  // Pointer to TWI receive event function

    sei(); // Enable Interrupts

    /* ___________________
      |                   | 
      |     Main Loop     |
      |___________________|
    */
    for (;;) {
        if (reset_now == true) {
            ResetMCU();
        }

        if (toggle_delay-- == 0) {
            if (blink == true) {
                LED_PORT ^= (1 << LED_PIN); // Toggle PB1
            } else {
                LED_PORT &= ~(1 << LED_PIN); // Turn PB1 off
            }
            toggle_delay = LONG_DELAY;
        }
    }
    return 0;
}

/* ________________________
  |                        |
  | TWI data request event |
  |________________________|
*/
//void RequestEvent(uint8_t received_bytes) {
void ReceiveEvent(uint8_t received_bytes) {

    for (uint8_t i = 0; i < received_bytes; i++) {
        //command[i] = Wire.read();
        command[i] = UsiTwiReceiveByte();
    }

    uint8_t opCodeAck = ~command[0]; // Command Operation Code acknowledge => Command Bitwise "Not"
    switch (command[0]) {
        // ******************
        // * SETIO1_1 Reply *
        // ******************
        case SETIO1_1: {
            blink = true;
            // Wire.write(opCodeAck);
            UsiTwiTransmitByte(opCodeAck);
            break;
        }
        // ******************
        // * SETIO1_0 Reply *
        // ******************
        case SETIO1_0: {
            blink = false;
            // Wire.write(opCodeAck);
            UsiTwiTransmitByte(opCodeAck);
            break;
        }
        // ******************
        // * INFORMAT Reply *
        // ******************        
        case INFORMAT: {
            char info[6] = {'H', 'e', 'l', 'l', 'o', '!'};
            // Wire.write(opCodeAck);
            for (int i = 0; i < sizeof(info); i++) {
                // Wire.write(info[i]);
                UsiTwiTransmitByte(info[i]);
            }
            break;
        }
        // ******************
        // * RESETMCU Reply *
        // ******************
        case RESETMCU: {
            // Wire.write(opCodeAck);
            UsiTwiTransmitByte(opCodeAck);
            reset_now = true;
            break;
        }
        // *************************
        // * Unknown Command Reply *
        // *************************
        default: {
            // Wire.write(UNKNOWNC);
            UsiTwiTransmitByte(UNKNOWNC);
            break;
        }
    }
}

/* __________________________
  |                          |
  | Function DisableWatchDog |
  |__________________________|
*/
void DisableWatchDog(void) {
    wdt_reset();
    MCUSR = 0;

#ifdef ARDUINO_AVR_PRO
    WDTCSR = ((1 << WDCE) | (1 << WDE));
    WDTCSR = 0;
#endif  // ARDUINO_AVR_PRO
#ifdef ARDUINO_AVR_ATTINYX5
    WDTCR = ((1 << WDCE) | (1 << WDE));
    WDTCR = ((1 << WDP2) | (1 << WDP1) | (1 << WDP0));
#endif
// ARDUINO_AVR_ATTINYX5
#if defined(__AVR_ATtiny25__) | \
    defined(__AVR_ATtiny45__) | \
    defined(__AVR_ATtiny85__)
    WDTCR |= ((1 << WDCE) | (1 << WDE));
    WDTCR = ((1 << WDP2) | (1 << WDP1) | (1 << WDP0));  // 2 seconds timeout
#endif
// ARDUINO_AVR_ATTINYX4
#if defined(__AVR_ATtiny24__) | \
    defined(__AVR_ATtiny44__) | \
    defined(__AVR_ATtiny84__)
    WDTCSR |= (1 << WDCE) | (1 << WDE);
    WDTCSR = ((1 << WDP2) | (1 << WDP1) | (1 << WDP0));  // 2 seconds timeout
#endif
}

/* ________________
  |                |
  | Function Reset |
  |________________|
*/
void ResetMCU(void) {
    LED_PORT &= ~(1 << LED_PIN); /* Turn led off */
    wdt_enable(WDTO_15MS);
    for (;;) {
    }
}

/* __________________________
  |                          |
  | Function SetCPUSpeed1MHz |
  |__________________________|
*/
void SetCPUSpeed1MHz(void) {
    cli();                                   /* Disable interrupts */
    CLKPR = (1 << CLKPCE);                   /* Mandatory for setting prescaler */
    CLKPR = ((1 << CLKPS1) | (1 << CLKPS0)); /* Set prescaler 8 (System clock / 8) */
    sei();                                   /* Enable interrupts */
}

/* __________________________
  |                          |
  | Function SetCPUSpeed8MHz |
  |__________________________|
*/
void SetCPUSpeed8MHz(void) {
    cli();                 /* Disable interrupts */
    CLKPR = (1 << CLKPCE); /* Mandatory for setting CPU prescaler */
    CLKPR = (0x00);        /* Set CPU prescaler 1 (System clock / 1) */
    sei();                 /* Enable interrupts */
}

// /* ______________
//   |              |
//   | Clear Screen |
//   |______________|
// */
// void ClrScr(void) {
//     Serial.write(27);     // ESC command
//     Serial.print("[2J");  // clear screen command
//     Serial.write(27);     // ESC command
//     Serial.print("[H");   // cursor to home command
// }
