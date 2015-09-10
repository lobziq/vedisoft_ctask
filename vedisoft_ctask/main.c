#include <stdio.h>
#include <string.h>

#include "converter.h"

int main()
{
	fputs("Hello World full of errors!\n", stderr);

	char input[303] = "";
	int l = strlen(input);
	//fprintf(stdout, "Length is: %d\n", l);

	if (fgets(input, 303, stdin))
	{
		int l = strlen(input);
		//fprintf(stdout, "Length is: %d\n", l);

		if (strlen(input) > 301)
		{
			fputs("Input is too big!\n", stderr);
		}
		if (strlen(input) == 1)
		{
			fputs("Input string is empty!\n", stderr);
		}

		memset(input, 0, sizeof(input));
	}

	char* out = getTypeValue(3, "01111110001110010101010111111011101100000001111110", 14);

	//char* check = binaryPacketToHex("001110010101010111110111011000000");

	//invertString(out, strlen(out));
	//char* out = decToBin(4545);

	fprintf(stdout, "PACKET IS: %s\n", out);



	//free(out);

	return 0;
}