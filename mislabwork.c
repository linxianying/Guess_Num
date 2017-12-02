#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <stdlib.h>
#include <stdio.h>
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

const unsigned int 
					menuScreen = 0,  
					gameScreen = 1, 
					winScreen = 2,
					wrongScreen = 3,
					loseScreen = 4,
					timeOutScreen =5;
int timeoutcount = 0;
int inputValue;
char currentScreen;
int upper = 100;
int lower = 0;
/* Interrupt Service Routine */
void user_isr( void )
{
  return;
}

int timertest()
{
	if (IFS(0)&0x100) // time-out event
	{
		IFS(0)=0; 
		return (1);
	}
	else
	{
		return (0);
	}
}
/* Lab-specific initialization goes here */
void labinit( void )
{
  // initialize Port E so that bits 7 through 0 of Port E are set as outputs(0)
  // Register TRISE has address 0xbf886100
  volatile int * trise = (volatile int *) 0xbf886100;
  // least 8 bits are 0
  * trise = * trise & 0xff00;
  //initialize port D so that bits 11 through 5 of Port D are set as inputs(1)
  TRISD = TRISD & 0x0fe0;
  timeoutcount = 0;
  T2CON = 0x70;
  T2CONSET = 0x8000;
  PR2 = (80000000/256)/10;
  return;
}

/* This function is called repetitively from the main program */
void labwork( void )
{
  volatile int * porte = (volatile int *) 0xbf886110;
  * porte = 0x0; // for LED outputs
  currentScreen = 0;
  //gameInitial();
  int btns;
  int swt;
  int value = 0;
  if(currentScreen == menuScreen){
	if(btns!=0||swt!=0){
		currentScreen = gameScreen;
		//updateScreen();
		//开始画面 随机键开始游戏
	}    
    if(currentScreen == gameScreen){
		delay( 1000 ); 
		btns = getbtns();
		swt = getsw();
		
		//for button 2
		while(swt==0){
			if( btns & 8){
			  value += 1;
			}else
			if( btns & 4 ){
				value += 10;
			}else
			if( btns & 2 ){
				value += 100;
			}else
			if( btns & 1 ){
				value += 1000;
			}
			if(value > 9999){
			   value = 0;
			}	
			display_update();
		}
	}else
    if(currentScreen == gameScreen){
        if(swt!=0){
			inputValue = value;
		}
	   //确认数据。
    }else
    if(currentScreen == wrongScreen){
        if(swt!=0){
	    //继续游戏
            currentScreen = gameScreen;
            //updateScreen();
        }else
           //继续游戏
            if(currentScreen == winScreen){
				if(swt!=0){
					currentScreen = menuScreen;
					//updateScreen();
				}
       
			}
    }
    display_image(96, icon);
  }
}
void updateScreen(void){
	if(currentScreen == menuScreen){
        //开始画面。
		display_string(0,"\t");
        display_string(1,"\tTo Start ");
        display_string(2,"\tPress Any Key");
        display_string(3,"\t");
    }else 
    if(currentScreen == winScreen){   
        //游戏结束画面。
		display_string(0,"\t");
        display_string(1,"\tRight Answer!");
        display_string(2,"\tCongratulation!");
        display_string(3,"\tYou Win.");
    }else 
    if(currentScreen == loseScreen){   
        //游戏结束画面。
		display_string(0,"\t");
        display_string(1,"\tNo Chance left!");
        display_string(2,"\tSorry!");
        display_string(3,"\tYou Lose.");
    }else 
    if(currentScreen == gameScreen){
		//猜数字界面。
        display_string(0,"\tGuess a number");
        display_string(1,"\tYour guess: ");
        display_string(2,"     ");
        display_string(3,"\t");
    }

    display_update();
    return;
}

void gameInitial(void){
	//char currentScreen;    
	//int inputValue;//输入的数值
	int attempt;//尝试次数
	int gameover;
	int answer;//随机数
	//int upper=100;//上限
	//int lower=0;//下限
	int chance=10;
	int currentRank=0;
	int arr[10];

	
	gameover = 0;
	/* random int between 1 and 100 */
	answer = 66;
	attempt = 0;
	upper=100;
	lower=0;
	int i=0;
	while(i<10){
		arr[i] = 11;
	}
	//updateScreen();	 
	return;
}
