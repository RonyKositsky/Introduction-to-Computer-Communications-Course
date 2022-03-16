/*!
******************************************************************************
\file BitTools.h
\date 25 February 2022
\author Jonathan Matetzky & Rony Kosistky
*****************************************************************************/
#ifndef __BIT_TOOLS_H__
#define __BIT_TOOLS_H__

/************************************
*      include                      *
************************************/
#include "Definitions.h"
#include <stdint.h>
#include <stdbool.h>

/************************************
*      defines                      *
************************************/
#define BIT_SET(a,b) ((a) |= (1<<(b)))
#define BIT_CLEAR(a,b) ((a) &= ~(1<<(b)))
#define CHECK_BIT(var,pos) ((var>>pos) & 1)
#define BIT_FLIP(a,b) ((a) ^= (1ULL<<(b)))

/************************************
*       variables                   *
************************************/
static int HammingPairingBitsIndexes[HAMM_PAIRITY_BITS] =
{
	FIRST_INDEX_PAIRITY_BIT,
	SECOND_INDEX_PAIRITY_BIT,
	THIRD_INDEX_PAIRITY_BIT,
	FOURTH_INDEX_PAIRITY_BIT,
	FIFTH_INDEX_PAIRITY_BIT
};

static int HammingMasks[HAMM_PAIRITY_BITS] =
{
	FIRST_PAIRITY_MASK, 
	SECOND_PAIRITY_MASK,
	THIRD_PAIRITY_MASK, 
	FOURTH_PAIRITY_MASK,
	FIFTH_PAIRITY_MASK, 
};


/************************************
*       API                         *
************************************/

/*!
******************************************************************************
\brief
Adding to recieved message 5 pairity bits.
\return Message with uninitialized hamming code.
*****************************************************************************/
uint32_t BitTools_ConvertStringToUint(char *massage, bool hammingAddition);

/*!
******************************************************************************
\brief
Adding to recieved message 5 pairity bits.
\return Message with uninitialized hamming code.
*****************************************************************************/
void BitTools_ConvertUintToString(char* massage, int numberOfBits, uint32_t num);

/*!
******************************************************************************
\brief
Adding to recieved message 5 pairity bits.
\return Message with uninitialized hamming code.
*****************************************************************************/
int BitTools_BitwiseXOR(uint32_t num);

/*!
******************************************************************************
\brief
Getting the nth bit of the num.
\return bit value.
*****************************************************************************/
int BitTools_GetNBit(uint32_t num, int n);

#endif //__BIT_TOOLS_H__