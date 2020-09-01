//
//  ram.c
//  OS_Shell
//
//  Created by Aymen on 2/18/19.
//  Copyright © 2019 Aymen. All rights reserved.
//



#include <stdio.h>
#include "kernel.h"






void addToRAM(FILE *p)
{

  

    for(int i=0;i<10;i++)
    {
        
        if(ram[i]==NULL)
        {
            ram[i]=p;
            break;

        }
        
    }
    
}

void deleteFromRam(FILE *p)
{
    for(int i=0;i<10;i++)
    {
        
        if(ram[i]==p)
        {
            ram[i]=NULL;
            break;
            
        }
        
    }
    
}

