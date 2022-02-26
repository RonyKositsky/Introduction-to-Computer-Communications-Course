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
#include <stdint.h>
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

uint32_t BitTools_PrepareHammingCodeNumber(char massage[MSG_SIZE])
{
	uint32_t val = 1;
	int hammingIndex = 0;
	int stringIndex = 0;

	if (massage == NULL)
		return 0;
	
	for (int index = 1; index < HAMM_MSG_SIZE; index++)
	{
		val <<= 1;
		if (index == HammingPairingBitsIndexes[hammingIndex])
		{
			hammingIndex++;
		}
		else
		{
			val += massage[stringIndex] - '0';
			stringIndex++;
		}
	}
	
	return val;
}

/*!
******************************************************************************
\brief
Adding to recieved message 5 pairity bits.
\return Message with uninitialized hamming code.
*****************************************************************************/
uint32_t BitTools_ConvertStringToUint(char* massage)
{
	uint32_t val = 0; 
	int i = 0;
	if (massage == NULL)
		return 0;

	while (massage[i] == '0' || massage[i] == '1')
	{  
		val <<= 1;
		val += massage[i] - '0';
		i++;
	}

	return val;
}

/*!
******************************************************************************
\brief
Adding to recieved message 5 pairity bits.
\return Message with uninitialized hamming code.
*****************************************************************************/
void BitTools_ConvertUintToString(char* massage, int numberOfBits, uint32_t num)
{
	for (int i = 0; i < numberOfBits; i++)
	{
		massage[i] = (num & (int)1 << (numberOfBits - i - 1)) ? '1' : '0';
	}
	massage[num] = '\0';
}

/*!
******************************************************************************
\brief
Adding to recieved message 5 pairity bits.
\return Message with uninitialized hamming code.
*****************************************************************************/
int BitTools_BitwiseXOR(uint32_t num)
{
	int pairity = 0;
	while (num)
	{
		pairity ^= num & 1;
		num >>= 1;
	}

	return pairity;
}




