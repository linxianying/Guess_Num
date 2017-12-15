
/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

#define TMR2PERIOD ((80000000 / 256) / 10)
#if TMR2PERIOD > 0xffff
#error "TMR2PERIOD is too big"
#endif

int mytime = 0x5957;
int timeoutcount = 0;
#define Screen 4
#define State 9
#define NUMBER_OF_STRINGS 20
#define Enable 1
#define Disable 0
#define Command 0

char displayList[NUMBER_OF_STRINGS][14]= {"string1", "string2", "string3", "string4", "string5","string6","string7","string8","string9","string10","string11","string12","string13"};



    // command  = 0 = swt1 = enable, command = 0 =reset
    // command1 = 1 = anybtn pressed, update current screen to dir 1
    // command2 = 2 = btn1 input = input+100, btn2 input+10, btn3 input+1, btn4 command = 3
    // command3 = 3 =


 void labinit( void )
{
  // initialize Port E so that bits 7 through 0 of Port E are set as outputs(0)
  // Register TRISE has address 0xbf886100
  //volatile int * trise = (volatile int *) 0xbf886100;
  // least 8 bits are 0
  // initialize port D so that bits 11 through 5 of Port D are set as inputs(1)
  TRISD = TRISD|0x0ff0; // 1 for input
  TRISE = TRISE&~0xff;

  T2CON = 0x70; // 0111 000 is for 1:256
  PR2 = TMR2PERIOD; // for a timeout value of 100 ms
  TMR2 = 0; // reset the counter
  T2CONSET = 0x8000; // start the timer
  return;
}

void resetData(int (*gameData[State]))
{
    int i, j;
    for (i = 0; i < Screen; i++)
    {
        for (j = 4; j < State; j++)
        {
            (*gameData)[j] = 0;
        }
        gameData++;
    }
}

void useButtons(int* current, int * pastButton, int (*gameData[State])) {
    int btns = getbtns();
    int currentP = &current;

	for (i = 1; i < 5; i++)
    {
        if(checkButton(i, btns, pastButton)){
        checkCommand(currentP,i,GameData)
        break;
	}
}
 * pastButton = btns;
}

void changeScreen(int* current) {
	int i;
  char (*ptr1)[14] = displayList;
	for ( i = 0; i < 3; i++){
		display_string(i,*ptr1[4*current][i]);
	}
}

void checkCommand(int* current, int buttonNumber, int (*gameData[State])){
  if(gameData[current][Command]== 1 && buttonNumber!=0){
    current=gameData[current][1];
  }
}

void game(void) {
   int current = 0;
   int *currentP;        /* pointer variable declaration */
   int gameData[Screen][State] = {
     //command,dir1,dir2,btn1234,swt12,
     {1, 1, 1, 0,0,0,0, 0,0},//0=MenuScreen,command1,direction=MenuScreen2,
     {1, 2, 2, 0,0,0,0, 0,0},//1=MenuScreen2,command1,direction=testScreen,
     {1, 1, 1, 0,0,0,0, 0,0}//2=testScreen,comman1,direction=MenuScreen,

   };
   currentP = &current;

    int pastButton = 0;
    while (True) {
    /*    if(IFS(0) & 0x100){
		  IFS(0) = 0;	//Reset all event flags (crude!)
		  // without this, the speed will become extremely fast
		  timeoutcount++;
		}*/
        if (dataArray[current][1] != 0) {
           current = ;
        }

	if(timeoutcount == 10){
          changeScreen(current);
          resetData(current);
          useButtons(current, &pastButton,gameData);
		/*  timeoutcount = 0; // reset timeout value to 0*/
		}
    }
}
