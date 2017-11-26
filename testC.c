/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */
#include "sidefuncs.h"

int main()
{
    printf("%d\n", rand() % 50);
    return 0;
}

int inputValue;
int attempt;
int arr;
int gameover;
int r;
int upper=100;
int lower=0;
int menuScreen = 0, 
    gameScreen = 1, 
    winScreen = 2,
    wrongScreen = 3,
    guessScreen = 4;
    
int currentScreen;    
    
 void gameMain(void){   
    int currentScreen = 0;

    listReset();
    
}
   
void updateScreen(){
    if(currentScreen == menuScreen){
        display_string(0,"\t");
        display_string(1,"\tTo Start ");
        display_string(2,"\tPress Any Key");
        display_string(3,"\t");
    }else 
    
    if(currentScreen == winScreen){   
        display_string(0,"\t");
        display_string(1,"\tRight Answer!");
        display_string(2,"\tCongratulation!");
        display_string(3,"\tYou Win.");
    }else 
    if(currentScreen == wrongScreen){
        char buffer[20];
        itoa(upper,buffer,10);
        
        string stringUpper = strcat("\tupper: ", buffer); 
        itoa(lower,buffer,10);
        string stringLower = strcat("\tlower: ", buffer); 
        
        display_string(0,"\tWrong Answer!");
        display_string(1,stringUpper);
        display_string(2,stringLower);
        display_string(3,"\tTry Again.");

    }else 
    if(currentScreen == guessScreen){
        char buffer2[20];
        itoa(InputValue,buffer,10);
        display_string(0,"\tguess a number");
        string stringInput = strcat("\t",buffer); 
        display_string(1,"\tYour guess: ");
        display_string(2,stringInput);
        display_string(3,"\t");
    
    }

    display_update();
}
    
    

void listReset(){
    gameover = 0;
	/* random int between 1 and 100 */
    r = rand() % 101;
    r++;
    attempt = 0;
    upper=100;
    lower=0;
}

void updateLeastPressed(){
    //Least pressed button = 1, others = 0
}

void getBntC(int n){
    int btns = getbtns();
    int swt  = getsw();
    updateLeastPressed();
    
    if(currentScreen == menuScreen){
        if(btns!=0){
            currentScreen = guessScreen;
            updateScreen();
        }else
        
    if(currentScreen == guessScreen){
        
    if( btns = 4 ){
      inputValue += 1;
   }else
   if( btns = 3 ){
       inputValue += 10;
   }else
   if( btns = 2 ){
       inputValue += 100;
   }else
   if( btns = 1 ){
       inputValue += 1000;
   }
   
   if(inputValue > 9999){
       inputValue = 0;
   }
            updateScreen()´;
        }else
   
   if(currentScreen == guessScreen){
       if(swt=1){
     getSwtC(swt);}
   }else
   
   if(currentScreen == wrongScreen){
       if(swt!=0){
            currentScreen = guessScreen;
            updateScreen();
        }else
           
           if(currentScreen == winScreen){
       if(swt!=0){
            currentScreen = menuScreen;
            updateScreen();
        }
       
   }
   
   
   
   
}

void getSwtC(int n){
    if( n = 1 ){
       compareInput();
       attempt ++;
   }else
   if( n = 2 && attempt!=0){
       listReset();
   }
    
}

void compareInput(){
    if (inputValue = r){
        currentScreen = winScreen;
        gameover = 1;
        updateScreen();
        
    }else{
        attempt ++;
        if (inputValue<upper&&inputValue>r){
            upper=inputValue;
        }if(inputValue>lower&&inputValue<r){
            lower=inputValue;
        }
        updateScreen();
    }
}
