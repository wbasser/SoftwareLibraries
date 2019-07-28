/******************************************************************************
 * @file DigiNetAPIProtocolHandler.c
 *
 * @brief DIGI network handler implementation 
 *
 * This file provides the implementation for the DIG device for networks
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
 * \addtogroup DigiNetAPIProtocolHandler
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "DigiNetAPIProtocolHandler/DigiNetAPIProtocolHandler.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the start delimiter
#define START_DELIMITER_CHAR                      ( 0x7E )

/// define the maximum buffer size
#define MAX_BUFFER_SIZE                           ( 1500 )

// enumerations ---------------------------------------------------------------
/// enumerate the decode states
typedef enum _RCVDECSTATE
{
  RCVDET_STATE_IDLE = 0,
  RCVDET_STATE_LENMSB,
  RCVDET_STATE_LENLSB,
  RCVDET_STATE_FRMTYPE,
  RCVDET_STATE_FRMID,
  RCVDET_STATE_FRMDATA,
  RCVDET_STATE_CHECKSUM
} RCVDECSTATE;

// structures -----------------------------------------------------------------
/// define the API frame structure
typedef struct _APIFRAMEHDR
{
  U8    nDelimiter;           ///< start delimiter
  U16UN tLength;              ///< message length
  U8    nFrameType;           ///< frame type
  U8    nFrameID;             ///< frame ID
} APIFRAMEHDR, *PAPIFRAMEHDR;
#define APIFRAMEHDR_SIZE                  sizeof( APIFRAMEHDR )

/// define the frame type/frame size structure
typedef struct _FRMTYPESIZE
{
  U8    nFrameType;   ///< frame type
  U8    nFrameSize;   ///< frame size
} FRMTYPESIZE, *PFRMTYPESIZE;
#define FRMTYPESIZE_SIZE                sizeof( FRMTYPESIZE )

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  APIFRAMEHDR tApiFrameHdr;                     ///< API frame header
static  U8          nRcvChecksum;                     ///< receive checksum
static  U8          nFrameId;                         ///< frame ID
static  U16         wFrameDataWrIndex;                 ///< buffer index
static  RCVDECSTATE eRcvDecState;                     ///< receive decode state

// local function prototypes --------------------------------------------------
static  void  TransmitApiPacket( DIGIFRAMETYPEENUM eFrameType, U16 wDataLength );

// constant parameter initializations -----------------------------------------
// instantiate the type/size table
static const CODE  FRMTYPESIZE atFrameTypeSizes[ DIGIFRAME_TYPE_ENUM_MAX ] =
{
  { DIGIFRAME_TYPE_ATCOMMAND,  DIGIATCOMMAND_SIZE  },
  { DIGIFRAME_TYPE_XMTSMS,     DIGIXMTSMS_SIZE     },
  { DIGIFRAME_TYPE_XMTIP,      DIGIXMTIP_SIZE      },
  { DIGIFRAME_TYPE_ATRESPONSE, DIGIATRESPONSE_SIZE },
  { DIGIFRAME_TYPE_XMTSTATUS,  DIGIXMTSTATUS_SIZE  },
  { DIGIFRAME_TYPE_MDMSTATUS,  DIGIMDMSTATUS_SIZE  },
  { DIGIFRAME_TYPE_RCVSMS,     DIGIRCVSMS_SIZE     },
  { DIGIFRAME_TYPE_RCVIP,      DIGIRCVIP_SIZE      },
};

/******************************************************************************
 * @function DigiNetAPIProtocolHandler_Initialize
 *
 * @brief initialization
 *
 * This function will perform any needed initialization
 *
 *****************************************************************************/
void DigiNetAPIProtocolHandler_Initialize( void )
{
  // set the frame id
  nFrameId = 1;
  
  // reset the state
  eRcvDecState = RCVDET_STATE_IDLE;
}

/******************************************************************************
 * @function DigiNetAPIProtocolHandler_SendCommand
 *
 * @brief send an AP command
 *
 * This function will format and fill the trnamsit buffer
 *
 * @param[in]   eFrameType      frame type
 * @param[in]   pnFrameData     pointer to the frame data
 * @param[in]   wFrameLength    frame data length
 * @param[in]   pnXmtBuffer     pointer to the transmit buffer
 *
 * @return      length of the data in the transmit buffer
 *
 *****************************************************************************/
