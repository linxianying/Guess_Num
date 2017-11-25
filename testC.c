#include <stdio.h>
#include <stdlib.h>
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

void listReset(int * array){
    gameover = 0;
	/* random int between 1 and 100 */
    r = rand() % 101;
    r++;
    attempt = 0;
    upper=100;
    lower=0;
	int i;
	int length = 50;
	arr = * array;
	for(i = 0; i < length; i++){
		array[i] = 0;
	}
	array[r] = 1;
}

void getBntC(int n){
   if( n = 4 ){
       inputValue += 1;
   }else
   if( n = 3 ){
       inputValue += 10;
   }else
   if( n = 2 ){
       inputValue += 100;
   }else
   if( n = 1 ){
       inputValue += 1000;
   }
   
   if(inputValue > 9999){
       inputValue = 0;
   }
   
}

void getSwtC(int n){
    if( n = 1 ){
       compareInput();
       attempt ++;
   }else
   if( n = 2 && attempt!=0){
       listReset(arr);
   }
    
}

void compareInput(){
    if (inputValue = r){
        gameover = 1;
        
    }else{
        attempt ++;
        if (inputValue<upper&&inputValue>r){
            upper=inputValue;
        }if(inputValue>lower&&inputValue<r){
            lower=inputValue;
        }
    }
}







