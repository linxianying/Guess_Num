#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */


//different screens
const unsigned int  MainMenu = 0,
                    FirstTimeRun = 1,
                    Introduction = 2,
                    GuessInit = 3,
                    GuessBig = 4,
                    GuessNearBig = 5,
                    GuessSmall = 6,
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

//different variables
unsigned int timeoutcount;
unsigned int initialRandom = 38;
unsigned int randomSeed = 100;
unsigned int input2;
unsigned int input = 1;
unsigned int rightAnswer = 100;
unsigned int money = 1;
unsigned int times;
unsigned int knif;
unsigned int health;
unsigned int promo;
unsigned int randomEvents;
unsigned int police;
unsigned int salary;
unsigned int attempts = 0;
unsigned int led = 1;

const int update = 0;
const int reset = 1;
const int check = 2;



/* Lab-specific initialization goes here */
void labinit(void){

    TRISD = TRISD|0x0ff0; // 1 for input
	TRISE = TRISE&~0xff;
	
    timeoutcount = 0;
    T2CON = 0x70;
    T2CONSET = 0x8000;
    PR2 = (80000000 / 256 ) / 10;
    return;
}

// return a^b
int power(int a,int b) { 
	int n=1; 
	int x=0;
	while(x<b) {
		n=n*a; 
		x++;
	}
	return n; 
} 


/**
 * Returns 1 if the button was pressed last cycle but is not pressed
 * this cycle. Ergo the button was released
 */
char checkButton(int buttonNr, int buttons, int * pastButton){
    int temp = power(2,buttonNr - 1);

    if( (temp & *pastButton) && ((buttons & temp) == 0) )                                                                                                                                                                                                                //if(    (((temp & ~(buttons)) & (pastButton & temp)) == temp)                    && pastButton)
        return 1;
    return 0;
}

