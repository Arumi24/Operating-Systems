//
//  interpreter.c
//  Assignment_1
//
//  Created by Aymen on 1/22/19.
//  Copyright © 2019 Aymen. All rights reserved.
//


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libgen.h>
#include "shell.h"
#include <time.h>
#include <math.h>
#include "shellmemory.h"
#include "memorymanager.h"
#include "ram.h"
#include "kernel.h"
#include "cpu.h"
#include "DISK_driver.h"
#include "ioscheduler.h"


//declaring command functions

int help(char *words[]);
int quit(char *words[]);
int set(char *words[]);
int print(char *words[]);
int script(char *words);
int run(char *words[]);
int exec(char *words[],int count);
void errors(int errCode);

char* partitionName;




//interpreter function that uses parsed command to run specific commands

int interpreter(char *words[],int count){
    
 
    
    int errCode=0;
    
    
    if(words[0]==NULL || count==0)
    {
        
        return 4;
    }
    
    if(!strcmp(words[0],"help"))
    {
      
        if(count>1)
        {
            return 6;
        }
        
        errCode= help(words);
        
    }
    else if(!strcmp(words[0],"quit"))
    {
        if(count>1)
        {
            return 6;
        }
        
        errCode= quit(words);
    }
    else if (!strcmp(words[0],"set"))
    {
        if(count>3)
        {
            return 6;
        }
        
        errCode = set(words);
    }
    else if (!strcmp(words[0],"print"))
    {
        if(count>2)
        {
            return 6;
        }
        
        errCode = print(words);
    }
    else if (!strcmp(words[0],"run"))
    {
        if(count>2)
        {
            return 6;
        }
        
        errCode = run(words);
    }
    else if (!strcmp(words[0],"exec"))
    {
        if(count>4)
        {
            return 6;
        }
        
        errCode=exec(words,count);

    }
    
    else if(!strcmp(words[0],"test"))
    {
        errCode=test(words,count);
    }
    else if(!strcmp(words[0],"mount"))
    {
        if(count>4)
        {
            return 6;
        }
        
        errCode=mounty(words,count);

    }
    
    else if(!strcmp(words[0],"write"))
    {
        if(count>10)
        {
            return 6;
        }
        
        errCode=writy(words,count);
    }
    
    else if(!strcmp(words[0],"read"))
    {
        if(count>10)
        {
            return 6;
        }
        
        errCode=ready(words,count);
    }

    
    else
    {
        errCode=1;
    }
    
    
    return errCode;
    
}

void errors(int errCode)
{
    if(errCode  !=0)
    {
        if (errCode==1)
        {
            printf("%s\n","Unknown Command");
        }
        if (errCode==3)
        {
            printf("%s\n","Variable does not exist");
        }
        if (errCode==5)
        {
            printf("%s\n","Script not Found");
        }
        if (errCode==6)
        {
            printf("%s\n","Too many arguments");
        }
        if (errCode==9)
        {
            printf("%s\n","Too many arguments");
        }
        
    }

}

//method to merge together 2 strings, used in set command

char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}


//method to chop off "\n" in commands

char *choppy( char *s )
{
    char *n = malloc( strlen( s ? s : "\n" ) );
    if( s )
        strcpy( n, s );
    n[strlen(n)-1]='\0';
    return n;
}


//help command, gives a list of existing commands in the shell

int help(char *words[])
{
    
    
    char *commands[100000];
    commands[0]="quit\t\t\t\t\t\t\t\tEnds prompt with 'Bye!'\n";
    commands[1]="set [VAR] [STRING] \t\t\t\t\tSaves VAR as STRING in shell memory, overwrites VAR\n";
    commands[2]="print [VAR]\t\t\t\t\t\t\tPrints VAR if exists\n";
    commands[3]="run [SCRIPT.TXT]\t\t\t\t\tRuns SCRIPT.TXT, takes in command line by line\n";
    commands[4]="help\t\t\t\t\t\t\t\tReturns all COMMANDS with DESCRIPTIONS\n";
    
    
    
    printf("%s","These shell commands are defined internally.  Type `help' to see this list\n\n");
    
    printf("%s","COMMAND\t\t\t\t\t\t\t\tDESCRIPTION\n");
    
    for(int i=0;i<5;i++)
    {
        printf("%s",commands[i]);
    }
    return 0;
}

