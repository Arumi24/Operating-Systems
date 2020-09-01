//
//  DISK_driver.h
//  OS_Shell
//
//  Created by Aymen on 4/10/19.
//  Copyright © 2019 Aymen. All rights reserved.
//



#include <stdio.h>

void initIO();

int partitions(char *name, int blocksize, int totalblocks);


int mountt(char *name);


int openfile(char *name, char *filename);



int readBlock(int file);


//int writeBlock(int file, char *data,char *filename);
int writeBlock(int file, char *data, char *filename);


char *returnBlock();




extern struct PARTITION
{
    int total_blocks;
    int block_size;
} partition;


extern struct FAT {
    char *filename;
    int file_length;
    int blockPtrs[10];
    int current_location;
} fat[20];


extern struct Map {
    int fatIndex;
    FILE *filePointer;
    
}map[5];

extern FILE *fp[5];





