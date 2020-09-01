//
//  cpu.c
//  OS_Shell
//
//  Created by Aymen on 2/18/19.
//  Copyright © 2019 Aymen. All rights reserved.
//


#include <stdio.h>
#include "shell.h"
#include "interpreter.h"
#include "kernel.h"
#include "pcb.h"

struct CPU
{
    FILE *IP;
    char IR[1000];
    int quanta;
    int offset;
};



struct CPU cpu;


void run_cpu(int quanta);



void run_cpu(int quanta)
{
    cpu.quanta=quanta;
    
    int errCode;
    for(int i=0;i<cpu.quanta;i++)
    {


        char* line = fgets(cpu.IR,999,(cpu.IP));
        
        
        
        if(line == NULL )
        {
            
            deleteHead();
            break;
        }

        line[strcspn(line, "\n")] = '\0';
        line[strcspn(line, "\r")] = '\0';
        
        //printf("line is is: %s\n",line);
        
        if(strlen(line) == 0 )
        {
            
            deleteHead();
            
            break;
        }
        
        
        
        errCode=parseInput(cpu.IR);
        
        
        if(errCode==2)
        {
            freeData();
            exit(0);
        }
        
        head->data->PC_offset++;

        if(head->data->PC_offset==4)
        {

            
            head->data->PC_page++;
            //printf("we have moved to page %d for %s\n",head->data->PC_page,head->data->filePath);
            //printf("\n");
            head->data->PC_offset=0;

        }
        
        errors(errCode);
        
    }
}