// quit command that sends an error code to the main method to stop the prompt loop

int quit(char *words[])
{
    
    char message[]="Bye!\n";
    printf("%s", message);
    return 2;
}

//method to set variable = value in the shell memory

int set(char *words[])
{
    
    
    char *word=m1Get(words[1]);
    
    
    
    
    if(!strcmp(word,"Not Found"))
    {
       
        
        char *variable = concat(words[1], "=");
        char *value= concat(variable, words[2]);
        
        m1Set(value);
    }
    else
    {
        m1Replace(words[1], words[2]);
        
    }
    
    return 0;
}



int test(char *words[],int count)
{
    char *fileName="test2";
    char *name="testfile";
    
    //initIO();
    //partitions(fileName, 5, 10);
    mountt(fileName);
    
  
    
    int x=openfile(name,fileName);
    printf("GOT %d\n",x);
    readBlock(x);
    writeBlock(x, "12345",fileName);
    readBlock(x);
    
    
    
  
    return 7;
    
}


int mounty(char *words[],int count)
{
    partitionName=words[1];
    int number_of_blocks=atoi(words[2]);
    int block_sizes=atoi(words[3]);
    
    
    
    //printf("INPUTS ARE %s, %d, %d\n",partitionName,number_of_blocks,block_sizes);
    initIO();
    partitions(partitionName, block_sizes, number_of_blocks);
    mountt(partitionName);
    
    return 60;
}


void writeMultipleBlocks(int index, char *data)
{
    //printf("MULTIPLE TERRIRORY\n");
    double size=ceil(((strlen(data))/(partition.block_size*1.0)));
    int position=0;
    
    int calculation=(size*partition.block_size)-((strlen(data)));
    
    for(int i=0;i<size;i++)
    {
        char *block=malloc(sizeof(block));
        memcpy( block, &data[position], partition.block_size );
        
        
        
        if(i==size-1)
        {
            
            for(int i=0;i<calculation;i++)
            {
                strcat(block,"0");
                
                
            }
        }
         
        
        strcat(block," ");
        //printf("FIRST BLOCK WRITTEN IS %s\n",block);
        writeBlock(index, block, partitionName);
        position=position+partition.block_size;
        
    }
    
}

int writy(char *words[], int count)
{
    
    char* fileName=words[1];
    
    char* data=malloc(sizeof(data));
    
    for(int i=2;i<count;i++)
    {
        strcat(data, words[i]);
        
        if(i!=(count-1))
        {
           strcat(data, " ");
        }
        
    }
    
    //printf("IT IS %s\n",data);
    
    
    const char s1[1] = "]";
    const char s2[1] = "[";
    char *cleanData;

    cleanData = strtok(data, s1);
    
    //printf("Cleaned is %s\n",cleanData);

    
    
    int index=openfile(fileName, partitionName);
    //printf("partition file name is %s\n",partitionName);
    //printf("index is %d\n",index);
    

  
    
    if(strlen(cleanData)==partition.block_size)
    {
        
        writeBlock(index, cleanData, partitionName);
    }
    else if(strlen(cleanData)>partition.block_size)
    {
        writeMultipleBlocks(index,cleanData);
    }
    
    return 60;
}


