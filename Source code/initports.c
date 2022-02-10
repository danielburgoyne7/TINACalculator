//PREPROCESSOR DIRECTIVES
//-----------------------
//HEADER
#include "initports.h"

// PORT INITIALISATION SUBROUTINES
// -------------------------------
void InitialisePorts(void) {

	PortAInit();
	PortBInit();
	PortDInit();
	PortEInit();

}

void PortAInit(void) {
	SYSCTL_RCGCGPIO_R |= 0x00000001;  // activate clock
	while ((SYSCTL_PRGPIO_R&0x01)==0) {
		// wait for peripheral ready signal
	}
	GPIO_PORTA_DIR_R = 0x0C;		  // set direction to output
	GPIO_PORTA_AFSEL_R = 0x00;        // regular port function
    GPIO_PORTA_AMSEL_R = 0x00;        // disable analog function for PA2-3
    GPIO_PORTA_PCTL_R = 0x00000000;   // PCTL GPIO for PA2-3
	GPIO_PORTA_DEN_R = 0x0C;          // enable digital port
}

void PortBInit(void) {
	SYSCTL_RCGCGPIO_R |= 0x00000002;  // activate clock
	while ((SYSCTL_PRGPIO_R&0x02)==0) {
		// wait for peripheral ready signal
	}
	GPIO_PORTB_DIR_R = 0x0F;		  // set direction to output
	GPIO_PORTB_AFSEL_R = 0x00;        // regular port function
    GPIO_PORTB_AMSEL_R = 0x00;        // disable analog function for PB0-3
    GPIO_PORTB_PCTL_R = 0x00000000;   // PCTL GPIO for PB0-3
	GPIO_PORTB_DEN_R = 0x0F;          // enable digital port
}

void PortDInit(void) {
	SYSCTL_RCGCGPIO_R |= 0x00000008;  // activate clock
	while ((SYSCTL_PRGPIO_R&0x08)==0) {
		// wait for peripheral ready signal
	}
	GPIO_PORTD_DIR_R = 0x0F;		  // set direction to output
	GPIO_PORTD_AFSEL_R = 0x00;        // regular port function
    GPIO_PORTD_AMSEL_R = 0x00;        // disable analog function for PD0-3
    GPIO_PORTD_PCTL_R = 0x00000000;   // PCTL GPIO for PD0-3
	GPIO_PORTD_DEN_R = 0x0F;          // enable digital port
}

void PortEInit(void) {
	SYSCTL_RCGCGPIO_R |= 0x00000010;  // activate clock
	while ((SYSCTL_PRGPIO_R&0x10)==0) {
		// wait for peripheral ready signal
	}
	GPIO_PORTE_DIR_R = 0x00;		  // set direction to input
	GPIO_PORTE_AFSEL_R = 0x00;        // regular port function
    GPIO_PORTE_AMSEL_R = 0x00;      // disable analog function for PE0-3
    GPIO_PORTE_PCTL_R = 0x00000000;  // PCTL GPIO for PE0-3
	//GPIO_PORTE_PDR_R |= 0x0F;		  // activate pull-down resistors for PE0-3
	GPIO_PORTE_DEN_R = 0x0F;         // enable digital port
}
