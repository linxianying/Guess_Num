#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */
#include "sidefuncs.h"

#define TRUE 1
#define FALSE 0

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
                    AttemptsMax = 10,
                    EventA1 = 11,
                    EventA2 = 12,
                    EventA3 = 13,
                    Rich = 14,
                    Promotion =15,
                    Work1 = 16,
                    Work2 = 17,
                    Work3 = 18,
                    Athome = 19;

const unsigned int MAX_GUESSES = 6;

const char ARROW = 62;

unsigned int timeoutcount;
unsigned int initialRandom = 38;
unsigned int randomSeed = 100;
unsigned int input2;
unsigned int input = 1;
unsigned int rightAnswer = 100;
unsigned int money=1;
unsigned int times;
unsigned int knif;
unsigned int health;
unsigned int promo;
unsigned int randomEvents;
unsigned int police;
unsigned int salary;
unsigned int attempts = 0;
unsigned int led = 1;

int getRandom(int num){
	int seed = findPrime(num)%73*35%66 + 51;
	seed++;
	int random = seed%500 + seed%299 + seed%199 + 3;
	return random;
}

int findPrime(int num){
	int i,j,temp,count=0;
	if(num <= 1)
		return 2;
	if(num >= 1000)
		num = num -999;
    for(i=2;i<=1000;i++){
        temp=0;
        for(j=2;j<=100;j++){
            if(i%j==0){
                temp=1;
                break;
            }
        }
        if(temp==0){
            count++;
        }
        if(count==num){
            return i;
        }
    }
	return num%99+88;
}