int ready(char *words[], int count)
{
    char* fileName=words[1];
    int numberOfBlocks = 0;
    int index=0;
    
    
    
    for(int i=0;i<20;i++)
    {
        if(strcmp(fileName,&fat[i].filename)==0)
        {
            index=i;
            
        }
    }
    
    
    
    if(index<20)
    {
        if(fat[index+1].blockPtrs[0]==-1)
        {
            numberOfBlocks=1;
        }
        if(fat[index+1].blockPtrs[0]!=-1)
        {
            numberOfBlocks=fat[index+1].blockPtrs[0]-fat[index].blockPtrs[0]-1;
        }
    
    }
    //printf("INDEX IS %d\n",index);
    

    
    for(int i=0;i<5;i++)
    {
        if(map[i].fatIndex==index)
        {
            
            long size=ftell(fp[i]);
            fseek(fp[i], size-6, SEEK_SET);
            //printf("FTELL IS %ld\n", size);
            //printf("number of blocks is %d\n",numberOfBlocks);
            
            for(int i=0;i<numberOfBlocks;i++)
            {
                readBlock(index);
            }
           
            //printf("index is %d\n",index);
            
            
            printf("%s\n",returnBlock());
            rewind(fp[i]);
            
            fseek(fp[i], size, SEEK_SET);
            long sizee=ftell(fp[i]);
            
            //printf("FTELL IS %ld\n", sizee);
            break;

            
        }
        
    }
    
    
    

    return 60;
}




//method to print the value of a given variable

int print(char *words[])
{
    
    
    char *word=m1Get(words[1]);
    
   
    
    if(!strcmp(word,"Not Found"))
    {
       
        return 3;
    }
    else
    {
       
        printf("%s\n",word);
    }
    
    return 0;
}

//script method that reads line by line given a text file, and executes each line as a command prompt

int script(char *fileName)
{
    
    
    int errCode =0;
    char line[1000];
    FILE *p =fopen(fileName,"r");
    
    if(p==NULL)
    {
        errCode=5;
        return errCode;
    }
    else
    {
        fgets(line,999,p);
        
        
        while(!feof(p))
        {
            
            line[strcspn(line, "\n")] = '\0';
            line[strcspn(line, "\r")] = '\0';
            
            //printf(" what i read is: %s\n",line);
            
            
            errCode =parseInput(line);
            
            
            //error codes defined within script again
            
            
            errors(errCode);
            
            
            char *check = fgets(line, 999,p);
           
            
            if(check==NULL )
            {
                break;
            }
            
         
            
            if(!strcmp(check,"\n") )
            {
                break;
            }
        }
        
        fclose(p);
        
    }
    return errCode;
    
    
    
}


//method to run script of text file

int run(char *words[])
{
    
    int errCode=script(words[1]);
    return errCode;
    
}

int checkSameName(char *words[],int count)
{
    char variable[1000];
    int boolean=0;
    int index;
    for(int i=0;i<count;i++)
    {
        for(int j=0;j<(count);j++)
        {
            if(i!=j)
            {
                if(!strcmp(words[i],words[j]))
                {
                    boolean=1;
                    strcpy(variable, words[i]);
                    index=i;
                    
                }
            }
            
        }
        
    }
    if(boolean==1)
    {
        printf("Error: Script %s already loaded\n",variable);
        printf("\n");
    }
    
    return boolean;
}

void getDuplicate(char *words[],int count, char * result)
{

    int index;
    for(int i=1;i<count;i++)
    {
        for(int j=1;j<(count);j++)
        {
            if(i!=j)
            {
                if(!strcmp(words[i],words[j]))
                {

                    strcpy(result, words[i]);
                    
                }
            }
            
        }
        
    }

}



int exec(char *words[],int count)
{
   
    
    for(int i=1;i<count;i++)
    {
        FILE *check=fopen(words[i],"r");
        if(check==NULL)
        {
            printf("File %s not found \n",words[i]);
            fclose(check);
            
        }
        else
        {
            fclose(check);
            char command[1000]="";
            strcat(command, "cp ");
            strcat(command,words[i]);
            strcat(command," BackingStore");
            system(command);
            
            
            
            char location[1000]="";
            strcat(location, "BackingStore/");
            strcat(location,words[i]);
            
            FILE *p=fopen(location,"r");
            launcher(p);
            
            strcpy(tail->data->filePath,location);
        }
    
      
       

    }


    scheduler();
    
    

    system("rm -rf BackingStore/*");
    
    
    return 0;
        
}
  


    


