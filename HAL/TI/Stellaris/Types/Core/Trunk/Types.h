/*****************************************************************************
//	 $Workfile: types.h $
//	  Function:	user defined types
//		Author:	Bill Basser
//	 $JustDate:  6/21/07 $
//	 $Revision: 9 $
//
//	This document contains proprietary data and information of 
//  Cyber Integration LLC.  It is the exclusive property of 
//  Cyber Integration, LLC and will not be disclosed in any form to 
//  any party without prior written permission of Cyber Integration, LLC.
//  This document may not be reproduced or further used without the 
//  prior written permission of Cyber Integration, LLC.
//  Copyright (C) 2003-2005 Cyber Integration, LLC. All Rights Reserved
//
//	$History: types.h $
 * 
******************************************************************************/

// test	for	types defined 
#ifndef	_TYPES_H
#define	_TYPES_H

// system includes
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

// StellarisWare Library includes
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_nvic.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_timer.h"
#include "inc/hw_types.h"
#include "driverlib/flash.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"

// define the special types
typedef char				        C8;
typedef signed char			    S8;   
typedef unsigned char		    U8;	
typedef signed short		    S16;	
typedef unsigned short      U16;   
#if !defined(BOOL)
typedef unsigned char		    BOOL;	 
#endif
typedef signed long			    S32;  
typedef unsigned long		    U32;
typedef signed long long	  S64;
typedef unsigned long long  U64; 
typedef float				        FLOAT; 
typedef double				      DOUBLE;
typedef volatile C8			    VC8;
typedef volatile S8			    VS8;	 
typedef volatile U8  		    VU8;	 
typedef volatile S16		    VS16;	 
typedef volatile U16		    VU16;	
typedef volatile S32		    VS32;  
typedef volatile U32		    VU32; 
typedef volatile S64		    VS64;
typedef volatile U64		    VU64;
typedef volatile BOOL  		  VBOOL;  
typedef volatile FLOAT		  VFLOAT;	
typedef volatile DOUBLE		  VDOUBLE;
typedef char*				        PC8;
typedef S8*					        PS8; 
typedef U8*		  			      PU8;	
typedef S16*				        PS16;	
typedef U16*				        PU16;  
typedef S32*				        PS32;  
typedef U32*				        PU32;
typedef S64*				        PS64;  
typedef U64*				        PU64;
typedef FLOAT* 				      PFLOAT;	
typedef void*				        PVOID;
typedef BOOL*               PBOOL;
typedef volatile char*		  PVC8;
typedef volatile PS8		    PVS8;	
typedef volatile PU8		    PVU8;	 
typedef volatile PS16		    PVS16;	 
typedef volatile PU16		    PVU16;	
typedef volatile PS32		    PVS32;	
typedef volatile PU32		    PVU32;
typedef volatile PFLOAT		  PVFLOAT;

// define the eeprom macro
#define	EEPROM	
#define	CODE	
#ifndef PACKED
#define PACKED  	  __attribute__ ((__packed__))
#endif
#define	INTERRUPT 	__attribute__ ((interrupt("IRQ")))
#define	NAKED		    __attribute__ ((naked))

// define the value	union
typedef	union
{
  U16	wValue;
  U8	anValue[ 2 ];
} U16UN, *PU16UN;
#define LE_U16_LSB_IDX  0
#define LE_U16_MSB_IDX  1
#define BE_U16_LSB_IDX  1
#define BE_U16_MSB_IDX  0

typedef	union
{
  FLOAT	fValue;
  U32		uValue;
  U16		awValue[ 2 ];
  U8		anValue[ 4 ];
} U32UN, *PU32UN;
#define LE_U32_LSB_IDX  0
#define LE_U32_MS1_IDX  1
#define LE_U32_MS2_IDX  2
#define LE_U32_MSB_IDX  3
#define BE_U32_LSB_IDX  3
#define BE_U32_MS1_IDX  2
#define BE_U32_MS2_IDX  1
#define BE_U32_MSB_IDX  0
#define	LE_U32_LSW_IDX	0
#define	LE_U32_MSW_IDX	1
#define	BE_U32_LSW_IDX	1
#define	BE_U32_MSW_IDX	0

