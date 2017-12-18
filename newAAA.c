
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

#define NUMBER_OF_STRINGS 20
#define Enabled 1
#define Disabled 0


signed int update = -1;

char displayList[NUMBER_OF_STRINGS][14]= {"\tstring1", "\tstring2", "\tstring3", "\tstring4", "\tstring5","\tstring6","\tstring7","\tstring8","\tstring9","\tstring10","\tstring11","\tstring12","\tstring13"};



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






signed int current;

#define Command 1
#define Direction 2
#define Direction2 3


#define Mainmenu 0
#define Intro 1
#define Win 2
#define Rank 3

#define NextCommand 1
#define ResetCommand -1

#define ROWS 4
#define COLS 7
#define State 7

int matrix[ROWS][COLS] = {
 {Mainmenu,NextCommand, Intro, Intro,Intro,Intro, -1},
 {Intro,NextCommand, Win, Win, Win, Win -1},
 {Win,NextCommand, Rank, Rank,Rank, Rank,-1},
 {Rank,ResetCommand,ResetCommand,ResetCommand,Intro,Intro,-1}
};

void game(void) {

    int pastButton = 0;
    updateScreen();
    while (1) {

         if(current == ResetCommand) {
           resetData(matrix);
           current = 0;
        }

        if(update == Enabled){
          updateScreen();
          update=Disabled;
        }

        useButtons(&pastButton);
		/*  timeoutcount = 0; // reset timeout value to 0*/

    }
}

void dataReset(int (*arr)[COLS])
{
    int i, j;
    for (i = 0; i < ROWS; i++)
    {
        for (j = 6; j < COLS; j++)
        {
            (*arr)[j]=0;
           // printf("%d\t", (*arr)[j]);
        }
        arr++;
       // printf("\n");
    }
}


void useButtons(int* pastButton) {
    int btns = getbtns();
    int i = 0;
	 while(1)
    {
      i++;
        if(checkButton(i, btns, pastButton)){
        break;
	}
  if(i==4){i=0;}

}
i=i+2;

   if(matrix[current][Command] == NextCommand){
    current=matrix[current][i];
    update = Enabled;
  }else
  if(matrix[current][Command] == ResetCommand){
   current=matrix[current][Direction];
   update = Disabled;
 }
 * pastButton = btns;
}
void updateScreen() {
 int c = 4*current;
	for(int i = 0; i < 3; i++){
		display_string(i,displayList[c+i]);
	}
}