U16 DigiNetAPIProtocolHandler_SendCommand( DIGIFRAMETYPEENUM eFrameType, PU8 pnFrameData, U16 wFrameLength, PU8 pnXmtBuffer )
{
  PAPIFRAMEHDR  ptFrameHdr;
  U16           wLength = 0;
  U8            nChecksum, nBufIdx;
  
  // point the transmit buffer to the frame hdr
  ptFrameHdr = ( PAPIFRAMEHDR )pnXmtBuffer;
  
  // set the delimiter
  ptFrameHdr->nDelimiter = START_DELIMITER_CHAR;
  
  // compute the length/adjust for the frame type/frame ID
  wLength = wFrameLength;
  wLength += 2;
  
  // add the length
  ptFrameHdr->tLength.anValue[ BE_U16_MSB_IDX ] = HI16( wLength );
  ptFrameHdr->tLength.anValue[ BE_U16_LSB_IDX ] = LO16( wLength );
  
  // add the frame type
  ptFrameHdr->nFrameType = eFrameType;
  
  // add the frame id/increment/adjust
  ptFrameHdr->nFrameID = nFrameId++;
  if ( nFrameId == 0 )
  {
    // set to one
    nFrameId = 1;
  }
  
  // copy the frame data to the buffer
  memcpy(( pnXmtBuffer + APIFRAMEHDR_SIZE ), pnFrameData, wFrameLength );
  
  // now compute the checksum
  nChecksum = 0;
  for ( nBufIdx = 3; nBufIdx < wLength; nBufIdx++ )
  {
    // add the bytes
    nChecksum += *( pnXmtBuffer + nBufIdx );
  }
  
  // complement it
  nChecksum = ~nChecksum;
  nChecksum++;
  
  // add it to the end of the buffer
  *( pnXmtBuffer + wLength++ ) = nChecksum;
  
  // return the length
  return( wLength );
}

/******************************************************************************
 * @function DigiNetAPIProtocolHandler_ProcessRcvChar
 *
 * @brief process a receive character
 *
 * This function will process the passed character and return a stus to indicate the
 * current decode status
 *
 * @param[in]   ptFrameData     pointer to the frame data structure storage
 * @param[in]   nChar           incoming character
 *
 * @return      appropriate frame decode status
 *
 *****************************************************************************/
DIGIFRMDECSTS DigiNetAPIProtocolHandler_ProcessRcvChar( PDIGIAPIFRAMEDAT ptFrameData, U8 nChar )
{
  DIGIFRMDECSTS eStatus = DIGI_FRMDEC_STS_NONE;
  
  // add each character to checksum
  nRcvChecksum += nChar;
  
  // determine the state
  switch( eRcvDecState )
  {
    case RCVDET_STATE_IDLE :
      // check for start character
      if ( nChar == START_DELIMITER_CHAR )
      {
        // move to MSB of the length
        eRcvDecState = RCVDET_STATE_LENMSB;
        
        // set in progress status
        eStatus = DIGI_FRMDEC_STS_INPROG;
      }
      break;
      
    case RCVDET_STATE_LENMSB :
      // add it to the frame data/go to LSB of the length
      tApiFrameHdr.tLength.anValue[ BE_U16_MSB_IDX ] = nChar;
      eRcvDecState = RCVDET_STATE_LENLSB;
      break;
      
    case RCVDET_STATE_LENLSB :
      // add it to the frame data/goto to frame type
      tApiFrameHdr.tLength.anValue[ BE_U16_LSB_IDX ] = nChar;
      eRcvDecState = RCVDET_STATE_FRMTYPE;
      
      // reset the checksum
      nRcvChecksum = 0;
      break;
      
    case RCVDET_STATE_FRMTYPE :
      // store the frame type
      tApiFrameHdr.nFrameType = nChar;
      eRcvDecState = RCVDET_STATE_FRMID;
      break;
      
    case RCVDET_STATE_FRMID :
      // store the frame id
      tApiFrameHdr.nFrameID = nChar;
      eRcvDecState = RCVDET_STATE_FRMDATA;
      
      // reset the data write index
      wFrameDataWrIndex = 0;
      break;
      
    case RCVDET_STATE_FRMDATA :
      // store the data
      ptFrameData->anBuffer[ wFrameDataWrIndex++ ] = nChar;
        
      // check for done
      if ( wFrameDataWrIndex >= tApiFrameHdr.tLength.wValue )
      {
        // go to checksum
        eRcvDecState = RCVDET_STATE_CHECKSUM;
      }
      break;
      
    case RCVDET_STATE_CHECKSUM :
      // check for a valid checksum
      if ( nRcvChecksum == 0xFF )
      {
        // set the status to the frame type
        eStatus = tApiFrameHdr.nFrameType;
      }
      
      // in all cases, back to idle
      eRcvDecState = RCVDET_STATE_IDLE;
      break;
      
    default :
      // should never happen
      eRcvDecState = RCVDET_STATE_IDLE;
      break;
  }
  
  // return the status
  return( eStatus );
}

/**@} EOF DigiNetAPIProtocolHandler.c */