void allLED(void){
	PORTE = 0x11111;
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


void toString(char str[], int num) {
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

int getRandom(int num){
	int seed = findPrime(num)%73*35%66 + 51;
	seed++;
	int random = seed%500 + seed%299 + seed%199 + 3;
	return random;
}



void user_isr(void) {
    return;
}


// to change screens when we needed
void changeScreen(const unsigned int current) {
    if (current == MainMenu) {
        display_string(0, "\tGame ver1.0");
        display_string(1, "\tWelcome!");
        display_string(2, "\tMonopolyG");
        display_string(3, "\tp Btn to play");
    } else if (current == FirstTimeRun) {
        display_string(0, "\tFirst Time?");
        display_string(1, "\tintroduct");
        display_string(2, "\tMonopolyG ");
        display_string(3, "\tNext");
    } else if (current == Introduction) {
        display_string(0, "\tWin Condition");
        display_string(1, "\t1 lottery");
        display_string(2, "\t2 promotion");
        display_string(3, "\t3 rich");
    } else if (current == Athome) {
        char str1[] = "\tMoney:";
        char str2[10];
        toString(str2, money);
        strcat(str1, str2);
        display_string(0, "\t1 work");
        display_string(1, "\t2 lottery");
        display_string(2, "\t3 go out");
        display_string(3, str1);
    } else if (current == GuessInit) {
        display_string(0, "\tbuy lottery!");
        char str1[] = "\tNo:";
        char str2[10];
        toString(str2, input);
        strcat(str1, str2);
        display_string(1, str1);
        display_string(2, "\t Select Num, btn23");
        display_string(3, "\t Confirm, btn1");
    } else if (current == GuessBig) {
        display_string(0, "\t Unfortunately");
        display_string(1, "\t You missed");
        display_string(2, "\t costMoney(1)");
        display_string(3, "\t Not even close");
    } else if (current == GuessNearBig) {
        display_string(0, "\t Lotto Consolation");
        display_string(1, "\t Prizes");
        display_string(2, "\t ++Money(1)");
        display_string(3, "\t Not bad.");
    } else if (current == GuessSmall) {
        display_string(0, "\t unfortunately");
        display_string(1, "\t you missed");
        display_string(2, "\t costMoney(1)");
        display_string(3, "\t close");
    } else if (current == GuessNearSmall) {
        display_string(0, "\t unfortunately");
        display_string(1, "\t Input is:");
        display_string(2, "\t Ans is larger ");
        display_string(3, "\t Almost，Almost.");
    } else if (current == Win) {
        display_string(0, "\t Jackpot!");
        display_string(1, "\t congratulation!");
        display_string(2, "\t You Win");
        display_string(3, "\t Play again?");
    } else if (current == TimeLimit) {
        display_string(0, "\t Sorry.");
        display_string(1, "\t Time Limit exceeded");
        display_string(2, "\t You Lose");
        display_string(3, "\t Play again?");
    } else if (current == AttemptsMax) {
        display_string(0, "\t GG");
        display_string(1, "\t Die of hunger");
        display_string(2, "\t You Lose");
        display_string(3, "\t Play again?");
    } else if (current == EventA1) {
        display_string(0, "\t Random event");
        display_string(1, "\t Store");
        display_string(2, "\t 1 Fraud(3)");
        display_string(3, "\t 2 robbery(4)");
    } else if (current == EventA2) {
        display_string(0, "\t Random event");
        display_string(1, "\t Bank-");
        display_string(2, "\t 1 salary(2)");
        display_string(3, "\t 2 robbery(N) ");
    } else if (current == EventA3) {
        display_string(0, "\t Arrested");
        display_string(1, "\t Prison");
        display_string(2, "\t You Lose");
        display_string(3, "\t Play again?");
    } else if (current == Rich) {
        display_string(0, "\t Congratulation");
        display_string(1, "\t You are rich");
        display_string(2, "\t You Win");
        display_string(3, "\t Play Again?");
    } else if (current == Work1) {
        display_string(0, "\t Working.");
        display_string(1, "\t Sitework-");
        display_string(2, "\t 1 workHard");
        display_string(3, "\t 2 pretend");
    } else if (current == Work2) {
        display_string(0, "\tuGetMoney");
        display_string(1, "\t++Money(2)");
        display_string(2, "\t1 buy lottery");
        display_string(3, "\t2 banktoHome");
    } else if (current == Work3) {
        display_string(0, "\t Work so much");
        display_string(1, "\t You died");
        display_string(2, "\t You Lose");
        display_string(3, "\t Play again?");
    } else if (current == Promotion) {
        display_string(0, "\t Congratulation");
        display_string(1, "\t You get promoted");
        display_string(2, "\t You Win");
        display_string(3, "\t Play Again?");
    }
    display_update();
}

void usebtns(const char current, int * pastButton, int * arr, int firstTime) {
    int btns = getbtns();
    if (current == Introduction) {
        if (checkButton(1, btns, pastButton)) {
            arr[update] = GuessInit;
        }
    } else if (current == MainMenu) {
        if (checkButton(1, btns, pastButton)) {
            if (firstTime == 0) {
                arr[update] = FirstTimeRun;
                firstTime = 1;
            } else {
                arr[update] = Athome;
            }
        }
    } else if (current == Athome) {
        if (money > 10) {
            arr[update] = Rich;
        } else
        if (checkButton(1, btns, pastButton)) {
            arr[update] = Work1;
        } else
        if (checkButton(2, btns, pastButton)) {
            arr[update] = GuessInit;
        } else
        if (checkButton(3, btns, pastButton)) {
            health = health - 1;
            if (randomEvents = 0) {
                arr[update] = EventA1;
                randomEvents++;
            } else {
                arr[update] = EventA2;
                randomEvents = randomEvents - 1;
            }
        }
    } else if (current == EventA1) {
        if (checkButton(1, btns, pastButton)) {
            if (police <= 2) {
                arr[update] = Work2;
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
                arr[update] = EventA3;
            }
        } else if (checkButton(2, btns, pastButton)) {
            if (police <= 2) {
                arr[update] = Work2;
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
                arr[update] = EventA3;
            }
        }
    } else if (current == EventA2) {
        if (checkButton(1, btns, pastButton)) {
            arr[update] = Work2;
            money = money + salary;
			int temp = salary;
			while(temp>0){
				addLED();
				temp--;
			}
            salary = 0;
        } else if (checkButton(2, btns, pastButton)) {
            if (police <= 2 && knif != 0) {
                arr[update] = Rich;
                money = 10;
				PORTE = 0xff;
            } else {
                arr[update] = EventA3;
            }
        }
    } else if (current == Work1) {
        if (checkButton(1, btns, pastButton)) {
            health++;
            if (health > 5) {
                arr[update] = Work3;
            } else {
                salary = salary + 2;
                police = police - 1;
                arr[update] = Work2;
            }
        } else if (checkButton(2, btns, pastButton)) {
            promo++;
            health = health - 1;
            salary = salary + 2;
            if (promo > 3) {
                arr[update] = Promotion;
            } else {
                arr[update] = Work2;
            }
        }
    } else if (current == Work2) {
        if (checkButton(1, btns, pastButton)) {
            arr[update] = GuessInit;
        } else if (checkButton(2, btns, pastButton)) {
            arr[update] = Athome;
        }
    } else if (current == FirstTimeRun) {
        if (checkButton(1, btns, pastButton)) {
            arr[update] = Introduction;
        }
    } else if (current == GuessBig || current == GuessNearBig || current == GuessSmall || current == GuessNearSmall) {
        if (checkButton(1, btns, pastButton)) {
            arr[update] = GuessInit;
        }
    } else if (current == GuessInit) {
		
        if (money < 0) {
            arr[update] = AttemptsMax;
        } else if (checkButton(1, btns, pastButton)) {
            arr[check] = 1;
			money = money - 2;
			minusLED();
			minusLED();
        } else if (checkButton(4, btns, pastButton)) {
            input += 100;
            arr[update] = GuessInit;
        } else if (checkButton(3, btns, pastButton)) {
            input += 10;
            arr[update] = GuessInit;
        } else if (checkButton(2, btns, pastButton)) {
            input += 1;
            arr[update] = GuessInit;
        }
        if (input > 1000) {
            input = 0;
        }
    } else if (current == Rich || current == Win || current == TimeLimit || current == AttemptsMax || current == EventA3 || current == Work3 || current == Promotion) {

		if (checkButton(1, btns, pastButton)) {
            arr[reset] = 1;
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
    * pastButton = btns;
}

void guessgod(void) {
	//volatile int * porte = (volatile int *) 0xbf886110;
    PORTE = 0x1;
	TRISECLR = 0xFF;
    char current = 0;
    int pastButton = 0;
    int arr[8];
    int arrLength = sizeof(arr) / sizeof(arr[0]);
    int i;
    times = 0;
    attempts = 0;
    for (i = 0; i < arrLength; i++) {
        arr[i] = 0;
    }
    i = 0;
    int difference;
    int bigger;
    rightAnswer = initialRandom;
    int firstTime;
    int timeMax = 10000;
    int attemptsMax = 10;
    while (1) {
        if (arr[check] == 1) {
            if (input == rightAnswer) {
                arr[update] = Win;
            } else if (times >= timeMax) {
                arr[update] = TimeLimit;
            } else if (attempts >= attemptsMax) {
                arr[update] = AttemptsMax;
            } else {
                difference = rightAnswer - input;
                if (difference < 0) {
                    arr[update] = GuessBig;
					//addLED();
                }
                /*else if((abs(difference)<50 )&&(bigger ==1)){
                                  arr[update] = GuessNearBig;}}*/
                else if (difference > 0) {
                    arr[update] = GuessSmall;
					//addLED();
                }
                /*
				  else if((abs(difference)<50 )&&(bigger !=1)){
				      arr[update] = GuessNearSmall;
				}*/
            }
        }
        if (arr[reset]) {
            times = 0;
            money = 1;
			PORTE = 0x1;
			led = 0;
            for (i = 0; i < arrLength; i++) {
                arr[i] = 0;
            }
            i = 0;
            bigger = 0;
            input = 1;
            rightAnswer = getRandom((rightAnswer + 1));
        }
        if (arr[update] != -1) {
            current = arr[update];
        }
        changeScreen(current);
        for (i = 0; i < arrLength; i++) {
            arr[i] = 0;
        }
        i = 0;
        arr[update] = -1;
        usebtns(current, & pastButton, arr, firstTime);
    }
}
