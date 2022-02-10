#include "LCD.h"
#include "initports.h"
#include "PLL.h"

//initialise LCD (referencing HD44780U datasheet p46 - for 3.3V = Vcc)
void LCDInit(void) {

    PA2 = 0x00; //initially set RS and EN low
    PA3 = 0x01 << 3; //TEST
    PA3 = 0x00;
	//DELAY SysTickWait(3200000); //wait for 40ms
	SendDisplayNibble(0x03,0x0);
	//DELAY SysTickWait(328000); //wait for 4.1ms
	SendDisplayNibble(0x03,0x0);
	//DELAY SysTickWait(8000); //wait for 100µs
	SendDisplayNibble(0x03,0x0);
	//DELAY SysTickWait(2960); //wait for 37µs
	SendDisplayNibble(0x02,0x0);
	SendDisplayByte(0x28,0x0);
	SendDisplayByte(0x08,0x0);
	SendDisplayByte(0x01,0x0);
	SendDisplayByte(0x06,0x0);

}

void LCDClear(void) {
	SendDisplayByte(0x01,0x0);
}

void LCDMoveCursor(int x, int y) {
	char address = 0x00;

	for (int i = 0; i < x; i++) { //until address = x
		address += 0x01;
	}
	if (y != 0) {
		address += 0x40; //0x40 must be added to the address, for the characters to be displayed on the second line
		                 //-- see HD44780 datasheet p. 29
	}
	address |= 0x80; //necessary to set DDRAM address -- see HD44780 datasheet p. 30
	SendDisplayByte(address,0x0); 
}

void LCDReturnCursorHome(void) {
    SendDisplayByte(0x02,0x0); //return cursor home (first line first char)
    //DELAY SysTickWait(121600); //wait for 1.52ms
}

//send byte to LCD as two nibbles, MSB then LSB (4-bit interfacing mode)
void SendDisplayByte(char byte, char RS) {
    char MSB;
    char LSB;

    MSB = (byte & 0xF0) >> 4; //most-significant bits, right-shift before transmission on PB0-3
    SendDisplayNibble(MSB,RS);
    LSB = byte & 0x0F; //least-significant bits
    SendDisplayNibble(LSB,RS);
    //DELAY SysTickWait(2960); //wait for 37µs
}

//send bits to LCD on PB0-3
void SendDisplayNibble(char nibble, char RS){
    nibble &= 0x0F; //clear char bits 4-7
    GPIO_PORTB_DATA_R = nibble; //send data on PB0-3
    PA3 = RS << 3; //send data on RS
    PulseEN(); //pulse EN line, so that the function can be enacted on the LCD
}

//the EN line must be pulsed for the bits in port B to be received by the LCD
void PulseEN(void) {
    PA2 |= 0x04; //turn on EN line
    SysTickWait(36); //wait for 450ns
    PA2 &= 0x00; //turn off EN line
}
