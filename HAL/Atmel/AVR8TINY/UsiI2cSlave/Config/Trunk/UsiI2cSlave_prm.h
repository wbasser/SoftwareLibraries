/******************************************************************************
 * @file UsiI2cSlave_prm.h
 *
 * @brief 
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
 
// ensure only one instantiation
#ifndef _USII2CSLAVE_PRM_H
#define _USII2CSLAVE_PRM_H

// include files
#include "CommandHandler/CommandHandler.h"

// Macros and Defines ---------------------------------------------------------
// define the port/direction/pin/bits for the interface
#define	USI_CTL_PRT		        PORTB
#define	USI_CTL_DDR		        DDRB
#define	USI_CTL_PIN		        PINB
#define	USI_SDA_MSK		        BIT( PB0 )
#define	USI_SCL_MSK		        BIT( PB2 )

// define the status/data/buffer/control registers
#define	USI_STS_REG		        USISR
#define	USI_DAT_REG		        USIDR
#define	USI_BUF_REG		        USIBR
#define	USI_CTL_REG		        USICR

// define the control bits
#define	USI_SIF_MSK		        BIT( USISIF )
#define	USI_OIF_MSK		        BIT( USIOIF )
#define	USI_SPF_MSK		        BIT( USIPF )
#define	USI_DCL_MSK		        BIT( USIDC )
#define	USI_CN3_MSK		        BIT( USICNT3 )
#define	USI_CN2_MSK		        BIT( USICNT2 )
#define	USI_CN1_MSk		        BIT( USICNT1 )
#define	USI_CN0_MSK		        BIT( USICNT0 )
#define	USI_SIE_MSK		        BIT( USISIE )
#define	USI_OIE_MSK		        BIT( USIOIE )
#define	USI_WM1_MSK		        BIT( USIWM1 )
#define	USI_WM0_MSK		        BIT( USIWM0 )
#define	USI_CS1_MSK		        BIT( USICS1 )
#define	USI_CS0_MSk		        BIT( USICS0 )
#define	USI_CLK_MSK		        BIT( USICLK )

// define the vectors
#define	USI_STR_VEC		        USI_START_vect
#define	USI_OVF_VEC		        USI_OVF_vect

// define the callback parameter
#define USI_GET_CALLBACK( PU8 )   CommandHandler_GetValue( PU8 );
#define USI_PUT_CALLBACK( U8 )    CommandHandler_PutValue( U8 );

// define the slave address
/// define the default address
#define USI_DEFAULT_ADDR      ( 0x33 )



/**@} EOF UsiI2cSlave_prm.h */

#endif  // _USII2CSLAVE_PRM_H	
