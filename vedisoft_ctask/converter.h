//#pragma once

#include <stdio.h>
#include <stdlib.h>


char* hexToBin(char hex);
char binToHex(const char* binary);
char* decToBin(unsigned short dec);

void invertString(char* s, int length);

char* preparePacket(const char* packet);
char* binaryPacketToHex(const char* packet);
char* getTypeValue(int type, const char* data, int size);
