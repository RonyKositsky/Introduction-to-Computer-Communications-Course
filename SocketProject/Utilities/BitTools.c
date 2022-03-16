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
#include <stdio.h>
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
	int	msg_index = 0;

	if (massage == NULL)
		return 0;

	for (int i = 0; i < HAMM_MSG_SIZE; i++)
	{
		if (hammingAddition && i == HammingPairingBitsIndexes[hamming_index])
		{
			hamming_index++;
		}
		else
		{
			char a = massage[msg_index];
			if (massage[msg_index] == '1')
				BIT_SET(val, i);
			msg_index++;
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

/*!
******************************************************************************
\brief
Getting the nth bit of the num.
\return bit value.
*****************************************************************************/
int BitTools_GetNBit(uint32_t num, int n)
{
	return  ((num >> n & 1));
}




