/*!
******************************************************************************
\file Definitios.h
\date 25 February 2022
\author Jonathan Matetzky & Rony Kosistky
*****************************************************************************/

#ifndef __DEFINITIONS_H__
#define __DEFINITIONS_H__

#define MAX_BUFFER 1024
#define MSG_SIZE 26
#define HAMM_MSG_SIZE 31
#define HAMM_PAIRITY_BITS 5
#define FIRST_INDEX_PAIRITY_BIT  1		 // 2^0
#define SECOND_INDEX_PAIRITY_BIT 2		 // 2^1
#define THIRD_INDEX_PAIRITY_BIT  4		 // 2^2
#define FOURTH_INDEX_PAIRITY_BIT 8		 // 2^3
#define FIFTH_INDEX_PAIRITY_BIT  16		 // 2^4
#define FIRST_PAIRITY_MASK  (0xAAAAAAA8) //(10101010101010101010101010101000)  
#define SECOND_PAIRITY_MASK (0x66666660) //(01100110011001100110011001100000)
#define THIRD_PAIRITY_MASK  (0x78787878) //(01111000011110000111100001111000)
#define FOURTH_PAIRITY_MASK (0x3F807F00) //(11111111000000001111111000000000)
#define FIFTH_PAIRITY_MASK  (0xFFFE0000) //(11111111111111100000000000000000)
#define TERMINATION_MESSAGE (0x80000000) // Bit 31 is on.
#define QUIT				(0xC0000000) // Bit 31 & 30 are on.
#define CONTINUE			(0xE0000000) // Bit 31 & 30 & 29 are on.

typedef enum
{
	CLIENT,
	SERVER
}SocketType;

#endif //__SOCKET_TOOLS_H__
