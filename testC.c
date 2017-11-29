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

int main()
{
    printf("%d\n", rand() % 50);
    return 0;
}

int inputValue;//输入的数值
int attempt;//尝试次数
int arr;//array
int gameover;
int r;//随机数
int upper=100;//上限
int lower=0;//下限
int timeout=10;
int currentRank=0;
int[] rank = new int[10] {11, 11, 11, 11, 11, 11, 11, 11, 11, 11}; //only tell when the player is top 10
const unsigned int menuScreen = 0, 
    		   gameScreen = 1, 
    		   winScreen = 2,
    		   wrongScreen = 3,
    		   guessScreen = 4,
		   timeOutScreen =5,
		   rankScreen=6;
    
int currentScreen;    
 
//游戏初始化
 void gameMain(void){   
	 
    currentScreen = 0;
    listReset();
    updateScreen();	 
	 
    
}
   
//更新画面
void updateScreen(){
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
    if(currentScreen == wrongScreen){
	 //错误画面。显示上限和下限。   
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
	//猜数字界面。
        char buffer2[20];
        itoa(InputValue,buffer,10);
        display_string(0,"\tguess a number");
        string stringInput = strcat("\t",buffer); 
        display_string(1,"\tYour guess: ");
        display_string(2,stringInput);
        display_string(3,"\t");
    
    }else
    if(currentScreen == timeOutScreen){
	//时间超时界面。
        char buffer2[20];
        itoa(InputValue,buffer,10);
        display_string(0,"\tTime Out!");
        string stringInput = strcat("\t",buffer); 
        display_string(1,"\tYour guess: ");
        display_string(2,"none";
        display_string(3,"\t");
    }else  
    if(currentScreen == rankScreen){
	//排名。
        char buffer2[20];
        itoa(InputValue,buffer,10);
        display_string(0,"\tRank List:");
        string stringInput = strcat("\t",buffer); 
        display_string(1,"\tYour rank: ");
        display_string(2,currentRank);
        display_string(3,"\t");
    
    }else
    display_update();
}
    
void computeRank(){
    if(attempt<arr[9]){
    	for(int i=9;i>=1;i--){
	    if(attempt<arr[i]&&attempt>arr[i-1]){
	        currentRank = i + 1;
		for(int j=9;j>=i;j--){
		    arr[j] = arr[j-1];
		}
		arr[i] = attempt;
	    }
	    if(attempt<arr[0]){
	        currentRank = 1;
		for(int j=9;j>=1;j--){
		    arr[j] = arr[j-1];
		}
		arr[0] = attempt;
	    }
	}
    }
}

void listReset(){
    //读作init,初始化数据。
	currentScreen = 0;
    gameover = 0;
	/* random int between 1 and 100 */
    r = rand() % 101;
    r++;
    attempt = 0;
    upper=100;
    lower=0;
}

void updateLeastPressed(){
    //最晚摁下的那个键为1，其他的键为0，可以用forloop来写	
    //Least pressed button = 1, others = 0
}

void getBntC(int n){
    int btns = getbtns();
    int swt  = getsw();
    updateLeastPressed();
    //更新键位数据。
	
	
    if(currentScreen == menuScreen){
        if(btns!=0){
            currentScreen = guessScreen;
            updateScreen();
		//开始画面 随机键开始游戏
        }else
        
    if(currentScreen == guessScreen){
        //inputValue更改
    if(btns & 8){
      inputValue += 1;
   }else
   if( btns & 4 ){
       inputValue += 10;
   }else
   if( btns & 2 ){
       inputValue += 100;
   }else
   if( btns & 1 ){
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
	   //确认数据。
   }else
   
   if(currentScreen == wrongScreen){
       if(swt!=0){
	    //继续游戏
            currentScreen = guessScreen;
            updateScreen();
        }else
           //继续游戏
           if(currentScreen == winScreen){
       if(swt!=0){
            currentScreen = menuScreen;
            updateScreen();
        }
       
   }
   
   
   
   
}

void getSwtC(int n){
	
    //swt1进行比较，swt2重置数据。
    if( n = 1 ){
       compareInput();
       attempt ++;
   }else
   if( n = 2 && attempt!=0){
       listReset();
   }
    
}

//比较	    
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
