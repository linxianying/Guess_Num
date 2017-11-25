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

#define TRUE 1
#define FALSE 0
volatile int * porte = (volatile int *) 0xbf886110;
volatile int * trise = (volatile int *) 0xbf886100;

/* Interrupt Service Routine */
void user_isr( void )
{
  return;
}

/* Lab-specific initialization goes here */
void labinit( void )
{
  // initialize Port E so that bits 7 through 0 of Port E are set as outputs(0)
  // Register TRISE has address 0xbf886100
  // least 8 bits are 0
  * trise = * trise & 0xff00;
  TRISD = TRISD & 0x0fe0;
  
  timeoutcount = 0;
  T2CON = 0x70;
  T2CONSET = 0x8000;
  PR2 = (80000000 / 256 ) / 10;
  return;
  return;
}
void led_display(char bin_time){
    (*porte) = bin_time;
}

/* This function is called repetitively from the main program */
void labwork( void )
{
  
  * porte = 0x0; // for LED outputs
  int btns;
  int sw;
  int guess = 0;
  while(1) { 
    delay( 1000 ); 
    btns = getbtns();
    sw = getsw();
    
    //for button 2
    if(btns & 1 ){
      guess = guess + 1000;
    }
    // for button 3
    if(btns & 2 ){
      guess = guess + 100;
    }
    // for button 4
    if(btns & 4){
      guess = guess + 10;
    }
	if(btns & 8){
      guess = guess + 1;
    }
    
    //time2string( textstring, mytime );
    //display_string( 3, textstring );
    //display_update();
    //tick( &mytime );
    //display_image(96, icon);
    // for LEDs ticking
    //* porte =  * porte - 0x1;
  }
}

void updateScreen(const unsigned int currentScreen, const int guess_nr, int * current_guess, const int symbol_index, int * rightAnswer, int * past_guess, const int past_guess_nr){
    if(currentScreen == START_SCREEN){
        display_string(0,"\t");
        display_string(1,"\tWELCOME TO");
        display_string(2,"\tGUESS NUM");
        display_string(3,"\t");
    }else if(currentScreen == GUESS_SCREEN){   
        char guess [16] = "\tGUESS: ";
        concatenate(guess, 16, guess_nr); 
        display_string(0, guess); 
        char input [16] = "\t";
        createString(input, symbol_index, current_guess); 
        display_string(1, input);
        display_string(2,"\t");
        display_string(3,"\t");
    }else if(currentScreen == LOSE_SCREEN){
        display_string(0,"\t\tGAME OVER");
        display_string(1,"");
        display_string(2,"\tRIGHT ANSWER:");
        char input [16] = "\t";
        createSimpleString(rightAnswer, input);
        display_string(3,input);
    }else if(currentScreen == WRONG_SCREEN){
        display_string(0,"\tWRONG");
        char input[16] = "\tYOU HAVE ";
        concatenate(input,16,MAX_GUESSES -guess_nr);
        display_string(1,input);
        if(MAX_GUESSES - guess_nr == 1){
            display_string(2,"\tGUESS LARGER");
        }else{
            display_string(2,"\tGUESS SMALLER");
        }
        display_string(3,"");
    }else if(currentScreen == WIN_SCREEN){
        display_string(0,"\t!!!YOU WON!!!");
        display_string(1,"\tYOUR ANSWER:");
        char input [16] = "\t";
        createSimpleString(rightAnswer, input);
        display_string(2,input);
        display_string(3,"\tPLAY AGAIN?");
    }else if(currentScreen == PAST_GUESS){
        char guess [16] = "\tGUESS NR: ";
        concatenate(guess, 16, past_guess_nr); 
        display_string(0, guess); 
        char input [16] = "\t";
        createSimpleString(past_guess,input); 
        display_string(1, input);
        display_string(2,"\t");
        display_string(3,"\t");
    }else if(currentScreen == INVALID_SCREEN){
        display_string(0,"OBS! INVALID");
        display_string(1,"SWITCH 4");
        display_string(2,"TO RESET");
        concatenate(input,16, guess_nr);
        display_string(3,input);
    }

    display_update();
}