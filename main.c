/**
 * @file main.c
 * @brief Main file for controlling a safe system.
 * @date Apr 18, 2024
 * @author vgalovic
 */

#include "./main.h"

/**
 * @brief Main function controlling the safe system.
 * @return Always returns 0.
 */
int16_t main() {

    // Initialize timer, pins, USART, and keypad
    timer0InteruptInit();
    pinInit(PORT_B, DIODE_PIN, OUTPUT);
    usartInit(BS);
    keypadInit(rows, cols);

    // Display initial message
    usartPutString("Sef je zatvoren!\r\n\r\n");

    // Main loop for safe system operation
    while(1) {
        // Scan keypad for button press
        button = keypadScan();

        // Check if a button is pressed and handle accordingly
        if(button != 0x00 && button_en != button) {
            button_en = button;
            dispayState();
            state = next_state;
            curentState();
        }

        // Update LED state
        ledState();
    }

    return 0;
}

/**
 * @brief Checks if the currently pressed button is a number.
 * @return true if the button is a number, false otherwise.
 */
bool isNumber() {
    if(button >= '0' && button <= '9')
        return true;
    return false;
}
/**
 * @brief Displays the current state of the safe system.
 */
void dispayState(){
	switch(next_state){
		// Switch case to display state-specific messages
		case CLOSED: usartPutString("Sef je zatvoren!\r\n\r\n"); break;
		case OPEN: usartPutString("Sef je Otovren!\r\n\r\n"); break;
		case UNLOCKED:
			if(password[0] == '\0')
				usartPutString("Sef je otkljucan!\r\n\r\n");
			else if(isNumber())
				 usartPutString("*");
			break;
		case NUM1_U:
			 if(isNumber())
				usartPutString("*");
			 break;
		case NUM2_U:
			if(isNumber())
				usartPutString("*\r\n\r\n");
			break;
		case LOCKED:
			if(button == password[0])
				usartPutString("*");
			else
				usartPutString("Sev je zakljucan!\r\n\r\n");
			break;
		case NUM1_L:
			if(button == password[1])
				usartPutString("*");
			break;
		case NUM2_L:
			if(button == password[2])
				usartPutString("*\r\n\r\n");
			break;
		default: break;
	}
}

/**
 * @brief Handles state transitions based on the current state and button press.
 */
void curentState(){
	switch(state){
		// Switch case to handle state transitions
		case CLOSED:
			switch(button){
				case 'A': next_state = OPEN; led = LED_BLINK; blink_counter = 1; break;
				case '#': next_state = UNLOCKED; led = LED_ON; break;
				default: next_state = CLOSED;
			}
			break;
		case OPEN:
			switch(button){
				case 'B': next_state = CLOSED; led = LED_BLINK; blink_counter = 2; break;
				default: next_state = OPEN;
			}
			break;
		case UNLOCKED:
			if(isNumber()){
				next_state = NUM1_U; led = LED_BLINK; password[0] = button; const_blink = true;
			}else if(button == '#'){
				next_state = CLOSED; led = LED_OFF;
			}else{
				 next_state = UNLOCKED;
			}
			break;
		case NUM1_U:
			if(isNumber()){
				next_state = NUM2_U; led = LED_BLINK; password[1] = button; const_blink = true;
			}else{
				next_state = NUM1_U;

			}
			break;
		case NUM2_U:
			if(isNumber()){
				next_state = LOCKED; led = LED_OFF; password[2] = button; const_blink = false;
			}else{
				next_state = NUM2_U;
			}
			break;
		case LOCKED:
			if(button == password[0]){
				next_state = NUM1_L;
				led = LED_BLINK;
				blink_counter = 1;
			}else{
				next_state = LOCKED;
				led = LED_BLINK;
				blink_counter = 2;
			}
			break;
		case NUM1_L:
			if(button == password[1]){
				next_state = NUM2_L;
				led = LED_BLINK;
				blink_counter = 1;
			}else{
				next_state = LOCKED;
				led = LED_BLINK;
				blink_counter = 2;
			}
			break;
		case NUM2_L:
			if(button == password[2]){
				next_state = UNLOCKED;
				led = LED_ON;
			}else{
				next_state = LOCKED;
				led = LED_BLINK;
				blink_counter = 2;
			}
			break;
		default: break;
	}
}

/**
 * @brief Updates the state of the LED based on the system state and conditions.
 */
void ledState(){
	switch(led){
	 	 // Switch case to control LED state
		case LED_OFF:
			pinSetValue(PORT_B, DIODE_PIN, LOW);
			break;
		case LED_ON:
			pinSetValue(PORT_B, DIODE_PIN, HIGH);
			break;
		case LED_BLINK:
			if(const_blink){
				timer0DelayMs(BLINK_MS);
				pinSetValue(PORT_B, DIODE_PIN, HIGH);
				timer0DelayMs(BLINK_MS);
				pinSetValue(PORT_B, DIODE_PIN, LOW);
			}
			else if(blink_counter > 0){
				while(blink_counter != 0){
					timer0DelayMs(BLINK_MS);
					pinSetValue(PORT_B, DIODE_PIN, HIGH);
					timer0DelayMs(BLINK_MS);
					pinSetValue(PORT_B, DIODE_PIN, LOW);
					blink_counter--;
				}
			led = LED_OFF;
			}
			break;
		default: break;
	}
}

//void listen(){
//	while(!(usartAvailable() != 0)){
//		}
//
//		_delay_ms(64);
//		usartGetString(word);
//		usartPutString("\r\n");
//}
