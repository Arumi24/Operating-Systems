//
//  shellmemory.c
//  Assignment_1
//
//  Created by Aymen on 1/22/19.
//  Copyright © 2019 Aymen. All rights reserved.
//


#include <stdio.h>
#include <string.h>
#include <stdlib.h>


//global shell memory

char *model1[1000];

//Common private functions

//new and improved replace method biietchhhnnneeeggaaa

char *replace(char *str, char *orig, char *rep)
{
    char *ptr;
    
    ptr = strchr(str, '=');
    if (ptr != NULL) {
        *ptr = '\0';
    }
    strcat(str, "=");
    strcat(str, rep);
    
    return str;
}


//method to match variable in the shell memory

int match(char *model, char *var)
{
    int i;
    int len=strlen(var);
    int matchCount=0;
    
    for(i=0;i<len;i++)
    {
        
        if(*(model+i)==*(var+i))
        {
            matchCount++;
        }
    }
    if(matchCount==len)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

//method to extract value of a given shell memory address

char *extract(char *model)
{
    char token = '=';
    char value[1000];
    int i, j, len=strlen(model);
    
    for(i=0;i<len && *(model+i)!=token;i++);
    for(i=i+1,j=0;i<len;i++,j++) value[j]=*(model+i);
    value[j]='\0';
    return strdup(value);
}


//public methods

//method to set new variable = value in shell memory

void m1Set(char *string)
{
    int i=0;
    for(i=0;i<1000;i++)
    {
        if(model1[i]==NULL)
        {
            model1[i]=strdup(string);
            break;
        }
    }
}

//method to replace the variable in th shell memory with a new value

void m1Replace(char *var, char *value)
{
    int i=0;
    for(i=0;i<1000;i++)
    {
        
        if(model1[i]==NULL)
        {
            
        }
        else
        {

            if(match(model1[i],var))
            {
              
                char *replacement=strdup(replace(model1[i],extract(model1[i]),value));
               
                model1[i]=strdup(replacement);
     
                
            }
        }
        
    }
}

//method to get value of variable

char *m1Get(char *var)
{
    int i=0;
    for(i=0;i<1000;i++)
    {
        
        if(model1[i]==NULL)
        {
            
        }
        else
        {
            if(match(model1[i],var))
            {
             
                return extract(model1[i]);
            }
        }
        
    }
    
    return "Not Found";
}









