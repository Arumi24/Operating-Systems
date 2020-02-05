//
//  IOSCHEDULER.c
//  OS_Shell
//
//  Created by Aymen on 4/10/19.
//  Copyright © 2019 Aymen. All rights reserved.
//

#include "IOSCHEDULER.h"
#include "DISK_driver.h"

struct IOQueue
{
    char *write_data;
    int command;

    
}readwrite_IO[10];


char *IOscheduler(char *data, int cmd, char *patitionFile, char *filename)
{
    for(int i=0;i<10;i++)
    {
      
            readwrite_IO[i].command=cmd;
            readwrite_IO[i].write_data=data;
            
            int index=openfile(filename,patitionFile);
            if(cmd==0)
            {
            
                //readBlock(index);
                //return returnBlock();
                
            }
            if(cmd==1)
            {
                //writeBlock(index, data, patitionFile);
                //readBlock(index);
                
                return returnBlock();
            }
        
    }
    return data;
}
