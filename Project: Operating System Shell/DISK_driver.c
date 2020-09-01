//
//  DISK_driver.c
//  OS_Shell
//
//  Created by Aymen on 4/10/19.
//  Copyright © 2019 Aymen. All rights reserved.
//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct PARTITION
{
    int total_blocks;
    int block_size;
} partition;

struct FAT {
    char *filename;
    int file_length;
    int blockPtrs[10];
    int current_location;
} fat[20];

struct Map {
    int fatIndex;
    FILE *filePointer;
    
}map[5];

char * block_buffer; FILE *fp[5];
char *diskPartition;
char diskBuffer[1000];

void initIO()
{
    
    partition.total_blocks=0;
    partition.block_size=0;
    
    for(int i=0;i<20;i++)
    {
        fat[i].filename=NULL;
        fat[i].file_length=0;
        for(int j=0;j<10;j++)
        {
            fat[i].blockPtrs[j]=-1;
        }
        fat[i].current_location=0;
    
    }
    
}

int exists(const char *fname)
{
    FILE *file;
    if ((file = fopen(fname, "r")))
    {
        fclose(file);
        return 1;
    }
    return 0;
}

int partitions(char *name, int blocksize, int totalblocks)
{
    diskPartition=name;
    char address[100];


    system("mkdir -p PARTITION");
    sprintf(address,"PARTITION/%s",name);
    
    if(exists(address)==1)
    {
        return 1;
    }
    else
    {
        FILE *fp=fopen(address,"w");
        
        if(fp != NULL)
        {
            
            fprintf(fp,"%d ",totalblocks);
            fprintf(fp,"%d\n",blocksize);
            
            for(int i=0;i<20;i++)
            {
                
                fprintf(fp, "%s ",fat[i].filename);
                fprintf(fp, "%d ", fat[i].file_length);
                for(int j=0;j<10;j++)
                {
                    fprintf(fp, "%d ", fat[i].blockPtrs[j]);
                }
                
                fprintf(fp, "%d\n", fat[i].current_location);
                
                
            }
            
            for(int i=0;i<totalblocks;i++)
            {
                for(int j=0;j<blocksize;j++)
                {
                    fprintf(fp, "%d", 0);
                }
                fprintf(fp, "%s", " ");
            }
            
            
            
            
        }
        
        fclose(fp);

    }
    
    
    
    return 1;
}





int mountt(char *name)
{
    char address[100];
    sprintf(address,"PARTITION/%s",name);
    FILE *fp=fopen(address,"r");
    int currChar;
    
    char test[1000];
    
    fscanf (fp, "%d %d", &partition.total_blocks, &partition.block_size);

    //printf("total block is %d\n",partition.total_blocks);
    
    //printf("block size is %d\n",partition.block_size);
   
    
    
    
    for(int i=0;i<20;i++)
    {
        fscanf(fp,"%s",&fat[i].filename);
        fscanf(fp,"%d",&fat[i].file_length);
        
        //printf("file name is %s\n",&fat[i].filename);
        //printf("file length is %d\n",fat[i].file_length);
        
        for(int j=0;j<10;j++)
        {
            fscanf(fp,"%d",&fat[i].blockPtrs[j]);
            //printf("%d\n",fat[i].blockPtrs[j]);
        }
        
        fscanf(fp,"%d",&fat[i].current_location);
        //printf("current location is %d\n",fat[i].current_location);
    }
    
    //strcpy(diskBuffer,"");
    long index=ftell(fp);
    fseek(fp, index+1, SEEK_SET);
    fread(diskBuffer,sizeof(diskBuffer),1,fp);
    //fscanf(fp, "%s",diskBuffer);
    //fgets(diskBuffer, 10000, fp);
    
    //getline(&diskBuffer, sizeof(diskBuffer), fp);

    
    //printf("BUFFER IS: %s\n",diskBuffer);
    
    fseek(fp, 0, SEEK_SET);
    fclose(fp);
    
    
    block_buffer = malloc(sizeof(partition.block_size+1));
    
    return 1;
}


FILE *getBlockPointer(int number, FILE *file)
{
    char line[1000];
    char byte[1];
    char block[partition.block_size+1];
    FILE *fp2 = fdopen (dup (fileno (file)), "r+");
    fseek(fp2, 0, SEEK_SET);
    for(int i=0;i<21;i++)
    {
        fgets(line, 999, fp2);
        
    }
  
    
    for(int j=0;j<number;j++)
    {
       fgets(block,partition.block_size+2,fp2);
    }
    
    return fp2;
}

