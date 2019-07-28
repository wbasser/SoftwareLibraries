/******************************************************************************
 * @file UsiI2cSlave.c
 *
 * @brief Universal Serial Interface implementation
 *
 * This file 
 *
 * @copyright Copyright (c) 2012 Cyber Intergration
 * This document contains proprietary data and information of Cyber Integration 
 * LLC. It is the exclusive property of Cyber Integration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Cyber Integration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Cyber Integration, LLC.
 *
 * Version History
 * ======
 * $Log: $
 * 
 *
 * \addtogroup UsiI2cSlave
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "UsiI2cSlave/UsiI2cSlave.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
typedef	enum _TWISTATE
{
	SLV_CHK_ADR = 0,	// slave address
	SLV_XMTSND_DAT,		  // slave send data
	SLV_XMTCHK_ACK,		  // check reply on send data
	SLV_XMTREQ_ACK,		  // request ack/nak on send data
	SLV_RCVREQ_DAT,		  // request data
	SLV_RCVGET_DAT,		  // get data and send ack
} TWISTATE;

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static	U8			          nDevAddr;
static	TWISTATE	        eState;
static  PVUSIINTCALLBACK  pvCallback;

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function UsiI2cSlave_Initialize
 *
 * @brief USI initialization
 *
 * This function will inititialize the USI for slave I2C operation
 *
 *****************************************************************************/
void UsiI2cSlave_Initialize( void )
{
	// set SCL/SDA outputs high
	USI_CTL_PRT |= ( USI_SDA_MSK | USI_SCL_MSK );
	USI_CTL_DDR |= USI_SDA_MSK;

	// set up the USI for an interrupt on start/positive edge clock/TWI mode
	USI_CTL_REG = USI_SIE_MSK | USI_WM1_MSK | USI_WM0_MSK | USI_CS1_MSK;
	USI_STS_REG = USI_SIF_MSK | USI_OIF_MSK | USI_SPF_MSK | USI_DCL_MSK;

  // set the device address to default
  nDevAddr = USI_DEFAULT_ADDR;

  // get the callback
  pvCallback = ( PVUSIINTCALLBACK )PGM_RDWORD( pvUsiCallBack );
}

/******************************************************************************
 * @function UsiI2cSlave_SetAddress
 *
 * @brief set the device address
 *
 * This function will set the device address
 *
 * @param[in]   nAddress    desired device address
 *
 *****************************************************************************/
void UsiI2cSlave_SetAddress( U8 nAddress )
{
  // save the address
  nDevAddr = nAddress;
}

/******************************************************************************
 * @function USI_STR_VEC
 *
 * @brief USI start interrupt request handler
 *
 * This function will handle the start detection interrupt handler
 *
 *****************************************************************************/
ISR( USI_STR_VEC )
{
	// reset the state
	eState = SLV_CHK_ADR;
	
	// set SDA as an input
	USI_CTL_DDR &= ( U8 )~USI_SDA_MSK;
	
	// ensure stat condition has occured
	while(( USI_CTL_PIN & USI_SCL_MSK ) & !( USI_STS_REG & USI_SPF_MSK ));
	
	// enable overflow interrupts/clear masks
	USI_CTL_REG |= USI_OIE_MSK;
	USI_STS_REG = USI_SIF_MSK | USI_OIF_MSK | USI_SPF_MSK | USI_DCL_MSK;
}

/******************************************************************************
 * @function USI_OVF_VEC
 *
 * @brief USI overflow interrupt request handler
 *
 * This function will handle the counter overflow interrupt handler
 *
 *****************************************************************************/
