/*!
******************************************************************************
\file BitTools.h
\date 25 February 2022
\author Jonathan Matetzky & Rony Kosistky
*****************************************************************************/
#include "Definitions.h"
#ifndef __BIT_TOOLS_H__
#define __BIT_TOOLS_H__

/************************************
*      include                      *
************************************/

/************************************
*       types                       *
************************************/


/************************************
*       API                         *
************************************/

/*!
******************************************************************************
\brief
Initializing new socket.
\return SOCKET.
*****************************************************************************/
void BitTools_GetMessageBits(char msg[2], char* bits, int size);

void BitTools_CheckBits(char bits[MSG_SIZE], char checkbits[HAMM_PAIRITY_BITS]);

void join_cur_to_msg(int size, int index, int mod, char* msg, char cur[2]);

// we will use this method to receive next 11 bits or next 15 bits from given msg.
// we will save the result in a two char array with the MSB unused bits in the second char 0s.
// we will name these returned chars ret1, ret2
// for this we will need 2-3 chars from the original msg. we will name these chars orig1, orig2, orig3.
void get_next_n_bits(int n, int index, int mod, char* msg, char result[2]);

#endif //__BIT_TOOLS_H__