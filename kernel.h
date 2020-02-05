//
//  kernel.h
//  OS_Shell
//
//  Created by Aymen on 2/18/19.
//  Copyright © 2019 Aymen. All rights reserved.
//



#include <stdio.h>

extern FILE *ram[10];

struct node
{
    struct PCB *data;
    struct node* next;
    
};

extern struct node *head, *tail;

void myInit(FILE *p);

void scheduler();

void addToReady(struct PCB *pcb);

void RoundRobin();

void deleteHead();

void scheduler();

void freeData();
