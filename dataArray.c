
#define Screen 3
#define State 9
#define NUMBER_OF_STRINGS = 20 
 
signed int current; 

 
int main ()
{
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
                             
}
 
void resetData(int (*arr)[State])
{
    int i, j;
    for (i = 0; i < ROWS; i++)
    {
        for (j = 4; j < COLS; j++)
        {
            (*arr)[j] = 0;
        }
        arr++;
    }
}
 
