//PREPROCESSOR DIRECTIVES
//-----------------------
#ifndef LCD_H
#define LCD_H
#define DDRAM 40 //see HD44780 datasheet p. 29 - no. of characters per line
#define DISPLAY_CHARACTERS 16
#define PA3   (*((volatile unsigned long *)0x40004020))
#define PA2   (*((volatile unsigned long *)0x40004010))

void LCDInit(void);
void LCDClear(void);
void LCDMoveCursor(int x, int y);
void LCDReturnCursorHome(void);
void SendDisplayByte(char byte, char RS);
void SendDisplayNibble(char nibble, char RS);
void PulseEN(void);

#endif
