/******************************************************************************
 * @file AD5720.c
 *
 * @brief AD5720 programmable resistor implementation 
 *
 * This file provides the implementation for the AD5720
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
 * $Rev: $
 * 
 *
 * \addtogroup AD5720
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "AD5720/AD5720.h"
#include "AD5720/AD5720_prm.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
#define the HIZ sequence bytes
#define HIZ_SEQ0_BYTE                 ( 0x80 )
#define hIZ_SEQ1_BYTE                 ( 0x81 )

/// define the maximum resistance
#define RESISTANCE_MAX                ( 20000 )

/// define the maximum counts
#define COUNTS_MAX                    ( 1024 )

/// define the control register bits
#define CTL_REG_50TP_WRENB            ( 0x01 )
#define CTL_REG_RDAC_WRENB            ( 0x02 )
#define CTL_REG_RDAC_TCDSB            ( 0x04 )
#define CTL_REG_50TP_PGMOK            ( 0x08 )

// enumerations ---------------------------------------------------------------
/// eumerate the commands
typedef enum _COMMANDS
{
  COMMAND_NOP = 0,
  COMMAND_WR_RDAC,
  COMMAND_RD_RDAC,
  COMMAND_STORE_RDAC,
  COMMAND_SFW_RESET,
  COMMAND_READ_50TP_DATA,
  COMMAND_READ_50TP_ADDR,
  COMMAND_WRITE_CTRL_REG,
  COMMAND_READ_CTRL_REG,
  COMMAND_SHUTDOWN,
  COMMAND_MAX
} COMMANDS;

// structures -----------------------------------------------------------------
/// define the shift register structure
typedef union _SHIFTREG
{
  struct
  {
    U16 wData     : 9;      ///< data
    U16 wCommand  : 4;      ///< command
    U16 wReserved : 2;      ///< reserved
  } tFields;
  U16UN tWord;
} SHIFTREG, *PSHIFTREG;
#define SHIFTREG_SIZE             sizeof( SHIFTREG )

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------
static  void  WriteReg( U16UN tData );

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function AD5720_Initializle
 *
 * @brief AD5720 initilaization
 *
 * This function will perform any needed initialization
 *
 *****************************************************************************/
void AD5720_Initializle( void )
{
}

/******************************************************************************
 * @function AD5720_SetResistance
 *
 * @brief set the resistance value
 *
 * This function will set the pot to desired resistance
 *
 * @param[in]   wResistance   desired resistance
 *
 *****************************************************************************/
void AD5720_SetResistance( U16 wResistance )
{
  SHIFTREG  tShiftReg;
  
  // unlock the control register
  tShiftReg.tFields.wCommand = COMMAND_WRITE_CTRL_REG;
  tShiftReg.tFields.wData = CTL_REG_RDAC_WRENB;
  WriteReg( tShiftReg.tWord );
  
  // calculate the DAC code/command/wrfite it
  tShiftReg.tFields.wData = ( wResistance * COUNTS_MAX ) / RESISTANCE_MAX;
  tShiftReg.tFields.wCommand = COMMAND_WR_RDAC;
  WriteReg( tShiftReg.tWord );
  
  // now set Hi-Z
  tShiftReg.tWord.anValue[ LE_U16_MSB_IDX ] = HIZ_SEQ0_BYTE;
  tShiftReg.tWord.anValue[ LE_U16_LSB_IDX ] = HIZ_SEQ1_BYTE;
  WriteReg( tShiftReg.tWord );

  tShiftReg.tWord.anValue[ LE_U16_MSB_IDX ] = 0;
  tShiftReg.tWord.anValue[ LE_U16_LSB_IDX ] = 0;
  WriteReg( tShiftReg.tWord );
}

/******************************************************************************
 * @function WriteReg
 *
 * @brief AD5720 initilaization
 *
 * This function will perform any needed initialization
 *
 * @param[in]   
 * * @return      
 *
 *****************************************************************************/
static void WriteReg( U16UN tData )
{
  // enable the chip select
  Gpio_Set( AD5720_CEN_PIN_ENUM, ON );
  
  // write the data
  Spi_Write( AD5720_SPI_DEV_ENUM, tData.anValue[ LE_U16_MSB_IDX ];
  Spi_Write( AD5720_SPI_DEV_ENUM, tData.anValue[ LE_U16_LSB_IDX ];
  
  // disable the chip select
  Gpio_Set( AD5720_CEN_PIN_ENUM, OFF );
}

/**@} EOF AD5720.c */
