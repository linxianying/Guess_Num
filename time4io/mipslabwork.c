#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */
#include "sidefuncs.h"

#define TRUE 1
#define FALSE 0

volatile char * mPORTE = (volatile char * ) 0xbf886110;

const unsigned int  Menu = 0,
                    FirstTimeRun = 1,
                    Introduction = 2,
                    GuessInit = 3,
                    GuessBig = 4,
                    GuessNearBig = 5,
                    Guesssmall = 6,
                    GuessNearSmall = 7,
                    Win = 8,
                    TimeLimit = 9,
                    AttemptsMax = 10;




const unsigned int MAX_GUESSES = 6;

const char ARROW = 62;

unsigned int timeoutcount;
unsigned int randomSeed;
unsigned int input2;
unsigned int input = 1;
unsigned int rightAnswer = 100;
unsigned int attempts;
unsigned int times;


/* Lab-specific initialization goes here */
void labinit( void ){
    volatile char * mTRISE = (volatile char *) 0xbf886100;
    volatile short * mTRISD = (volatile short *) 0xbf8860C0;

    *mTRISE = 0x00;     // Set to 0 for output.
    *mTRISD = *mTRISD | 0x0fe0; // Set bits 5-11 to input

    //Clock init
    timeoutcount = 0;
    T2CON = 0x70;
    T2CONSET = 0x8000;
    PR2 = (80000000 / 256 ) / 10;
    return;
}


/**
 * Timer that controlls the screens refreshrate, the blinking of the leds and supplies
 * a random seed for the sequence generator.
 */
int timer(void){
    if(IFS(0) & 0x100){
        IFS(0) = 0;
        timeoutcount++;
        randomSeed = (randomSeed + 2) % 100;
        return 1;
    }
    return 0;
}

void tostring(char str[], int num)
{
    int i, rem, len = 0, n;
 
    n = num;
    while (n != 0)
    {
        len++;
        n /= 10;
    }
    for (i = 0; i < len; i++)
    {
        rem = num % 10;
        num = num / 10;
        str[len - (i + 1)] = rem + '0';
    }
    str[len] = '\0';
}
 


/**
 * Updates the screen with the information given from the mastermind loop. Is ONLY called from the mastermind loop
 * Is not allowed to change the value from any of the inputs it is given, only allowed to display thing on the screen.
 */
void updateScreen(const unsigned int currentScreen) {
    if (currentScreen == Menu) {
        display_string(0, "\tGame ver1.0");
        display_string(1, "\tWelcome!");
        display_string(2, "\tGuessGod");
        display_string(3, "\tBtn to play");
    } else if (currentScreen == FirstTimeRun) {
        display_string(0, "\tFirst Time?");
        display_string(1, "\tIntro:");
        display_string(2, "\t3 Levels");
        display_string(3, "\tNext");
    } else if (currentScreen == Introduction) {
        display_string(0, "\tGuess a number");
        display_string(1, "\tChange with btn ");
        display_string(2, "\tcomfirm with swt");
        display_string(3, "\tready to play?");
    } else if (currentScreen == GuessInit) {
        display_string(0, "\tGuess!");
        char str1[] = "\tInput:";
        char str2[10];
        tostring(str2,input);
        strcat(str1, str2);
        display_string(1, str1);
        display_string(2, "\tChange input with button");
        display_string(3, "\t");
    } else if (currentScreen == GuessBig) {
        display_string(0, "\tWrong answer!");
        display_string(1, "\tInput is:");
        display_string(2, "\tA is less");
        display_string(3, "\tNot even close. xd");
    } else if (currentScreen == GuessNearBig) {
        display_string(0, "\t Wrong answer!");
        display_string(1, "\t Input is:");
        display_string(2, "\t A is less");
        display_string(3, "\t Almost，Almost.");
    } else if (currentScreen == Guesssmall) {
        display_string(0, "\tWrong answer!");
        display_string(1, "\tInput is:");
        display_string(2, "\tA is larger");
        display_string(3, "\tNot even close. xd");
    } else if (currentScreen == GuessNearSmall) {
        display_string(0, "\t Wrong answer!");
        display_string(1, "\t Input is:");
        display_string(2, "\t A is larger ");
        display_string(3, "\t Almost，Almost.");
    } else if (currentScreen == Win) {
        display_string(0, "\tRight Answer!");
        display_string(1, "\tcongratulation!");
        display_string(2, "\tYour Attempts:");
        display_string(3, "\tPlay Again?");
    } else if (currentScreen == TimeLimit) {
        display_string(0, "\tSorry.");
        display_string(1, "\tTime Limit exceeded");
        display_string(2, "\tU Lost the game");
        display_string(3, "\tPlay again?");
    } else if (currentScreen == AttemptsMax) {
        display_string(0, "\tSorry.");
        display_string(1, "\tNo attempts");
        display_string(2, "\tU Lost the game");
        display_string(3, "\tPlay again?");
    }
    display_update();
}

/**
 * Returns 1 if the button was pressed last cycle but is not pressed
 * this cycle. Ergo the button was released
 */
