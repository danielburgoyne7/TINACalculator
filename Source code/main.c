//SID: 201219884
//NAME: DANIEL WILLIAM BURGOYNE
//MODULE: ELEC3662
//MINI-PROJECT, SEMESTER 1 2020/21

//PREPROCESSOR DIRECTIVES
//-----------------------
#include "PLL.h"
#include "LCD.h"
#include "initports.h"
#include "math.h"

//PROTOTYPES
//----------
void EnterPassword(void);
void PasswordDisplay(int flag);
void CalculatorMain(void);
double CalculatorRecursion(char input[], int start, int end, int* div_by_zero, int* answer_size_error);
void DisplayAnswer(double answer);
void DivisionByZeroError(void);
void AnswerTooLarge(void);
char GetPressedKey(void);

//GLOBAL VARIABLES
//----------------
char functions_primary[4][4] = {{'1','2','3','+'},
								{'4','5','6','-'},
								{'7','8','9','.'},
								{'=','0','D','S'}};
char functions_secondary[4][4] = {{'1','2','3','*'},
								  {'4','5','6','/'},
								  {'7','8','9','^'},
								  {'=','0','C','S'}};
unsigned int shift = 0;
																
//MAIN
//----
int main(void){
	
    PLLInit(); //initialise PLL (PLL.c)
	SysTickInit(); //initialise SysTick (PLL.c)
	InitialisePorts(); //initialise ports A, B, D and E (initports.c)
	LCDInit(); //initialise LCD (LCD.c)
	EnterPassword(); //password
	while(1) {
		CalculatorMain(); //calculator
	}

}

//make user enter password, before accessing the calculator
//password is 1234
void EnterPassword(void) {

    int i = 0; //counter: index of input digit
    char password[4] = {'1','2','3','4'};
    char input[4] = {' ',' ',' ',' '};

    PasswordDisplay(1); //display "ENTER PASSWORD"
    LCDReturnCursorHome(); //set cursor position to home (first line first char)

    while (1) {
        input[i] = GetPressedKey(); //store pressed key value in input array
        SendDisplayByte('*',0x1); //show input has been detected by displaying '*'
        if (i == 3 && input[0] == password[0] && input[1] == password[1] && input[2] == password[2] && input[3] == password[3]) {
            break; //break if we've reached the end of the input and the input matches the password
        } else if (i == 3) {
            LCDClear(); //clear screen
            PasswordDisplay(0); //display "TRY AGAIN"
            LCDReturnCursorHome();
        }
        i = (i + 1) % 4; //i is in range 0-3
    }

}

//Send text to the LCD -- ENTER PASSWORD and INCORRECT

void PasswordDisplay(int flag) {

    LCDMoveCursor(0,1); //move cursor to second line first char
    if (flag == 0) {
        SendDisplayByte('T',0x1);
        SendDisplayByte('R',0x1);
        SendDisplayByte('Y',0x1);
        SendDisplayByte(' ',0x1);
        SendDisplayByte('A',0x1);
        SendDisplayByte('G',0x1);
        SendDisplayByte('A',0x1);
        SendDisplayByte('I',0x1);
        SendDisplayByte('N',0x1);
    } else if (flag == 1) {
        SendDisplayByte('E',0x1);
        SendDisplayByte('N',0x1);
        SendDisplayByte('T',0x1);
        SendDisplayByte('E',0x1);
        SendDisplayByte('R',0x1);
        SendDisplayByte(' ',0x1);
        SendDisplayByte('P',0x1);
        SendDisplayByte('A',0x1);
        SendDisplayByte('S',0x1);
        SendDisplayByte('S',0x1);
        SendDisplayByte('W',0x1);
        SendDisplayByte('O',0x1);
        SendDisplayByte('R',0x1);
        SendDisplayByte('D',0x1);
    }

}

