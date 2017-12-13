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
unsigned int times;
unsigned int knif;
unsigned int health;
unsigned int promo;
unsigned int randomEvents;
unsigned int police;
unsigned int salary;
unsigned int attempts = 0;
unsigned int led = 1;
signed int money = 1;

const int update = 0;
const int reset = 1;
const int check = 2;

unsigned int checkUpdate;
unsigned int checkValue;
unsigned int checkReset;

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
void allLED(void){
	led = 0;
	PORTE = 0x1;
	int i=0;
	for(i=0;i<8;i++)
		addLED();
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


void turnToString(char charList[], int num) {
	int remainder;
    int temp = num;
	int length = 0;
    while (temp != 0) {
        length++;
        temp = temp / 10;
    }
	int i;
    for (i =0;i<length;i++) {
        remainder = num%10;
        num = num/10;
        charList[length-i-1] = remainder+'0';
    }
    charList[length] = '\0';
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
	return num;
}

int getRandom(int num){
	int seed = findPrime(num)*35%1251;
	int random = seed%1001;
	
	//srand(seed);
	//int random = rand()%1001;
	return random;
}



void user_isr(void) {
    return;
}


// to change screens when we needed
void changeScreen(const unsigned int current) {
    if (current == MainMenu) {
		display_image(96, icon);
        display_string(0, "\tWelcome");
        display_string(1, "\tto");
        display_string(2, "\tLotteryGod");
        display_string(3, "\tBtn to play");
    } else if (current == FirstTimeRun) {
		display_image(96, icon);
        display_string(0, "\tFirst time?");
        display_string(1, "\tIntro");
        display_string(2, "\tLotteryGod ");
        display_string(3, "\tNext");
    } else if (current == Introduction) {
		display_image(96, icon);
        display_string(0, "\tWin Conditions");
        display_string(1, "\t1 lottery");
        display_string(2, "\t2 promotion");
        display_string(3, "\t3 rich");
    } else if (current == Athome) {
        char str1[] = "\tMoney:";
        char str2[10];
        turnToString(str2, money);
        strcat(str1, str2);
        display_string(0, "\t1 work");
        display_string(1, "\t2 lottery");
        display_string(2, "\t3 go out");
        display_string(3, str1);
    } else if (current == GuessInit) {
        display_string(0, "\tbuy lottery!");
        char str1[] = "\tNo:";
        char str2[10];
        turnToString(str2, input);
        strcat(str1, str2);
        display_string(1, str1);
        display_string(2, "\tSelect Num, btn23");
        display_string(3, "\tConfirm, btn1");
    } else if (current == GuessBig) {
        display_string(0, "\tUnfortunately");
        display_string(1, "\tYou missed");
        display_string(2, "\tcostMoney(1)");
        display_string(3, "\tNot even close");
    } else if (current == GuessNearBig) {
        display_string(0, "\tLotto Consolation");
        display_string(1, "\tPrizes");
        display_string(2, "\t++Money(1)");
        display_string(3, "\tNot bad.");
    } else if (current == GuessSmall) {
        display_string(0, "\tunfortunately");
        display_string(1, "\tyou missed");
        display_string(2, "\tcostMoney(1)");
        display_string(3, "\tclose");
    } else if (current == GuessNearSmall) {
        display_string(0, "\tunfortunately");
        display_string(1, "\tInput is:");
        display_string(2, "\tAns is larger ");
        display_string(3, "\tAlmost，Almost.");
    } else if (current == Win) {
		allLED();
		display_image(96, icon);
        display_string(0, "\tJackpot!");
        display_string(1, "\tCongratulation!");
        display_string(2, "\tYou Win");
        display_string(3, "\tPlay again?");
    } else if (current == TimeLimit) {
        display_string(0, "\tSorry.");
        display_string(1, "\tTime Limit exceeded");
        display_string(2, "\tYou Lose");
        display_string(3, "\tPlay again?");
    } else if (current == AttemptsMax) {
		display_image(96, icon);
        display_string(0, "\tBankrupt");
        display_string(1, "\tDie of hunger");
        display_string(2, "\tYou Lose");
        display_string(3, "\tPlay again?");
    } else if (current == EventA1) {
        display_string(0, "\tRandom event");
        display_string(1, "\tStore");
        display_string(2, "\t1 Fraud(3)");
        display_string(3, "\t2 robbery(4)");
    } else if (current == EventA2) {
        display_string(0, "\tRandom event");
        display_string(1, "\tBank-");
        display_string(2, "\t1 salary(2)");
        display_string(3, "\t2 robbery(N) ");
    } else if (current == EventA3) {
        display_string(0, "\tArrested");
        display_string(1, "\tPrison");
        display_string(2, "\tYou Lose");
        display_string(3, "\tPlay again?");
    } else if (current == Rich) {
		allLED();
        display_string(0, "\tCongratulation");
        display_string(1, "\tYou are rich");
        display_string(2, "\tYou Win");
        display_string(3, "\tPlay Again?");
    } else if (current == Work1) {
        display_string(0, "\tWorking.");
        display_string(1, "\tSitework-");
        display_string(2, "\t1 workHard");
        display_string(3, "\t2 pretend");
    } else if (current == Work2) {
        display_string(0, "\tuGetMoney");
        display_string(1, "\t++Money(2)");
        display_string(2, "\t1 buy lottery");
        display_string(3, "\t2 banktoHome");
    } else if (current == Work3) {
        display_string(0, "\tWork so much");
        display_string(1, "\tYou died");
        display_string(2, "\tYou Lose");
        display_string(3, "\tPlay again?");
    } else if (current == Promotion) {
		allLED();
        display_string(0, "\tCongratulation");
        display_string(1, "\tYou get promoted");
        display_string(2, "\tYou Win");
        display_string(3, "\tPlay Again?");
    }
    display_update();
}

void checkSwt(void){
	int swts = getsw();
	if(swts&1){
		checkReset = 1;
	}
	return;
}

//Returns 1 if the button was pressed last cycle but not this
//used in useButtons most of the time
char checkButton(int buttonNumber, int buttons, int * pastButton){
    int p = power(2,buttonNumber - 1);
    if( (p&*pastButton) && ((buttons&p) == 0) )                                                                                                                                                                                                                //if(    (((temp & ~(buttons)) & (pastButton & temp)) == temp)                    && pastButton)
        return 1;
    return 0;
}

void useButtons(const char current, int * pastButton, int firstTime) {
	checkSwt();
    int btns = getbtns();
    if (current == Introduction) {
        if (checkButton(1, btns, pastButton)) {
            checkUpdate = GuessInit;
        }
    } else if (current == MainMenu) {
        if (checkButton(1, btns, pastButton)) {
            if (firstTime == 0) {
                checkUpdate = FirstTimeRun;
                firstTime = 1;
            } else {
                checkUpdate = Athome;
            }
        }
    } else if (current == Athome) {
      if(money<0){
		  if (checkButton(1, btns, pastButton)){checkUpdate = AttemptsMax;}
            checkUpdate = AttemptsMax;
      }
      else if (money > 8) {
		if (checkButton(1, btns, pastButton)){
			checkUpdate = Rich;
		}
        } else
        if (checkButton(1, btns, pastButton)) {
            checkUpdate = Work1;
        } else0
        if (checkButton(2, btns, pastButton)) {
            checkUpdate = GuessInit;
        } else
        if (checkButton(3, btns, pastButton)) {
            health = health - 2;
            if (randomEvents = 0) {
                checkUpdate = EventA1;
                randomEvents++;
            } else {
                checkUpdate = EventA2;
                randomEvents = randomEvents - 1;
            }
        }
    } else if (current == EventA1) {
        if (checkButton(1, btns, pastButton)) {
            if (police <= 2) {
                checkUpdate = Work2;
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
                checkUpdate = EventA3;
            }
        } else if (checkButton(2, btns, pastButton)) {
            if (police <= 2) {
                checkUpdate = Work2;
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
                checkUpdate = EventA3;
            }
        }
    } else if (current == EventA2) {
        if (checkButton(1, btns, pastButton)) {
            checkUpdate = Work2;
            money = money + salary;
			int temp = salary;
			        if (money > 8) {
                    money = 8;
					allLED();
					checkUpdate = Rich;
                }
			while(temp>0){
				addLED();
				temp--;
			}
            salary = 0;
        } else if (checkButton(2, btns, pastButton)) {
            if (police <= 2 && knif != 0) {
                checkUpdate = Rich;
                money = 8;
				    PORTE = 0xff;
            } else {
                checkUpdate = EventA3;
            }
        }
    } else if (current == Work1) {
        if (checkButton(1, btns, pastButton)) {
            health++;
            if (health > 3) {
                checkUpdate = Work3;
            } else {
                salary = salary + 2;
                police = police - 1;
                checkUpdate = Work2;
            }
        } else if (checkButton(2, btns, pastButton)) {
            promo++;
            health = health - 1;
            salary = salary + 2;
            if (promo > 3) {
                checkUpdate = Promotion;
            } else {
                checkUpdate = Work2;
            }
        }
    } else if (current == Work2) {
        if (checkButton(1, btns, pastButton)) {
            checkUpdate = GuessInit;
        } else if (checkButton(2, btns, pastButton)) {
            checkUpdate = Athome;
        }
    } else if (current == FirstTimeRun) {
        if (checkButton(1, btns, pastButton)) {
            checkUpdate = Introduction;
        }
    } else if (current == GuessBig || current == GuessNearBig || current == GuessSmall || current == GuessNearSmall) {
        if (checkButton(1, btns, pastButton)) {
          if(money<0){
          checkUpdate = AttemptsMax;
          }else{checkUpdate = GuessInit;}
            
        }else if (checkButton(2, btns, pastButton)) {
          if(money<0){
          checkUpdate = AttemptsMax;
          }else{checkUpdate = Athome;}
        }
    } else if (current == GuessInit) {
		
        if (money < 0) {
          if (checkButton(1, btns, pastButton)){checkUpdate = AttemptsMax;}
          
            checkUpdate = AttemptsMax;
        } else if (checkButton(1, btns, pastButton)) {
            checkValue = 1;
			      money = money - 1;
			      minusLED();
        } else if (checkButton(4, btns, pastButton)) {
            input += 100;
            checkUpdate = GuessInit;
        } else if (checkButton(3, btns, pastButton)) {
            input += 10;
            checkUpdate = GuessInit;
        } else if (checkButton(2, btns, pastButton)) {
            input += 1;
            checkUpdate = GuessInit;
        }
        if (input > 1000) {
            input = 0;
        }
    } else if (current == Rich || current == Win || current == TimeLimit || current == AttemptsMax || current == EventA3 || current == Work3 || current == Promotion) {
        if (checkButton(1, btns, pastButton)) {
            checkReset = 1;
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
    int i;
    times = 0;
    attempts = 0;
    i = 0;
    int difference;
    int bigger;
    rightAnswer = initialRandom;
    int firstTime;
    int timeMax = 10000;
    int attemptsMax = 10;

    while (1) {
        if (checkValue == 1) {
            if (input == rightAnswer) {
                checkUpdate = Win;
            } else if (times >= timeMax) {
                checkUpdate = TimeLimit;
            } else if (attempts >= attemptsMax) {
                checkUpdate = AttemptsMax;
            } else {
                difference = rightAnswer - input;
                    if(money<0){
            checkUpdate = AttemptsMax;
            }else      
                if (difference < 0) {
                    checkUpdate = GuessBig;
					//addLED();
                }

                else if (difference > 0) {
                    checkUpdate = GuessSmall;
					//addLED();
                }

            }
        }
        if (checkReset) {
            times = 0;
            money = 1;
			PORTE = 0x1;
			led = 1;
            checkValue = 0;
            checkUpdate = 0; 
            checkReset = -1;
          	salary = 0;
            bigger = 0;
            input = 1;
			knif = 0;
			health = 0;
			randomEvents = 0;
			promo = 0;
			police = 0;
            rightAnswer = getRandom((rightAnswer + 1));
        }
        if (checkUpdate != -1) {
            current = checkUpdate;
        }
        changeScreen(current);
        checkValue = 0;
        checkUpdate = 0;
        checkReset = 0;
        checkUpdate = -1;
        useButtons(current, & pastButton, firstTime);
    }
}
