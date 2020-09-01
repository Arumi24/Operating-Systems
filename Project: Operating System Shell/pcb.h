//
//  pcb.h
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

