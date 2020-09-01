//
//  memorymanager.h
//  OS_Shell
//
//  Created by Aymen on 3/20/19.
//  Copyright © 2019 Aymen. All rights reserved.
//


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libgen.h>
#include "pcb.h"



int countTotalPages(FILE *f);
FILE *findPage(int pageNumber, FILE *f);
int findFrame(FILE *page);
int findVictim(struct PCB *p);
int updateFrame(int frameNumber, int victimFrame, FILE *page);
int updatePageTable(struct PCB *p, int pageNumber,int frameNumber, int victimFrame);