typedef	union
{
  U64		hValue;
  U32		auValue[ 2 ];
  U16		awValue[ 4 ];
  U8		anValue[ 8 ];
} U64UN, *PU64UN;
#define LE_U64_LSB_IDX  0
#define LE_U64_MS1_IDX  1
#define LE_U64_MS2_IDX  2
#define LE_U64_MS3_IDX  3
#define LE_U64_MS4_IDX  4
#define LE_U64_MS5_IDX  5
#define LE_U64_MS6_IDX  6
#define LE_U64_MSB_IDX  7
#define BE_U64_LSB_IDX  7
#define BE_U64_MS1_IDX  6
#define BE_U64_MS2_IDX  5
#define BE_U64_MS3_IDX  4
#define BE_U64_MS4_IDX  3
#define BE_U64_MS5_IDX  2
#define BE_U64_MS6_IDX  1
#define BE_U64_MSB_IDX  0
#define	LE_U64_LSW_IDX	0
#define	LE_U64_MW1_IDX	1
#define	LE_U64_MW2_IDX	2
#define	LE_U64_MSW_IDX	3
#define	BE_U64_LSW_IDX	3
#define	BE_U64_MW1_IDX	2
#define	BE_U64_MW2_IDX	1
#define	BE_U64_MSW_IDX	0
#define	LE_U64_LSU_IDX	0
#define	LE_U64_MSU_IDX	1
#define	BE_U64_LSU_IDX	1
#define	BE_U64_MSU_IDX	0

// define the min/max macros
#ifndef MAX
#define	MAX(a,b)	(((a) > (b)) ? (a) : (b))
#endif
#ifndef MIN
#define	MIN(a,b)	(((a) < (b)) ? (a) : (b))
#endif

// define the TRUE/FALSE ON/OFF
#define	TRUE	1
#define	FALSE	0
#define	ON		1
#define	OFF		0
#define	HI		1
#define	LO		0

// define the bit macro
#ifndef BIT
#define BIT(n)          (1L << (n))
#endif

#ifndef BITS
#define	BITS(v,n)		(v << (n))
#endif

// define the forever macro
#define	FOREVER	for( ; ; )

// define the low/high macros
#define	LO16(a)	(a&0xFF)
#define	HI16(a)	((a>>8)&0xFF)

// define the program space read/write macros
#define	PGM_RDBYTE(a)	(a)
#define	PGM_RDWORD(a)	(a)
#define	PGM_RDDWRD(a)	(a)
#define	PGM_RDBLCK(a)	(a)

// define the printf macro
#define	PRINTF_P    printf
#define	PUTS_P      puts
#define	STRNCMP_P   strncmp
#define	STRTOK      strtok
#define STRTOK_P    strtok
#define	FPUTS_P     fputs
#define	FPRINTF_P   fprintf
#define	STRCPY_P    strcpy
#define MEMCPY_P    memcpy
#define SPRINTF_P   sprintf

// enumerations ---------------------------------------------------------------
typedef enum _TIMEOSTYPE
{
  TIME_OS_ANDROID = 0,
  TIME_OS_MICRODOS,
  TIME_OS_IOS,
  TIME_OS_UNIX,
  TIME_OS_IBMPCBIOS,
  TIME_OS_NTP,
  TIME_OS_MAX
} TIMEOSTYPE;

/// define the date/time	structures
typedef	struct _DATETIME
{
	U8	nMonth;       ///< month ( 1-12 )
	U8	nDay;         ///< day ( 1-31 )
	U16	wYear;        ///< year ( xxxx )
	U16	wJulian;      ///< julian day( 1-365 )
	U8	nDayOfWeek;   ///< day of week ( 1-7)
	U8	nHours;       ///< hours ( 0-23 )
	U8	nMinutes;     ///< minutes ( 0-59 )
	U8	nSeconds;     ///< seconds ( 0-59 )
} DATETIME, *PDATETIME;
#define DATETIME_SIZE	  sizeof( DATETIME )

/// define the FAT date/time structure
typedef union _FATTIME
{
	struct
	{
		U32	uSeconds	: 5;
		U32	uMinutes	: 6;
		U32	uHours		: 5;
		U32	uDay		  : 5;
		U32	uMonth		: 4;
		U32	uYear		  : 7;
	} tFields;
	U32 uTime;
} FATTIME, *PFATTIME;
#define FATTIME_SIZE    sizeof( FATTIME )

// varialbe	type modifiers
/*************************************************************************************
    c =	int8_t
    n =	unsigned int8_t
    i =	int
    w =	unsigned int
    l =	long
    u =	unsigned long
    f =	float
    d =	double
    b =	boolean
    s =	string
    t =	structure
    h = 64 bit value

    pointer	modifier for above
    p =	pointer
    a =	array
*************************************************************************************/

#endif
