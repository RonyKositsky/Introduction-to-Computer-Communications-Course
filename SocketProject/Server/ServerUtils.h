/*!
******************************************************************************
\file ServerUtils.h
\date 25 February 2022
\author Jonathan Matetzky & Rony Kosistky
*****************************************************************************/

#ifndef __SERVER_UTILS_H__
#define __SERVER_UTILS_H__

/************************************
*      include                      *
************************************/


/************************************
*      definitions                 *
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
Initialize func
\details
Must be called only once
\param
 [in] counter_val - reset counter value
 [out] out_val    -
\return none
*****************************************************************************/
void ServerUtils_ServerInit(char* argv[]);

void ServerUtils_ServerTearDown();

#endif //__SERVER_UTILSw_H__