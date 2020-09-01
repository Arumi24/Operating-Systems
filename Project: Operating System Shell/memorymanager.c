//
//  memorymanager.c
//  OS_Shell
//
//  Created by Aymen on 3/20/19.
//  Copyright © 2019 Aymen. All rights reserved.
//


#include "memorymanager.h"
#include "kernel.h"
#include "ram.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int launcher(FILE*p)
{
    struct PCB *ptr=makePCB(p);
 
    int frame;
    int victim;
    int iteration;
    
    if(countTotalPages(p)==1)
    {
        iteration=1;
    }
    else
    {
        iteration=2;
    }
    
    srand(time(0));
    for(int i=0;i<iteration;i++)
    {
        
        FILE *fp=findPage(i, ptr->PC);
        frame=findFrame(fp);
        printf("frame is %d\n",frame);
        victim=findVictim(ptr);
        printf("victim is %d\n",victim);
        updateFrame(frame, victim, fp);
        updatePageTable(ptr, i, frame, victim);
        ptr->pages_max=countTotalPages(ptr->PC);
        

    }
    
    ptr->pages_max=countTotalPages(ptr->PC);
    
    for(int i=0;i<10;i++)
    {
        printf("%d, ", ptr->pageTable[i]);
    }
     
    
     
    addToReady(ptr);
    rewind(ptr->PC);
  
    return 1;
}

int countTotalPages(FILE *f)
{
    int count=0;
    char line[256];
    while(fgets(line,sizeof(line),f))
    {
        count++;
    }
    
    return ceil(count/4.0);
 
}

FILE *findPage(int pageNumber, FILE *f)
{
    
    char line[1000];
    
    FILE *fp2 = fdopen (dup (fileno (f)), "r");
    fseek(fp2, 0, SEEK_SET);
       
    for(int i=0;i<(4*pageNumber);i++)
    {
        fgets(line, 999, fp2);
    }

    return fp2;
}

int findFrame(FILE *page)
{
    for(int i=0;i<10;i++)
    {
        if(ram[i]==NULL)
        {
            return i;
        }
    }
    
    return -1;
}

int findVictim(struct PCB *p)
{
    int belongs=-1;
    
    int r = random()%10;
    
 
    
    while(1)
    {
        belongs=-1;
        for(int i=0;i<10;i++)
        {
           if(p->pageTable[i]==r)
           {
               belongs=1;
             
           }
        }
        if(belongs==-1)
        {
            return r;
        }
        else
        {
            
            r = (r + 1) % 10;

        }
        
        
    }

  
}

int updateFrame(int frameNumber, int victimFrame, FILE *page)
{
    if(frameNumber==-1)
    {
        ram[victimFrame]=page;
        return victimFrame;
    }
    else
    {
        ram[frameNumber]=page;
        return frameNumber;
    }
}

int updatePageTable(struct PCB *p, int pageNumber,int frameNumber, int victimFrame)
{
    if(frameNumber==-1)
    {
        p->pageTable[pageNumber]=victimFrame;
    }
    else
    {
        p->pageTable[pageNumber]=frameNumber;
    }
    
    return p->pageTable[pageNumber];
}