void CalculatorMain(void) {
	
	char key; //key that has been pressed down (? = no valid down press)
	char input[DDRAM]; //input equation
	char wait_for_keypress;
	int cursor = 0; //LCD cursor position
	int clear = 0;
	int div_by_zero = 0; //division by zero error flag
	int answer_size_error = 0; //flag if answer exceeds INT_MIN or INT_MAX
	double answer;
	shift = 0; //reset shift
	
	LCDClear();
	LCDReturnCursorHome();
		
	while(1) {
		key = GetPressedKey(); //returns keypad state
		if (cursor < (DISPLAY_CHARACTERS - 1) || key == '=') { //do not enact a function if we will exceed the first line capacity, unless evaluating
			if (key == '=' && cursor > 0) {
			    if (input[cursor-1] == '+' || input[cursor-1] == '-' || input[cursor-1] == '*' || input[cursor-1] == '/' || input[cursor-1] == '^') {
			        //do not accept operator then equals
			        //the many nested conditional blocks are necessary for this function
			        //this is because I believe input[-1] will throw an error - which occurs if cursor = 0
			    } else {
			        input[cursor] = key;
				    SendDisplayByte(key,0x1);
				    break;
			    }
			} else if (key == 'C') {
				clear = 1;
				break;
			} else if (key == 'D' && cursor > 0) {
				cursor--;
				input[cursor] = '\0'; //nullify last character
				LCDMoveCursor(cursor,0);
				SendDisplayByte(' ',0x1);
				LCDMoveCursor(cursor,0);
			} else if (key == 'S') {
				shift ^= 1;
			} else if (key == '+' || key == '-' || key == '*' || key == '/' || key == '^') {
                if (cursor == 0) { //prepend 0 if there is no first operand
                    input[cursor] = '0';
                    SendDisplayByte('0',0x1);
                    cursor++;
                }
                if (input[cursor-1] == '+' || input[cursor-1] == '-' || input[cursor-1] == '*' || input[cursor-1] == '/' || input[cursor-1] == '^') {
                    //do not accept consecutive operators
                } else {
                    input[cursor] = key;
                    SendDisplayByte(key,0x1);
                    cursor++;
                }
			} else if ((key <= 57 && key >= 48) || key == '.') { //i.e. if it is a digit 0-9 or '.'
				input[cursor] = key;
				SendDisplayByte(key,0x1);
				cursor++;
			}
		}
	}
	if (clear == 0) {
	    answer = CalculatorRecursion(input,0,cursor-1,&div_by_zero,&answer_size_error);
	    if (answer_size_error == 1) {
	        AnswerTooLarge();
	    } else if (div_by_zero == 1) {
            DivisionByZeroError();
	    } else {
	        DisplayAnswer(answer);
	    }
	    wait_for_keypress = GetPressedKey();
    }
}

