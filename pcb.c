//
//  pcb.c
//  OS_Shell
//
//  Created by Aymen on 2/18/19.
//  Copyright © 2019 Aymen. All rights reserved.
//




#include <stdio.h>


struct PCB
{
    FILE *PC;
    int pageTable[10];
    int PC_page, PC_offset, pages_max;
    char filePath[1000];
};



struct PCB* makePCB(FILE *p);

struct PCB* makePCB(FILE *p)
{
    
    struct PCB *ptr= (struct PCB *)malloc(sizeof(struct PCB));;
    ptr->PC=p;
    for(int i=0;i<10;i++)
    {
        ptr->pageTable[i]=-1;
    }
  
    ptr->pages_max=0;
    ptr->PC_offset=0;
    ptr->PC_offset=0;;
    
    
    return ptr;
    
}