int getFreeBlock(char *filename)
{
    
    
    char line[1000];
    
    char byte[1];
    int number=-1;
    char block[partition.block_size+1];
    FILE *fp3 = fdopen (dup (fileno (filename)), "r");
    fseek(fp3, 0, SEEK_SET);
    for(int i=0;i<21;i++)
    {
        fgets(line, 999, fp3);
        
    }
    
    
    while(1)
    {
        //printf("HERE\n");
        number++;
 
          
        fgets(block,partition.block_size+2,fp3);
        
        //printf("GETTING BLOCK #%d is  %s\n",number,block);
        //printf("READING BLOCK %c\n",block[0]);
        
        
        if(block[1]=='0')
        {
            break;
        }
        
        
       
    }
    
    fclose(fp3);
    
    
    return number;

}

void updatePartition()
{
    
    char address[100];
    sprintf(address,"PARTITION/%s",diskPartition);
    
    FILE *fp=fopen(address,"w");
    
    if(fp != NULL)
    {
        
        fprintf(fp,"%d ",partition.total_blocks);
        fprintf(fp,"%d\n",partition.block_size);
        
        
        for(int i=0;i<20;i++)
        {
            //printf("file name is %s\n",&fat[i].filename);
            
            if(strcmp("(null)",&fat[i].filename)==1)
            {
                
                fprintf(fp, "%s ",fat[i].filename);
            }
            else
            {
                fprintf(fp, "%s ",&fat[i].filename);
            }
            
            fprintf(fp, "%d ", fat[i].file_length);
            
            for(int j=0;j<10;j++)
            {
                fprintf(fp, "%d ", fat[i].blockPtrs[j]);
            }
            
            fprintf(fp, "%d\n", fat[i].current_location);
            
            
        }
        
        //diskBuffer[strcspn(diskBuffer, "\n")] = "";
        
        //fprintf(fp,"%s",diskBuffer);
        diskBuffer[strcspn(diskBuffer, "\n")] = 0;
        fputs(diskBuffer, fp);
        
        fseek(fp, 0, SEEK_SET);
        
    }
    
    fclose(fp);
    


}

int openfile(char *name, char *filename)
{
    
    char address[100];
    char line[100];
    int index=-1;
    
    sprintf(address,"PARTITION/%s",filename);
    
    FILE *f=fopen(address,"r+");
    

    for(int i=0;i<20;i++)
    {
        //printf("file name is %s\n",&fat[i].filename);
        if(strcmp(name,&fat[i].filename)==0)
        {
            for(int j=0;j<5;j++)
            {
                
                if(map[j].fatIndex == i)
                {
                    //printf("HERE\n");
                    
                    if(fat[i].blockPtrs[0]==-1)
                    {
                        fat[i].blockPtrs[0]=getFreeBlock(f);
                       
                    }
                    else{
                        fat[i].blockPtrs[0]=getFreeBlock(f);
                    }
                    
                    //printf("BLOCK POINTER POINTS TO %d\n",fat[i].blockPtrs[0]);
                    fp[j]=getBlockPointer(fat[i].blockPtrs[0], f);
                    index=i;
                    
                    return index;
                }
                    
                
                
            }
            
       
            
        }

    }
    
    
    
    for(int i=0;i<20;i++)
    {
 
        if(strcmp("(null)",&fat[i].filename)==0)
        {
            index=i;
            strcpy(&fat[i].filename,name);
            fat[i].blockPtrs[0]=getFreeBlock(f);
          
            
          
            updatePartition();
            //printf("UPDATED\n");
            
            for(int j=0;j<5;j++)
            {
                if(fp[j] == NULL)
                {
                    //printf("HERE\n");
                    fp[j]=getBlockPointer(fat[i].blockPtrs[0], f);
                    map[j].fatIndex=i;
                    map[j].filePointer=fp[j];
                    index=i;
                  
                    return index;
                    
                    
                }

            }
            
            
            return i;
        }
        
    }
    
    
    return index;
}
 


int readBlock(int file)
{
   
    for(int i=0;i<5;i++)
    {
       
        if(map[i].fatIndex==file)
        {
            
            long size=ftell(fp[i]);
            //printf("FTELL IS %ld\n", size);
            
            fgets(block_buffer,partition.block_size+1, fp[i]);
            
            fseek(fp[i], size+6, SEEK_SET);
            //printf("Block read is %s\n",block_buffer);
            break;
            
        }
        
    }
    
    return 1;
}

char *returnBlock()
{
    return block_buffer;
}



static void writeFile(int position, char *data, i)
{
    rewind(fp[i]);
    fseek(fp[i], position, SEEK_SET);
    fwrite(data, strlen(data), 1, fp[i]);
    fclose(fp[i]);
  
}


int writeBlock(int file, char *data, char *filename)
{

    char address[100];
    sprintf(address,"PARTITION/%s",filename);
    
    FILE *f=fopen(address,"r+");
    
    for(int i=0;i<5;i++)
    {
        if(map[i].fatIndex==file)
        {
            long size=ftell(fp[i]);
           
            writeFile(size, data,i);
            
            fp[i]=fopen(address,"r+");
            fseek(fp[i], size+partition.block_size+1 , SEEK_SET);
            break;
        }
    }
    
    return 1;
}

