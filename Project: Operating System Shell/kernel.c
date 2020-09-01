//
//  kernel.c
//  OS_Shell
//
//  Created by Aymen on 2/18/19.
//  Copyright © 2019 Aymen. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include "interpreter.h"
#include "shellmemory.h"
#include "memorymanager.h"
#include <stdlib.h>
#include "cpu.h"
#include "shell.h"
#include "ram.h"
#include <stdio.h>
#include <stdlib.h>


FILE *ram[10];

typedef struct node
{
    struct PCB *data;
    struct node* next;
    
} node;

struct node *head, *tail;


void myInit(FILE *p);

void scheduler();

void addToReady(struct PCB *pcb);

void backOfReady();

void boot();






void boot()
{
    
    for(int i=0;i<10;i++)
    {
        ram[i]=NULL;
        
    }
    
    
    system("if [ -d BackingStore ]; then rm -r BackingStore; mkdir BackingStore; else mkdir BackingStore; fi");
    

        
}



void addToReady(struct PCB *pcb)
{
    struct node *linkedlist =(node*) malloc(sizeof(linkedlist));
    
    linkedlist->data=pcb;
    linkedlist->next=NULL;
    if(head==NULL)
    {
        head=linkedlist;
        
    }
    else
    {
        tail->next=linkedlist;
        
    }
    

    tail=linkedlist;

}


void myInit(FILE *p)
{
    addToRAM(p);
    struct PCB *ptr=makePCB(p);
    addToReady(ptr);
  
}


void RoundRobin()
{
   
    if(head==NULL)
    {
        return;
    }
    
    if(head->next==NULL)
    {
        
    }
    else
    {
        struct node *temp=head;
        head=head->next;
        tail->next=temp;
        tail=temp;
        tail->next=NULL;

    }
   
}

void deleteHead()
{
    struct node *temp=head;
    deleteFromRam(temp->data->PC);
    head=temp->next;
    
    free(temp);
    
}

void pageFault(struct node *head)
{
   
    if(head->data->pageTable[head->data->PC_page]!=NULL)
    {
        head->data->PC=ram[head->data->PC_page];
        head->data->PC_offset=0;
    }
    char path[1000];
    strcpy(path,head->data->filePath);
    FILE *p=fopen(path, "r");
    FILE *fp=findPage(head->data->PC_page, p);
    int frame=findFrame(fp);
    //printf("frame is %d\n",frame);
    int victim=findVictim(head->data);
    //printf("victim is %d\n",victim);
    updateFrame(frame, victim, fp);
    updatePageTable(head->data, head->data->PC_page, frame, victim);
    
    /*
    for(int i=0;i<10;i++)
    {
        printf("%d, ", head->data->pageTable[i]);
    }
     */
    
    
}


void freeData()
{

 
    while(head != NULL){

        deleteHead();
	
    }

    
    for(int i=0;i<10;i++)
    {
	if(ram[i]!=NULL)
	   {
		ram[i]=NULL;
	   }

    }

   
    
}




void scheduler()
{
    
    
    int x=1;
    
    
    while(x!=0)
    {
        if(head==NULL)
        {
            break;
        }
        
        if(ram[head->data->pageTable[head->data->PC_page]]==NULL)
        {
            //printf("PAGE FAULT !!!\n");
            pageFault(head);
        }
        
        //printf("\n");
        //printf("we are in page %d in frame %d\n",head->data->PC_page,head->data->pageTable[head->data->PC_page]);
        cpu.IP=ram[head->data->pageTable[head->data->PC_page]];
        
        //printf("\n");
        //printf("we are running 2 quantas of %s from page %d and offset %d\n",head->data->filePath,head->data->PC_page,head->data->PC_offset);
        run_cpu(2);
        
        RoundRobin();
        
        
    }
    
}



int main(int argc, const char * argv[])
{
    printf("Kernel 1.0 loaded!\n");
    boot();
    UI();
}


