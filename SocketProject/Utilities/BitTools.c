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

/************************************
*       API implementation          *
************************************/


/*!
******************************************************************************
\brief
Adding to recieved message 5 pairity bits.
\return Message with uninitialized hamming code.
*****************************************************************************/
uint32_t BitTools_ConvertStringToUint(char* massage, bool hammingAddition)
{
	uint32_t val = 0; 
	int i = 0;
	int hamming_index = 0;

	if (massage == NULL)
		return 0;

	while (massage[i] == '0' || massage[i] == '1')
	{  
		val <<= 1;
		
		if (hammingAddition && i == HammingPairingBitsIndexes[hamming_index])
		{
			hamming_index++;
		}
		else
		{
			val += massage[i] - '0';
		}

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

char BitTools_GetNBit(uint32_t num, int n)
{
	return  (char)(((1 << n) - 1) & num);
}




