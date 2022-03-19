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
uint32_t BitTools_ConvertStringToUint(char* massage)
{
	uint32_t val = 0; 
	int hamming_index = 0;
	int	msg_index = 0;

	for (int i = 0; i < HAMM_MSG_SIZE; i++)
	{
		if (massage[i] == '1')
			BIT_SET(val, i);
	}

	return val;
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




