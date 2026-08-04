
#ifndef RECORDER_H

#define RECORDER_H

#include <stdio.h>
void screenRec(int width,int breath,int fps,const char* name, unsigned int* PBO);
void isRecord(int choice);
void ifRecord(int option,unsigned int* PBO);
void bufferReader(unsigned int* PBO);

#endif