char buttonPressed(int buttonNr, int buttons, int * lastBtns){
    int temp = powerTo(2,buttonNr - 1);

    if( (temp & *lastBtns) && ((buttons & temp) == 0) )                                                                                                                                                                                                                //if(    (((temp & ~(buttons)) & (lastBtns & temp)) == temp)                    && lastBtns)
        return 1;
    return 0;
}

const int UPDATE_SCREEN         = 0;
const int ITER_INCREASE         = 1;
const int SYMBOL_INCREASE       = 2;
const int SYMBOL_INDEX_CHANGE   = 3;
const int RESET                 = 4;
const int CHECK_ANSWER          = 5;
const int REMOVE_BLINKS         = 6;
const int GOTO_MAP              = 7;


void usebtns(const char currentScreen, int * lastBtns, int * dataArray, int firstTime) {
    int btns = getbtns();
    if (currentScreen == Introduction) {
        if (buttonPressed(1, btns, lastBtns)) {
            dataArray[UPDATE_SCREEN] = GuessInit;
        }
    } else if (currentScreen == Menu) {
        if (buttonPressed(1, btns, lastBtns)) {
            if (firstTime == 0) {
                dataArray[UPDATE_SCREEN] = FirstTimeRun;
                firstTime = 1;
            } else {
                dataArray[UPDATE_SCREEN] = GuessInit;
            }
        }
    } else if (currentScreen == FirstTimeRun) {
        if (buttonPressed(1, btns, lastBtns)) {
            dataArray[UPDATE_SCREEN] = Introduction;
        }
    } else if (currentScreen == GuessBig || currentScreen == GuessNearBig || currentScreen == Guesssmall || currentScreen == GuessNearSmall) {
        if (buttonPressed(1, btns, lastBtns)) {
            dataArray[UPDATE_SCREEN] = GuessInit;
        }
    } else if (currentScreen == GuessInit) {
        if (buttonPressed(1, btns, lastBtns)) {
            dataArray[CHECK_ANSWER] = TRUE;
        } else if (buttonPressed(4, btns, lastBtns)) {
            input += 100;
            dataArray[UPDATE_SCREEN] = GuessInit;
        } else if (buttonPressed(3, btns, lastBtns)) {
            input += 10;
            dataArray[UPDATE_SCREEN] = GuessInit;
        } else if (buttonPressed(2, btns, lastBtns)) {
            input += 1;
            dataArray[UPDATE_SCREEN] = GuessInit;
        }
        if (input > 1000) {
            input = 0;
        }
    } else if (currentScreen == Win || currentScreen == TimeLimit || currentScreen == AttemptsMax) {
        if (buttonPressed(1, btns, lastBtns)) {
            dataArray[RESET] = 1;
        }
    }
    * lastBtns = btns;
}

void guessgod(void) {
    char currentScreen = 0;
    int lastBtns = 0;
    int dataArray[8];
    int dataArray_length = sizeof(dataArray) / sizeof(dataArray[0]);
    int i;
    times = 0;
    attempts = 0;
    for (i = 0; i < dataArray_length; i++) {
        dataArray[i] = 0;
    }
    i = 0;
    int difference;
    int bigger;
    rightAnswer = 100;

    int firstTime;
    int timeMax = 10000;
    int attemptsMax = 10;
    while (TRUE) {
        if (dataArray[CHECK_ANSWER] == TRUE) {
            if (input == rightAnswer) {
                dataArray[UPDATE_SCREEN] = Win;
            } else if (times >= timeMax) {
                dataArray[UPDATE_SCREEN] = TimeLimit;
            } else if (attempts >= attemptsMax) {
                dataArray[UPDATE_SCREEN] = AttemptsMax;
            } else {
              
              difference = rightAnswer - input;
                
                if(difference<0){
                  dataArray[UPDATE_SCREEN] = GuessBig;
                }/*else if((abs(difference)<50 )&&(bigger ==1)){
                  dataArray[UPDATE_SCREEN] = GuessNearBig;}}*/
                
              else if(difference>0){
                  dataArray[UPDATE_SCREEN] = Guesssmall;
                }/*else if((abs(difference)<50 )&&(bigger !=1)){
                  dataArray[UPDATE_SCREEN] = GuessNearSmall;
                }*/
            }
        }
        if (dataArray[RESET]) {
            times = 0;
            attempts = 0;
            for (i = 0; i < dataArray_length; i++) {
                dataArray[i] = 0;
            }
            i = 0;
            bigger = 0;
            input = 1;
            rightAnswer = 50;
        }
        if (dataArray[UPDATE_SCREEN] != -1) {
            currentScreen = dataArray[UPDATE_SCREEN];
        }
        updateScreen(currentScreen);
      
        for (i = 0; i < dataArray_length; i++) {
            dataArray[i] = 0;
        }
        i = 0;
        dataArray[UPDATE_SCREEN] = -1;
        usebtns(currentScreen, & lastBtns, dataArray, firstTime);
    }
}
void user_isr(void) {
    return;
}
int abs(int number){
		if(number>=0)
      return number;
    else
      return -number;
}
