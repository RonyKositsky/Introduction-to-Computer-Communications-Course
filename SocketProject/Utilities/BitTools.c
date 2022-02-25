/*!
******************************************************************************
\file BitTools.c
\date 25 February 2022
\author Jonathan Matetzky & Rony Kosistky
*****************************************************************************/

/************************************
*      include                      *
************************************/
#include "BitTools.h"

/************************************
*      functions                    *
************************************/
static char BitTools_GetBitAsChar(char c, int i)
{
	char ret = 1;
	char only_wanted_bit = (ret << (7 - i)) & c;
	return only_wanted_bit != 0;
}

/************************************
*       API implementation          *
************************************/
void BitTools_GetMessageBits(char msg[2], char* bits, int size)
{
	unsigned char cur = msg[0];
	for (int i = 0; i < size; i++)
	{
		int ind = i;
		if (i > 7) 
		{
			cur = msg[1];
			ind = i - 8;
		}
		bits[i] = BitTools_GetBitAsChar(cur, ind);
	}
}


void BitTools_CheckBits(char bits[11], char checkbits[4]) 
{
	unsigned char checkbit1, checkbit2, checkbit3, checkbit4;
	checkbit1 = (bits[0] ^ bits[1] ^ bits[3] ^ bits[4] ^ bits[6] ^ bits[8] ^ bits[10]) & 1;
	checkbit2 = (bits[0] ^ bits[2] ^ bits[3] ^ bits[5] ^ bits[6] ^ bits[9] ^ bits[10]) & 1;
	checkbit3 = (bits[1] ^ bits[2] ^ bits[3] ^ bits[7] ^ bits[8] ^ bits[9] ^ bits[10]) & 1;
	checkbit4 = (bits[4] ^ bits[5] ^ bits[6] ^ bits[7] ^ bits[8] ^ bits[9] ^ bits[10]) & 1;
	checkbits[0] = checkbit1;
	checkbits[1] = checkbit2; 
	checkbits[2] = checkbit3; 
	checkbits[3] = checkbit4;
}