/* Lab-specific initialization goes here */
void labinit( void ){

    TRISD = TRISD|0x0ff0; // Set bits 5-11 to input
	TRISE = TRISE&~0xff;
	
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

void tostring(char str[], int num) {
    int i, rem, len = 0, n;

    n = num;
    while (n != 0) {
        len++;
        n /= 10;
    }
    for (i = 0; i < len; i++) {
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
        display_string(2, "\tMonopolyG");
        display_string(3, "\tp Btn to play");
    } else if (currentScreen == FirstTimeRun) {
        display_string(0, "\tFirst Time?");
        display_string(1, "\tintroduct");
        display_string(2, "\tMonopolyG ");
        display_string(3, "\tNext");
    } else if (currentScreen == Introduction) {
        display_string(0, "\tWin Condition");
        display_string(1, "\t1 lottery");
        display_string(2, "\t2 promotion");
        display_string(3, "\t3 rich");
    } else if (currentScreen == Athome) {
        char str1[] = "\tMoney:";
        char str2[10];
        tostring(str2, money);
        strcat(str1, str2);
        display_string(0, "\t1 work");
        display_string(1, "\t2 lottery");
        display_string(2, "\t3 goOutside");
        display_string(3, str1);
    } else if (currentScreen == GuessInit) {
        display_string(0, "\tbuyALottery!");
        char str1[] = "\tNo:";
        char str2[10];
        tostring(str2, input);
        strcat(str1, str2);
        display_string(1, str1);
        display_string(2, "\tselect Num with btn");
        display_string(3, "\tcofirm with btn1");
    } else if (currentScreen == GuessBig) {
        display_string(0, "\tunfortunately");
        display_string(1, "\tYou missed");
        display_string(2, "\tcostMoney(1)");
        display_string(3, "\tNot even close. xd");
    } else if (currentScreen == GuessNearBig) {
        display_string(0, "\t Lotto Consolation");
        display_string(1, "\t Prizes");
        display_string(2, "\t ++Money(1)");
        display_string(3, "\t Not bad.");
    } else if (currentScreen == Guesssmall) {
        display_string(0, "\tunfortunately");
        display_string(1, "\tYou missed");
        display_string(2, "\tcostMoney(1)");
        display_string(3, "\tclose to get");
    } else if (currentScreen == GuessNearSmall) {
        display_string(0, "\tunfortunately");
        display_string(1, "\t Input is:");
        display_string(2, "\t A is larger ");
        display_string(3, "\t Almost，Almost.");
    } else if (currentScreen == Win) {
        display_string(0, "\tJackpot!");
        display_string(1, "\tcongratulation!");
        display_string(2, "\tYou Win");
        display_string(3, "\tPlay Again?");
    } else if (currentScreen == TimeLimit) {
        display_string(0, "\tSorry.");
        display_string(1, "\tTime Limit exceeded");
        display_string(2, "\tU Lost the game");
        display_string(3, "\tPlay again?");
    } else if (currentScreen == AttemptsMax) {
        display_string(0, "\tGG.");
        display_string(1, "\tdiedOfStarvation");
        display_string(2, "\tU Lost the game");
        display_string(3, "\tPlay again?");
    } else if (currentScreen == EventA1) {
        display_string(0, "\tRandom event");
        display_string(1, "\tStore-");
        display_string(2, "\t1 Fraud(3)");
        display_string(3, "\t2 robbery(4)");
    } else if (currentScreen == EventA2) {
        display_string(0, "\tRandom event");
        display_string(1, "\tBank-");
        display_string(2, "\t1 salary(2)");
        display_string(3, "\t2 robbery(N) ");
    } else if (currentScreen == EventA3) {
        display_string(0, "\tprison");
        display_string(1, "\tarrested");
        display_string(2, "\tU Lost the game");
        display_string(3, "\tPlay again?");
    } else if (currentScreen == Rich) {
        display_string(0, "\tcongratulation");
        display_string(1, "\tyouBecomeRich");
        display_string(2, "\tYou Win");
        display_string(3, "\tPlay Again?");
    } else if (currentScreen == Work1) {
        display_string(0, "\tWorking.");
        display_string(1, "\tSitework-");
        display_string(2, "\t1 workHard");
        display_string(3, "\t2 pretend");
    } else if (currentScreen == Work2) {
        display_string(0, "\tuGetMoney");
        display_string(1, "\t++Money(2)");
        display_string(2, "\t1 buy lottery");
        display_string(3, "\t2 banktoHome");
    } else if (currentScreen == Work3) {
        display_string(0, "\tovertime");
        display_string(1, "\tu died");
        display_string(2, "\tU Lost the game");
        display_string(3, "\tPlay again?");
    } else if (currentScreen == Promotion) {
        display_string(0, "\tcongratulation");
        display_string(1, "\tyou get promoted");
        display_string(2, "\tYou Win");
        display_string(3, "\tPlay Again?");
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

int power(int a,int b) { 
	int n=1; 
	int x=0;
	while(x<b) {
		n=n*a; 
		x++;
	}
	return n; 
} 


void addLED(void){
	if(led == 0){
		PORTE = 0x1;
	}else if(led>8){
		led = 0;
		PORTE = 0x1;
	}else if(led == 8){
		PORTE = 0x0;
	}else{
		int loop = power(2,led);
		while(loop>0){
			PORTE += 1;
			loop--;
		}
		
	}
	led++;
	return;
}

void minusLED(void){
	if(led <= 0 || PORTE == 0x0){
		PORTE = 0x0;
	}else{
		int loop = power(2,led-1);
		while(loop>0){
			PORTE -= 1;
			loop--;
		}
	}
	led--;
	return;
}

const int UPDATE_SCREEN         = 0;
const int RESET                 = 1;
const int CHECK_ANSWER          = 2;


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
                dataArray[UPDATE_SCREEN] = Athome;
            }
        }
    } else if (currentScreen == Athome) {
        if (money > 10) {
            dataArray[UPDATE_SCREEN] = Rich;
        } else
        if (buttonPressed(1, btns, lastBtns)) {
            dataArray[UPDATE_SCREEN] = Work1;
        } else
        if (buttonPressed(2, btns, lastBtns)) {
            dataArray[UPDATE_SCREEN] = GuessInit;
        } else
        if (buttonPressed(3, btns, lastBtns)) {
            health = health - 1;
            if (randomEvents = 0) {
                dataArray[UPDATE_SCREEN] = EventA1;
                randomEvents++;
            } else {
                dataArray[UPDATE_SCREEN] = EventA2;
                randomEvents = randomEvents - 1;
            }
        }
    } else if (currentScreen == EventA1) {
        if (buttonPressed(1, btns, lastBtns)) {
            if (police <= 2) {
                dataArray[UPDATE_SCREEN] = Work2;
                police = police + 2;
                money = money + 3;
				addLED();
				addLED();
				addLED();
                if (money > 10) {
                    money = 10;
					PORTE = 0xff;
                }
            } else {
                dataArray[UPDATE_SCREEN] = EventA3;
            }
        } else if (buttonPressed(2, btns, lastBtns)) {
            if (police <= 2) {
                dataArray[UPDATE_SCREEN] = Work2;
                police = police + 3;
                money = money + 4;
				addLED();
				addLED();
				addLED();
				addLED();
                if (money > 10) {
                    money = 10;
					PORTE = 0xff;
                }
                knif++;
            } else {
                dataArray[UPDATE_SCREEN] = EventA3;
            }
        }
    } else if (currentScreen == EventA2) {
        if (buttonPressed(1, btns, lastBtns)) {
            dataArray[UPDATE_SCREEN] = Work2;
            money = money + salary;
			int temp = salary;
			while(temp>0){
				addLED();
				temp--;
			}
            salary = 0;
        } else if (buttonPressed(2, btns, lastBtns)) {
            if (police <= 2 && knif != 0) {
                dataArray[UPDATE_SCREEN] = Rich;
                money = 10;
				PORTE = 0xff;
            } else {
                dataArray[UPDATE_SCREEN] = EventA3;
            }
        }
    } else if (currentScreen == Work1) {
        if (buttonPressed(1, btns, lastBtns)) {
            health++;
            if (health > 5) {
                dataArray[UPDATE_SCREEN] = Work3;
            } else {
                salary = salary + 2;
                police = police - 1;
                dataArray[UPDATE_SCREEN] = Work2;
            }
        } else if (buttonPressed(2, btns, lastBtns)) {
            promo++;
            health = health - 1;
            salary = salary + 2;
            if (promo > 3) {
                dataArray[UPDATE_SCREEN] = Promotion;
            } else {
                dataArray[UPDATE_SCREEN] = Work2;
            }
        }
    } else if (currentScreen == Work2) {
        if (buttonPressed(1, btns, lastBtns)) {
            dataArray[UPDATE_SCREEN] = GuessInit;
        } else if (buttonPressed(2, btns, lastBtns)) {
            dataArray[UPDATE_SCREEN] = Athome;
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
		
        if (money < 0) {
            dataArray[UPDATE_SCREEN] = AttemptsMax;
        } else if (buttonPressed(1, btns, lastBtns)) {
            dataArray[CHECK_ANSWER] = TRUE;
			money = money - 2;
			minusLED();
			minusLED();
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
    } else if (currentScreen == Rich || currentScreen == Win || currentScreen == TimeLimit || currentScreen == AttemptsMax || currentScreen == EventA3 || currentScreen == Work3 || currentScreen == Promotion) {
        if (buttonPressed(1, btns, lastBtns)) {
            dataArray[RESET] = 1;
        }
        money = 1;
		PORTE = 0x1;
		led = 0;
        police = 0;
        knif = 0;
        health = 0;
        randomEvents = 0;
        promo = 0;
        salary = 0;
    }
    * lastBtns = btns;
}
void guessgod(void) {
	//volatile int * porte = (volatile int *) 0xbf886110;
    PORTE = 0x1;
	TRISECLR = 0xFF;
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
    rightAnswer = initialRandom;
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
                if (difference < 0) {
                    dataArray[UPDATE_SCREEN] = GuessBig;
					//addLED();
                }
                /*else if((abs(difference)<50 )&&(bigger ==1)){
                                  dataArray[UPDATE_SCREEN] = GuessNearBig;}}*/
                else if (difference > 0) {
                    dataArray[UPDATE_SCREEN] = Guesssmall;
					//addLED();
                }
                /*else if((abs(difference)<50 )&&(bigger !=1)){
                                  dataArray[UPDATE_SCREEN] = GuessNearSmall;
                                }*/
            }
        }
        if (dataArray[RESET]) {
            times = 0;
            money = 1;
			PORTE = 0x1;
			led = 0;
            for (i = 0; i < dataArray_length; i++) {
                dataArray[i] = 0;
            }
            i = 0;
            bigger = 0;
            input = 1;
            rightAnswer = getRandom((rightAnswer + 1));
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

