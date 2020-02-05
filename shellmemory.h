//
//  shellmemory.h
//  OS_Shell
//
//  Created by Aymen on 2/21/19.
//  Copyright © 2019 Aymen. All rights reserved.
//


#include <stdio.h>

extern char *model1[1000];
char *extract(char *model);
void m1Set(char *string);
void m1Replace(char *var, char *value);
char *m1Get(char *var);
