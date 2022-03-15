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
#define FILE_NAME_BUFFER    2048

#define FIRST_INDEX_PAIRITY_BIT  0		 // 2^0 - 1
#define SECOND_INDEX_PAIRITY_BIT 1		 // 2^1 - 1
#define THIRD_INDEX_PAIRITY_BIT  3		 // 2^2 - 1
#define FOURTH_INDEX_PAIRITY_BIT 7		 // 2^3 - 1
#define FIFTH_INDEX_PAIRITY_BIT  15		 // 2^4 - 1

#define FIRST_PAIRITY_MASK		  (0x55555555) //(01010101010101010101010101010101)  
#define SECOND_PAIRITY_MASK		  (0x66666666) //(01100110011001100110011001100110)
#define THIRD_PAIRITY_MASK		  (0x38787878) //(00111000011110000111100001111000)
#define FOURTH_PAIRITY_MASK		  (0x7F807F80) //(01111111100000000111111110000000)
#define FIFTH_PAIRITY_MASK		  (0x7FFF8000) //(01111111111111111000000000000000)

#define ACK (0x80000000)

#define ASSERT(cond, msg, ...)																\
		if (!(cond))																		\
		{																					\
			printf("Assertion failed at file %s line %d: \n", __FILE__, __LINE__);			\
			printf(msg, __VA_ARGS__);														\
			exit(-1);																		\
		}																					\


typedef enum
{
	CLIENT,
	SERVER
}SocketType;

#endif //__SOCKET_TOOLS_H__
