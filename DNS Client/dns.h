/*!
******************************************************************************
\file dns.h
\date 15 May 2022
\author Jonathan Matetzky & Rony Kosistky 
*****************************************************************************/

#ifndef __DNS_H__
#define __DNS_H__

/************************************
*      include                      *
************************************/
#include "definitions.h"
#include "windows.h"
#include "conio.h"
#include "stdio.h"

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
void dnsQuery(unsigned char* host, char* ip);


#endif //__DNS_H__