ISR( USI_OVF_VEC )
{
	U8	  nTemp;
  BOOL  bStatus;

  // check for a stop condition
  if ( USI_STS_REG & USI_SPF_MSK )
  {
    // issue a stop event
    if ( pvUsiCallBack != NULL )
    {
      // process the callback
      pvUsiCallBack( USI_IRQ_EVENT_STPRCVD, &nTemp );
    }
  }

	// process the state
	switch( eState )
	{
	  case SLV_CHK_ADR :
		  // get the data
		  nTemp = USI_DAT_REG;
		  if ((( nTemp >> 1 ) == USI_DEFAULT_ADDR ) || ( nTemp == 0 ))
		  {
			  // test the direction
			  if ( nTemp & 0x01 )
			  {
				  // slave transmit mode
				  eState = SLV_XMTSND_DAT;
			  }
			  else
			  {
				  // slave receive mode
				  USI_DAT_REG = 0;
				  USI_CTL_DDR |= USI_SDA_MSK;
				  USI_STS_REG = ( USI_OIF_MSK | USI_SPF_MSK | USI_DCL_MSK | 14 );
				
				  // goto request data
				  eState = SLV_RCVREQ_DAT;
			  }
		  }
		  else
		  {
			  // not ours - setup for start condition
			  USI_CTL_REG &= ( U8 )~USI_OIE_MSK;
			  USI_STS_REG |= ( USI_OIF_MSK | USI_SPF_MSK | USI_DCL_MSK );
			  eState = SLV_CHK_ADR;
		  }
		  break;
		
	  case SLV_XMTCHK_ACK :
		  // check for a NACK from the master
		  if ( USI_DAT_REG != 0 )
		  {
			  // setup for start condition
			  USI_CTL_REG &= ( U8 )~USI_OIE_MSK;
			  USI_STS_REG |= ( USI_OIF_MSK | USI_SPF_MSK | USI_DCL_MSK );
			  eState = SLV_CHK_ADR;
		  }
		  else
		  {
        // check for a valid get function pointer
        if ( pvUsiCallBack != NULL )
        {
          // do the callback
          bStatus = pvUsiCallBack( USI_IRQ_EVENT_GETCHAR, &nTemp );
        }
        else
        {
          bStatus = FALSE;
        }
      
        // determine action
			  if ( bStatus )
			  {
				  // output data/setup for send
				  USI_DAT_REG = nTemp;
				  USI_CTL_DDR |= USI_SDA_MSK;
				
				  // clear flags - goto SLV_REQ_ACK 
				  USI_STS_REG = ( USI_OIF_MSK | USI_SPF_MSK | USI_DCL_MSK );
				  eState = SLV_XMTREQ_ACK;
			  }
			  else
			  {
				  // setup for start condition
				  USI_CTL_REG &= ( U8 )~USI_OIE_MSK;
				  USI_STS_REG |= ( USI_OIF_MSK | USI_SPF_MSK | USI_DCL_MSK );
				  eState = SLV_CHK_ADR;
			  }
		  }
		  break;
		
	  case SLV_XMTSND_DAT :
      // check for a valid get function pointer
      if ( pvUsiCallBack != NULL )
      {
        // do the callback
        bStatus = pvUsiCallBack( USI_IRQ_EVENT_GETCHAR, &nTemp );
      }
      else
      {
        bStatus = FALSE;
      }
      
      // determine action
			if ( bStatus )
			{
  			// output data/setup for send
	  		USI_DAT_REG = nTemp;
		  	USI_CTL_DDR |= USI_SDA_MSK;
			
			  // clear flags - goto SLV_REQ_ACK
			  USI_STS_REG = ( USI_OIF_MSK | USI_SPF_MSK | USI_DCL_MSK );
			  eState = SLV_XMTREQ_ACK;
		  }
		  else
		  {
  			// setup for start condition
	  		USI_CTL_REG &= ( U8 )~USI_OIE_MSK;
		  	USI_STS_REG |= ( USI_OIF_MSK | USI_SPF_MSK | USI_DCL_MSK );
			  eState = SLV_CHK_ADR;
		  }
		  break;
		
	  case SLV_XMTREQ_ACK :
		  // set USI to read ACK
		  USI_CTL_DDR &= ( U8 )~USI_SDA_MSK;
		  USI_DAT_REG = 0;
		  USI_STS_REG = ( USI_OIF_MSK | USI_SPF_MSK | USI_DCL_MSK | 14 );
		  eState = SLV_XMTCHK_ACK;
		  break;
		
	  case SLV_RCVREQ_DAT :
		  // set USI to read data
		  USI_CTL_DDR &= ( U8 )~USI_SDA_MSK;
		  USI_STS_REG = ( USI_OIF_MSK | USI_SPF_MSK | USI_DCL_MSK );
		  eState = SLV_RCVGET_DAT;
		  break;
		
	  case SLV_RCVGET_DAT :
		  // read the data
		  nTemp = USI_DAT_REG;
    
      // check for a valid get function pointer
      if ( pvUsiCallBack != NULL )
      {
        // do the callback
        pvUsiCallBack( USI_IRQ_EVENT_PUTCHAR, &nTemp );
      }
		
		  // send ack and go back to SLV_REQ_DAT
		  USI_DAT_REG = 0;
		  USI_CTL_DDR |= USI_SDA_MSK;
		  USI_STS_REG = ( USI_OIF_MSK | USI_SPF_MSK | USI_DCL_MSK | 14 );
		  eState = SLV_RCVREQ_DAT;
		  break;
		

	  default :
		  // should never happen
		  eState = SLV_CHK_ADR;
		  break;
	}
}

/**@} EOF Usi.c */