//Calculation with correct operator precedence is done using a recursive algorithm
//'Greatest' operator precedence = exponentiation (^) -- value 3
//then division and multiplication -- value 2
//then addition and subtraction -- value 1
double CalculatorRecursion(char input[], int start, int end, int* div_by_zero, int* answer_size_error) {

    int lowest_order_of_precedence[2] = {3,start}; //store lowest-precedence operator order
                                                   //found during scan of equation and its index
                                                   //FIRST ELEMENT: 3 (default) = ^, 2 = / and *, 1 = + and -
                                                   //SECOND ELEMENT: index
    int order_of_i = 0; //order-of-precedence of currently queried operator
    int higher_precedence = 0; //0 (default) means the equation is in left-to-right order of descending operator precedence
                               //1 means there is a section of the equation where a higher-precedence operator follows a lower one
                               //This prevents the equation from being evaluated in one go from left to right
    int i = start; //counter
    int decimal_index = -1; //-1 if not after decimal point
    double answer = 0;
    double operand = 0;
    double result_of_recursion = 0;
    double exponent = 0;
    char active_operator = '+';

    //Iterate through the characters in the equation to find the operators and store their order of precedence.
    //If the order of precedence of the second of a consecutive pair of operators is greater than the first,
    //'break' the operation at that point and enter a recursion.
    //This ensures the equation can be evaluated using a 'basic calculator' method.
    while (higher_precedence == 0 && i <= end) {
        if (input[i] == '+' || input[i] == '-' || input[i] == '*' || input[i] == '/' || input[i] == '^') {
            //Assign order of precedence to the operator
            if (input[i] == '+' || input[i] == '-') {
                order_of_i = 1;
            } else if (input[i] == '*' || input[i] == '/') {
                order_of_i = 2;
            } else if (input[i] == '^') {
                order_of_i = 3;
            }
            //Check if we need to enter a recursion
            if (order_of_i > lowest_order_of_precedence[0]) {
                result_of_recursion = CalculatorRecursion(input,lowest_order_of_precedence[1]+1,end,div_by_zero,answer_size_error);
                end = i;
                higher_precedence = 1;
            } else {
                lowest_order_of_precedence[0] = order_of_i;
                lowest_order_of_precedence[1] = i;
            }
        }
        i++;
    }
    i = start; //return counter to start
    while (i <= end && *div_by_zero == 0 && *answer_size_error == 0) { //break if we reach the end of the equation or a div-by-zero error is thrown
        if (input[i] <= 57 && input[i] >= 48) {
            if (decimal_index == -1) { //if digit is in integer part of a number
                //Shift the digits in the existing operand one position to the left, by multiplying the value by 10.
                //Convert the ASCII value of the character to its digit value which it represents,
                //by converting it to a double and subtracting 48
                operand = operand * 10.0 + ((double) input[i] - 48.0);
            } else { //if digit is in fraction part of a number
                decimal_index++; //denotes index of decimal digit
                exponent = decimal_index * -1.0;
                //Multiply input digit by 10^(-decimal_index) and apply the value of the character
                operand += pow(10.0,exponent) * ((double) input[i] - 48.0);
            }
        } else if (input[i] == '.') {
            decimal_index = 0; //finish with integer part of operand, now act on fractional part
        }
        if (input[i] == '+' || input[i] == '-' || input[i] == '*' || input[i] == '/' || input[i] == '^' || i == end) {
            if (i == end && (input[i] == '+' || input[i] == '-' || input[i] == '*' || input[i] == '/' || input[i] == '^')) {
                //If we have undergone recursion, the end character will be an operator;
                //this signals to apply the result of the recursion.
                operand = result_of_recursion;
            }
            switch (active_operator) {
            case '+': //addition
                answer += operand;
                break;
            case '-': //subtraction
                answer -= operand;
                break;
            case '*': //multiplication
                answer *= operand;
                break;
            case '/': //division
                if (operand != 0.0) {
                    answer /= operand;
                } else { //division by zero, cancel operation
                    *div_by_zero = 1;
                }
                break;
            case '^': //exponentiation
                answer = pow(answer,operand);
                break;
            }
            if (operand <= -2147483648.0 || operand >= 2147483647.0 || answer <= -2147483648.0 || answer >= 2147483647.0) {
                *answer_size_error = 1;
            }
            decimal_index = -1; //reset to integer part
            active_operator = input[i]; //apply next operator
            operand = 0.0; //reset operand value
        }
        i++; //increment counter
    }

    return answer;
}

//Display answer
//maximum of (DISPLAY_CHARACTERS - 2 - neg) digits for integer part of floating-point answer
void DisplayAnswer(double answer) {

    double fractpart; //fraction part of answer
    double intpart_d; //integer part of answer as a double type
    int intpart_i; //integer part of answer as an integer type
    int intpart_max_no_of_digits = DISPLAY_CHARACTERS - 2;
    int intpart_no_of_digits = 0;
    int temp;
    int answer_too_large = 0; //display error flag -- 1 if no. of digits > (DISPLAY_CHARACTERS - 2 - neg)
    int neg = 0; //1 if answer is negative; must fit '-' sign on display
    char digit;

    //display '-' if answer is negative and reduce maximum size of integer part
    if (answer < 0) {
        neg = 1;
        intpart_max_no_of_digits -= neg;
        answer *= -1;
        LCDMoveCursor(0,1);
        SendDisplayByte('-',0x1);
    }

    //split fraction and integer parts of answer
    fractpart = modf(answer,&intpart_d); //modf reference: http://www.cplusplus.com/reference/cmath/modf/
    intpart_i = intpart_d;
    temp = intpart_i;

    //get number of digits of integer part
    while (temp && intpart_no_of_digits <= intpart_max_no_of_digits) {
        temp /= 10;
        intpart_no_of_digits++;
    }

    //display integer part, unless no. of digits > (DISPLAY_CHARACTERS - 2 - neg)
    if (intpart_no_of_digits == 0) {
        LCDMoveCursor(neg,1);
        SendDisplayByte('0',0x1);
        intpart_no_of_digits++;
    } else if (intpart_no_of_digits <= intpart_max_no_of_digits) {
        for (int i = 0; i < intpart_no_of_digits; i++) {
            LCDMoveCursor(intpart_no_of_digits-i-1+neg,1);
            digit = (intpart_i % 10) + 48;
            SendDisplayByte(digit,0x1);
            intpart_i /= 10;
        }
    } else {
        answer_too_large = 1;
    }

    if (answer_too_large == 0) {
        //display fraction part
        LCDMoveCursor(intpart_no_of_digits+neg,1);
        SendDisplayByte('.',0x1);
        for (int j = intpart_no_of_digits + neg + 1; j < DISPLAY_CHARACTERS; j++) {
            fractpart = fmod(fractpart * 10, 10.0);
            digit = (int) fractpart + 48;
            SendDisplayByte(digit,0x1);
        }
    } else {
        AnswerTooLarge();
    }
}

