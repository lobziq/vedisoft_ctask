#include "converter.h"

#include <string.h>

void invertString(char* s, int length)
{
	for (int i = 0; i < length / 2; i++)
	{
		char temp = s[i];
		s[i] = s[length - i - 1];
		s[length - i - 1] = temp;	
	}
	
}

char* fixPacket(char* packet)
{
	//checking for errors in packet
	int packetLength = strlen(packet);
	char* fixedPacket = malloc(packetLength + 1);

	int oneCounter = 0;
	int gap = 0;

	for (int i = 0; i < packetLength; i++)
	{
		if (oneCounter == 6)
		{
			if (packet[i] != '0')
			{
				fprintf(stderr, "Incorrect type3 packet, x6 times 1\n");

				strcpy(packet, "Error1");
				return packet;
			}
			oneCounter = 0;
			gap++;
		}
		else
		{
			if (packet[i] == '1')
			{
				oneCounter++;
			}
			else oneCounter = 0;
			fixedPacket[i - gap] = packet[i];
		}
	}

	int fixedPacketLength = packetLength - gap;
	float check = (fixedPacketLength) / 8;

	//check if is divisible by 8
	if (check != (int)check)
	{
		fprintf(stderr, "Incorrect type3 packet, not divisible by 8\n", check);

		strcpy(packet, "Error2");
		return packet;
	}

	//check if longer than 8k bits
	if (fixedPacketLength > 8000)
	{
		fprintf(stderr, "Incorrect type3 packet, length is more than 8000 (%i)\n", fixedPacketLength);

		strcpy(packet, "Error3");
		return packet;
	}

	fixedPacket[fixedPacketLength] = '\0';

	return fixedPacket;
}

char* parsePacket(char* packet)
{
	//already operating with prepared packet
	//ready to convert

	int packetLength = strlen(packet);
	int hexLength = strlen(packet) / 4;
	char* hexdata = malloc(hexLength + 1);

	fprintf(stdout, "hexdata length is %i \n", hexLength);

	for (int i = 0; i < packetLength; i+=4)
	{
		char temp[5];
		temp[0] = packet[i];
		temp[1] = packet[i + 1];
		temp[2] = packet[i + 2];
		temp[3] = packet[i + 3];
		temp[4] = '\0';

		//fprintf(stdout, "Binary to hex converting: %s \n", temp);

		hexdata[i / 4] = binToHex(temp);
	}
	hexdata[hexLength] = '\0';

	return hexdata;
}

char binToHex(const char binary[4])
{
	if (strlen(binary) != 4)
	{
		fprintf(stderr, "Incorrect binary length: %d\n", sizeof(binary) / sizeof(char));
		return 'x';
	}

	char symbols[6] = "ABCDEF";

	const char* table[16];
	table[0] = "0000";
	table[1] = "0001";
	table[2] = "0010";
	table[3] = "0011";
	table[4] = "0100";
	table[5] = "0101";
	table[6] = "0110";
	table[7] = "0111";
	table[8] = "1000";
	table[9] = "1001";
	table[10] = "1010";
	table[11] = "1011";
	table[12] = "1100";
	table[13] = "1101";
	table[14] = "1110";
	table[15] = "1111";

	char c;

	for (int i = 0; i < 16; i++)
	{
		
		if (strcmp(table[i], binary) == 0)
		{
			if (i < 10)
			{
				c = i + '0';
			}
			else
			{
				c = symbols[i - 10];
			}
			
		}
	}

	return c;
}

char* hexToBin(char hex)
{
	int idx = hex - '0';
	char* output = malloc(5);

	const char* table[16];
	table[0] = "0000";
	table[1] = "0001";
	table[2] = "0010";
	table[3] = "0011";
	table[4] = "0100";
	table[5] = "0101";
	table[6] = "0110";
	table[7] = "0111";
	table[8] = "1000";
	table[9] = "1001";
	table[10] = "1010";
	table[11] = "1011";
	table[12] = "1100";
	table[13] = "1101";
	table[14] = "1110";
	table[15] = "1111";

	switch (hex)
	{
		case 'A':
		{
			idx = 10;
			break;
		}
		case 'B':
		{
			idx = 11;
			break;
		}
		case 'C':
		{
			idx = 12;
			break;
		}
		case 'D':
		{
			idx = 13;
			break;
		}
		case 'E':
		{
			idx = 14;
			break;
		}
		case 'F':
		{
			idx = 15;
			break;
		}
	}

	if (idx < 16 && idx >= 0)
	{
		for (int i = 0; i < 5; i++)
		{
			output[i] = table[idx][i];
		}
		return output;
	}
	else
	{
		//error code improvisation
		output = "XXXX";
		return output;
	}

	return output;
}

char* decToBin(unsigned short dec)
{
	char* output = malloc(5);
	char c[6] = "ABCDEF";
	unsigned short n;
	unsigned short oldn = dec;
	unsigned short remainder;

	for (int i = 3; i >= 0; i--)
	{
		n = oldn / 16;
		remainder = oldn - (16 * n);
		oldn = n;
		if (remainder > 9)
		{
			output[i] = c[remainder - 10];
		}
		else
		{
			output[i] = remainder + '0';
		}
	}

	output[4] = '\0';

	return output;
}

char* getTypeValue(int type, const char* data, int size)
{
	if (type == 1 || type == 4 || type == 2)
	{
		int s = size;
		if (type == 2) s /= 8;

		char* output = malloc(7);

		output[0] = '0';
		output[1] = type + '0';
		output[6] = '\0';

		char* d = decToBin(size);

		for (int i = 0; i < 4; i++)
		{
			output[i + 2] = d[i];
		}

		free(d);

		return output;
	}
	else if (type == 3)
	{
		//prepare our packet for packet parsing functions
		int packetLength = (strlen(data) - 15); //packet without boundaries with end symbol
		char* packet = malloc(packetLength);

		for (int i = 0; i < packetLength; i++)
		{
			packet[i] = data[i + 8];
		}

		packet[packetLength - 1] = '\0';

		//return hexdata;
	}
	else
	{
		//???
	}
}