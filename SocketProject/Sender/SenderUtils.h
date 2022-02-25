/*!
******************************************************************************
\file SenderUtils.c
\date 25 February 2022
\author Jonathan Matetzky & Rony Kosistky
*****************************************************************************/

#ifndef __SENDER_UTILS_H__
#define __SENDER_UTILS_H__

/************************************
*       API                         *
************************************/

/*!
******************************************************************************
\brief
Initialize the sender.
\param
 [in] argv - arguments from the user.
\return none
*****************************************************************************/
void SenderUtils_SenderInit(char* argv[]);

/*!
******************************************************************************
\brief
Adding hamming code to the message we have read.
\return none
*****************************************************************************/
void SenderUtils_AddHammCode();

/*!
******************************************************************************
\brief
Append the massage with the hamming code to the writing buffer.
\return none
*****************************************************************************/
void SenderUtils_AppendToBuffer();

/*!
******************************************************************************
\brief
Reading from the input file the next 26 bits.
\return Number of bits read.
*****************************************************************************/
int SenderUtils_ReadBytesFromFile();

/*!
******************************************************************************
\brief
Tear down our sender.
\return none
*****************************************************************************/
void SenderUtils_SenderTearDown();

#endif //__SENDER_UTILS_H__