//Display error message
void DivisionByZeroError(void) {
    LCDClear();
    LCDReturnCursorHome();
    SendDisplayByte('D',0x1);
    SendDisplayByte('I',0x1);
    SendDisplayByte('V',0x1);
    SendDisplayByte(' ',0x1);
    SendDisplayByte('B',0x1);
    SendDisplayByte('Y',0x1);
    SendDisplayByte(' ',0x1);
    SendDisplayByte('0',0x1);
    SendDisplayByte(' ',0x1);
    SendDisplayByte('E',0x1);
    SendDisplayByte('R',0x1);
    SendDisplayByte('R',0x1);
    SendDisplayByte('O',0x1);
    SendDisplayByte('R',0x1);
}

void AnswerTooLarge(void) {
    LCDClear();
    LCDReturnCursorHome();
    SendDisplayByte('A',0x1);
    SendDisplayByte('N',0x1);
    SendDisplayByte('S',0x1);
    SendDisplayByte('W',0x1);
    SendDisplayByte('E',0x1);
    SendDisplayByte('R',0x1);
    SendDisplayByte(' ',0x1);
    SendDisplayByte('T',0x1);
    SendDisplayByte('O',0x1);
    SendDisplayByte('O',0x1);
    SendDisplayByte(' ',0x1);
    SendDisplayByte('L',0x1);
    SendDisplayByte('A',0x1);
    SendDisplayByte('R',0x1);
    SendDisplayByte('G',0x1);
    SendDisplayByte('E',0x1);
}

//Wait for a key down, then key up, then return the character associated with the key
char GetPressedKey(void) {

    int j = -1; //row of button pressed -- -1 = no button pressed
    int i = 0; //counter, column of button pressed

    while (j == -1) {
        GPIO_PORTD_DATA_R = 0x01 << i; //toggle voltage ON to each column in turn
        SysTickWait(10); //wait for voltage reading at port E
        if ((GPIO_PORTE_DATA_R&0x01) == 0x01) {
            while((GPIO_PORTE_DATA_R&0x01) == 0x01) {} //wait until key no longer pressed
            j = 0;
        } else if ((GPIO_PORTE_DATA_R&0x02) == 0x02) {
            while((GPIO_PORTE_DATA_R&0x02) == 0x02) {}
            j = 1;
        } else if ((GPIO_PORTE_DATA_R&0x04) == 0x04) {
            while((GPIO_PORTE_DATA_R&0x04) == 0x04) {}
            j = 2;
        } else if ((GPIO_PORTE_DATA_R&0x08) == 0x08) {
            while((GPIO_PORTE_DATA_R&0x08) == 0x08) {}
            j = 3;
        }
        i++;
        if (i == 4 && j == -1) {
            i = 0;
        }
    }
    i -= 1;
    //GPIO_PORTD_DATA_R &= 0x00; //turn port D voltages off
    if (shift == 0) {
        return functions_primary[j][i];
    } else if (functions_secondary[j][i] == 'S') {
        return functions_secondary[j][i];
    } else {
        shift = 0;
        return functions_secondary[j][i];
    }
}
