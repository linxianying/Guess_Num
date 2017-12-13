
#define Screen 4
#define State 9
#define NUMBER_OF_STRINGS = 20 
 
signed int current; 

    int gameData[Screen][State] = {{1, 1, 1, 0,0,0,0, 0,0},//command,dir1,dir2,btn1234,swt12, 
                              {1, 2, 2, 0,0,0,0, 0,0},
                              {1, 3, 3, 0,0,0,0, 0,0},
                              {1, 5, 6, 0,0,0,0, 0,0}};
    
    // command  = 0 = swt1 = enable, command = 0 =reset
    // command1 = 1 = anybtn pressed, update current screen to dir 1
    // command2 = 2 = btn1 input = input+100, btn2 input+10, btn3 input+1, btn4 command = 3
    // command3 = 3 = 
    char displayList[NUMBER_OF_STRINGS][14];
    strcpy(strs[0], "welcome");
    strcpy(strs[1], "display1");
    strcpy(strs[2], "display2");
    strcpy(strs[3], "display3"); 
 
void resetData(int (*gameData[State])
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

void useButtons(int * pastButton) {
    int btns = getbtns2();
        if (btns==1){
            gameData[current][4] = 1;
        }else 
        if(btn==2){
            gameData[current][5] = 1;
        }else 
        if(btn==3){
            gameData[current][6] = 1;
        }else
        if(btn==3){
            gameData[current][7] = 1;} 
 
 * pastButton = btns;
}

void changeScreen() { 
		      for (i = 0; i < 3; i++){
        display_string(i,displayList[4*current][i]);
        }
    } 
 
void game(void) {
    current = 0;
    int pastButton = 0;
    while (1) {
        
        if (dataArray[current][1] != 0) {
           current = ;
        }
    
        changeScreen(current);
        resetData(current);
        useButtons(& pastButton);
    }
}
