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
	//TODO: Make sure if it is correct for our coding type.
	return ((1 << (7 - i)) & c) != 0;
}

/************************************
*       API implementation          *
************************************/
void BitTools_GetMessageBits(char msg[2], char* bits, int size)
{
	//TODO: Refactor.

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


void BitTools_CheckBits(char bits[MSG_SIZE], char checkbits[HAMM_PAIRITY_BITS])
{
	//TODO: Refactor.

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

void BitTools_ConcatenationMassage(int size, int index, int mod, char* msg, char cur[2])
{
	//TODO: Refactor.
	unsigned char prev_char = msg[index];
	unsigned char orig1 = cur[0];
	unsigned char orig2 = cur[1];
	unsigned char ret1, ret2, ret3;
	if (mod == 0)
	{
		msg[index] = orig1;
		msg[index + 1] = orig2;
	}
	else 
	{
		ret1 = (orig1 >> mod) | prev_char;
		ret2 = (orig1 << (8 - mod)) | (orig2 >> mod);
		ret3 = orig2 << (8 - mod);
		msg[index] = ret1;
		if (index + 1 < size) {
			msg[index + 1] = ret2;
		}
		if (index + 2 < size) {
			msg[index + 2] = ret3;
		}
	}
}

void BitTools_GetNextNBists(int n, int index, int mod, char* msg, char result[2])
{
	//TODO: Refactor for sure.
	unsigned char orig1 = msg[index]; 
	char orig2 = msg[index + 1]; 
	char orig3 = msg[index + 2];

	int total_no_bits_needed_from_next_chars, no_bits_needed_from_orig2, no_bits_needed_from_orig3,
		no_bits_from_orig2_in_ret1, no_bits_from_orig2_in_ret2;

	unsigned char ret1_from_orig1, ret1_fron_orig2, ret1, filtered_orig2, filtered_orig3, ret2_from_orig2, ret2_from_orig3,
		ret2;

	total_no_bits_needed_from_next_chars = n - (8 - mod);

	if (total_no_bits_needed_from_next_chars > 8) 
	{
		no_bits_needed_from_orig2 = 8;
		no_bits_needed_from_orig3 = total_no_bits_needed_from_next_chars - 8;
		filtered_orig2 = orig2;
		filtered_orig3 = (orig3 >> (8 - no_bits_needed_from_orig3)) << (8 - no_bits_needed_from_orig3);
	}
	else 
	{
		no_bits_needed_from_orig2 = total_no_bits_needed_from_next_chars;
		no_bits_needed_from_orig3 = 0;
		filtered_orig2 = (orig2 >> (8 - no_bits_needed_from_orig2)) << (8 - no_bits_needed_from_orig2);
		filtered_orig3 = 0;
	}
	ret1_from_orig1 = orig1 << mod;
	no_bits_from_orig2_in_ret1 = mod;
	ret1_fron_orig2 = filtered_orig2 >> (8 - no_bits_from_orig2_in_ret1);
	ret1 = ret1_from_orig1 | ret1_fron_orig2;
	no_bits_from_orig2_in_ret2 = no_bits_needed_from_orig2 - no_bits_from_orig2_in_ret1;
	ret2_from_orig2 = ((filtered_orig2 << no_bits_from_orig2_in_ret1) >> (8 - no_bits_from_orig2_in_ret2))
		<< (8 - no_bits_from_orig2_in_ret2);
	ret2_from_orig3 = filtered_orig3 >> ((n - 8) - no_bits_needed_from_orig3);
	ret2 = ret2_from_orig2 | ret2_from_orig3;
	result[0] = ret1;
	result[1] = ret2;
}


