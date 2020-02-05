//
//  cpu.h
//  OS_Shell
//
//  Created by Aymen on 2/18/19.
//  Copyright © 2019 Aymen. All rights reserved.
//




#include <stdio.h>

struct CPU
{
    FILE *IP;
    char IR[1000];
    int quanta;
    int offset;
};

extern struct CPU cpu;

void run_cpu(int quanta);

void pageFault();

