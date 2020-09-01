//
//  shell.c
//  Assignment_1
//
//  Created by Aymen on 1/22/19.
//  Copyright © 2019 Aymen. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include "interpreter.h"
#include "time.h"
#include "shellmemory.h"
#include <stdlib.h>


//declaring parse function

int parseInput(char ui[]);

void UI()
{
    //declaring prompt variables
    
    char prompt[100]={'$','\0'};
    char userInput[1000];
    int errorCode=0;
    
    
    printf("Welcome to the <Aymen Rumi> shell!\nVersion 2.0 Created January 2019\n");
    
    //while loop for prompt, user enters command and shell executes with parse & interpreter
    
    while(1)
    {
     
        printf("%s ",prompt);
        char *check = fgets(userInput, 999,stdin);
        if( check ==NULL)
        {
            exit(0);
        }
        userInput[strcspn(userInput, "\n")] = '\0';
        userInput[strcspn(userInput, "\r")] = '\0';
        errorCode = parseInput(userInput);
        
        //given error code executes different commands by the shell

        errors(errorCode);
        
        if (errorCode==2)
        {

            break;
        }
        

    }
    
    
}

//parse method that calls on interpreter

int parseInput(char ui[])
{
    int count=0;
    char tmp[200]; int a,b;
    char *words[100]; int w=0;
    
    for( a=0; ui[a]==' ' && a<1000; a++);
    
    while(ui[a] != '\0' && a<1000)
    {
        for(b=0; ui[a] !='\0' && ui[a]!=' ' && a<1000; a++,b++)
        {
            tmp[b] = ui[a];
        }
        
        tmp[b]= '\0';
        
        words[w] = strdup(tmp);
        
        count++;
        
      
        
        a++, w++;
        
    }
    
    

    return interpreter(words,count);
}



