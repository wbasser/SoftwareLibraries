/******************************************************************************
 * @file TYpes.h
 *
 * @brief type declarations
 *
 * This file provides the type definitions
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
 * \addtogroup Micro
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _TYPES_H
#define _TYPES_H

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

// system includes ------------------------------------------------------------
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// process defines
#include <stm32l4xx.h>

// Macros and Defines ---------------------------------------------------------
// define the map macro
#define MAP(val,inmin,inmax,outmin,outmax)  ((val-inmin)*(outmax-outmin)/(inmax-inmin)+outmin)

// define the eeprom macro
#define	EEPROM	
#define	CODE	  
#define	PACKED      __attribute__((__packed__))
#define ALIGNED(s)  __attribute__((__aligned__(s)))

#define LE_U16_LSB_IDX  0
#define LE_U16_MSB_IDX  1
#define BE_U16_LSB_IDX  1
#define BE_U16_MSB_IDX  0

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
#define	MAX(a,b)	(((a) > (b)) ? (a) : (b))
#define	MIN(a,b)	(((a) < (b)) ? (a) : (b))

// define the TRUE/FALSE ON/OFF
#define	TRUE	  1
#define	FALSE	  0
#define	ON		  1
#define	OFF		  0
#define	HI		  1
#define	LO		  0

// define the GET/PUT macros
#define	GET		  0
#define	PUT		  1

// define the forever macro
#define	FOREVER	for( ; ; )

// define the normal bit masks
#define	BIT_0	  0
#define	BIT_1	  1
#define	BIT_2	  2
#define	BIT_3	  3
#define	BIT_4	  4
#define	BIT_5	  5
#define	BIT_6	  6
#define	BIT_7	  7
#define	BIT_8	  8
#define	BIT_9	  9
#define	BIT_10	10
#define	BIT_11	11
#define	BIT_12	12
#define	BIT_13	13
#define	BIT_14	14
#define	BIT_15	15
#define	BIT_16	16
#define	BIT_17	17
#define	BIT_18	18
#define	BIT_19	19
#define	BIT_20	20
#define	BIT_21	21
#define	BIT_22	22
#define	BIT_23	23
#define	BIT_24	24
#define	BIT_25	25
#define	BIT_26	26
#define	BIT_27	27
#define	BIT_28	28
#define	BIT_29	29
#define	BIT_30	30
#define	BIT_31	31

// define the low/high macros
#define	LO16(a)	  (a&0xFF)
#define	HI16(a)	  ((a>>8)&0xFF)

// define the bit shift macros
#define BIT(a)    (1<<a)
#define BITS(a,v) (a<<v)

// define the program space read/write macros
#define	PGM_RDBYTE(a)	        (a)
#define	PGM_RDWORD(a)	        (a)
#define	PGM_RDDWRD(a)	        (a)
#define	PGM_RDBYTE_PTR(a)	    (a)
#define	PGM_RDWORD_PTR(a)	    (a)
#define	PGM_RDDWRD_PTR(a)	    (a)

// define the eeprom space read/write macros
#define	EEP_RDBYTE(a)	          
#define	EEP_RDWORD(a)	          
#define	EEP_RDDWRD(a)	          
#define	EEP_RDBLOCK(a,b,c)	    
#define	EEP_RDBYTE_PTR(a)	      
#define	EEP_RDWORD_PTR(a)	      
#define	EEP_RDDWRD_PTR(a)	      
#define	EEP_RDBLOCK_PTR(a,b,c)	
#define	EEP_WRBYTE(a,d)	        
#define	EEP_WRWORD(a,d)	        
#define	EEP_WRDWRD(a,d)	        
#define	EEP_WRBLOCK(a,b,c)	    
#define	EEP_WRBYTE_PTR(a,d)	    
#define	EEP_WRWORD_PTR(a,d)	    
#define	EEP_WRDWRD_PTR(a,d)	    
#define	EEP_WRBLOCK_PTR(a,b,c)	

// define the printf macro
#define	PRINTF_P	  printf
#define SPRINTF_P   sprintf
#define	PUTS_P		  puts
#define	STRNCMP_P	  strncmp
#define	STRCMP_P	  strcmp
#define	STRCPY_P	  strcpy
#define	STRSEP_P	  strsep
#define STRTOKR_P   strtok_r
#define MEMCPY_P    memcpy
#define STRLEN_P    strlen

// define the macro to determine the number of elements of an array of structure
#define NUMELEMENTS( array ) \
  ( sizeof( array ) / sizeof( array[0] ))

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

// structures -----------------------------------------------------------------
// define the special types
typedef char				        C8;
typedef int8_t				      S8;
typedef uint8_t				      U8;
typedef int16_t				      S16;
typedef uint16_t			      U16;
typedef uint8_t				      BOOL;
typedef int32_t				      S32;
typedef uint32_t			      U32;
typedef int64_t				      S64;
typedef uint64_t			      U64;
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
typedef BOOL*               PBOOL;
typedef FLOAT* 				      PFLOAT;
typedef void*				        PVOID;
typedef volatile char*		  PVC8;
typedef volatile PS8		    PVS8;
typedef volatile PU8		    PVU8;
typedef volatile PS16		    PVS16;
typedef volatile PU16		    PVU16;
typedef volatile PS32		    PVS32;
typedef volatile PU32		    PVU32;
typedef volatile PFLOAT		  PVFLOAT;

// define the value	unions
typedef	union
{
  U16	wValue;
  U8	anValue[ 2 ];
} U16UN, *PU16UN;

typedef	union
{
  FLOAT	fValue;
  U32		uValue;
  U16		awValue[ 2 ];
  U8		anValue[ 4 ];
} U32UN, *PU32UN;

typedef	union
{
  U64		hValue;
  U32		auValue[ 2 ];
  U16		awValue[ 4 ];
  U8		anValue[ 8 ];
} U64UN, *PU64UN;

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



/**@} EOF Types.h */

#endif  // _TYPES_H

