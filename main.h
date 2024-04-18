/**
 * @file main.h
 * @brief Header file for the safe system main functionality.
 * @date Apr 18, 2024
 * @author vgalovic
 */

#ifndef MAIN_H_
#define MAIN_H_

#include "./keypad/keypad.h"
#include "./pin/pin.h"
#include "./timer0/timer0.h"
#include "./usart/usart.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BS 9600 ///< Baud rate for USART communication

#define BLINK_MS 500 ///< Blink interval in milliseconds

/**
 * @brief Enumeration defining different states of the safe system.
 */
typedef enum State {
    CLOSED, ///< Closed state
    OPEN,   ///< Open state
    UNLOCKED, ///< Unlocked state
    NUM1_U, ///< Number 1 entered in unlocked state
    NUM2_U, ///< Number 2 entered in unlocked state
    LOCKED, ///< Locked state
    NUM1_L, ///< Number 1 entered in locked state
    NUM2_L  ///< Number 2 entered in locked state
} State;

State state = CLOSED; ///< Current state of the safe system
State next_state = CLOSED; ///< Next state of the safe system

/**
 * @brief Enumeration defining different states of the LED.
 */
typedef enum Led {
    LED_OFF,  ///< LED is off
    LED_ON,   ///< LED is on
    LED_BLINK ///< LED is blinking
} Led;

Led led = LED_OFF; ///< Current state of the LED

uint8_t rows[4] = {11, 10, 9, 8}; ///< Keypad rows
uint8_t cols[4] = {7, 6, 5, 4};   ///< Keypad columns

uint8_t blink_counter = 0; ///< Counter for LED blinking
bool const_blink = false;  ///< Flag for constant LED blinking

uint8_t button;       ///< Current button pressed
uint8_t button_en;    ///< Last button pressed

char password[3]; ///< Password for the safe system

int16_t main(); ///< Main function declaration

bool isNumber(); ///< Function to check if the current button pressed is a number

void curentState(); ///< Function to handle state transitions

void ledState(); ///< Function to control the LED state

#endif /* MAIN_H_ */
