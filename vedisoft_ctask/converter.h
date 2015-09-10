//#pragma once

#include <stdio.h>
#include <stdlib.h>


char* hexToBin(char hex);
char binToHex(const char binary[4]);
char* decToBin(unsigned short dec);

void invertString(char* s, int length);

char* parsePacket(char* packet);
char* fixPacket(char* packet);
char* getTypeValue(int type, const char* data, int size);
