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
 * $Log: $
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
#include <stdint.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

// system definitions
#include "SystemDefines/SystemDefines_prm.h"

// Macros and Defines ---------------------------------------------------------
// define the AVR8 flag
#ifndef __ATMEL_AVR__
#define	__ATMEL_AVR__
#endif

// define the eeprom macro
#define	EEPROM	__attribute__((section(".eeprom")))
#define	CODE	  __ATTR_PROGMEM__
#define	PACKED

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

// define the constrain macro
#define CONSTRAIN(a,l,h) ((a)<(l)?(l):((a)>(h)?(h):(a))) 

// define the map macro
#define MAP(val,inmin,inmax,outmin,outmax)  ((val-inmin)*(outmax-outmin)/(inmax-inmin)+outmin)

// define the TRUE/FALSE ON/OFF
#define	TRUE	1
#define	FALSE	0
#define	ON		1
#define	OFF		0
#define	HI		1
#define	LO		0

// define the GET/PUT macros
#define	GET		0
#define	PUT		1

// define the forever macro
#define	FOREVER	for( ; ; )

// define the normal bit masks
#define	BIT_0	0
#define	BIT_1	1
#define	BIT_2	2
#define	BIT_3	3
#define	BIT_4	4
#define	BIT_5	5
#define	BIT_6	6
#define	BIT_7	7

// define the low/high macros
#define	LO16(a)	(a&0xFF)
#define	HI16(a)	((a>>8)&0xFF)

// define the macro for creating a word
#define MAKEU16(m,l)  ((m<<8)|l)

// define the bit shift macros
#define BIT(a)    (1<<a)
#define BITS(a,v) (a<<v)

// define the program space read/write macros
#if ( FLASHEND > 0xFFFF )
#define	PGM_RDBYTE(a)	    pgm_read_byte_far((PU32)&a)
#define	PGM_RDWORD(a)	    pgm_read_word_far((PU32)&a)
#define	PGM_RDDWRD(a)	    pgm_read_dword_far((PU32)&a)
#define	PGM_RDBYTE_PTR(a)	pgm_read_byte_near((U32)a)
#define	PGM_RDWORD_PTR(a)	pgm_read_word_near((U32)a)
#define	PGM_RDDWRD_PTR(a)	pgm_read_dword_near((U32)a)
#else
#define	PGM_RDBYTE(a)		  pgm_read_byte_near((PU16)&a)
#define	PGM_RDWORD(a)		  pgm_read_word_near((PU16)&a)
#define	PGM_RDDWRD(a)		  pgm_read_dword_near((PU16)&a)
#define	PGM_RDBYTE_PTR(a)	pgm_read_byte_near((U16)a)
#define	PGM_RDWORD_PTR(a)	pgm_read_word_near((U16)a)
#define	PGM_RDDWRD_PTR(a)	pgm_read_dword_near((U16)a)
#endif // ( FLASHEND > 0xFFFF )

// define the eeprom space read/write macros
#define	EEP_RDBYTE(a)	          eeprom_read_byte((PU8)&a)
#define	EEP_RDWORD(a)	          eeprom_read_word((PU16)&a)
#define	EEP_RDDWRD(a)	          eeprom_read_dword((PU32)&a)
#define	EEP_RDBLOCK(a,b,c)	    eeprom_read_block((PVOID)&a,(PVOID)&b,c)
#define	EEP_RDBYTE_PTR(a)	      eeprom_read_byte((PU8)a)
#define	EEP_RDWORD_PTR(a)	      eeprom_read_word((PU16)a)
#define	EEP_RDDWRD_PTR(a)	      eeprom_read_dword((PU32)a)
#define	EEP_RDBLOCK_PTR(a,b,c)	eeprom_read_block((PVOID)a,(PVOID)b,c)
#define	EEP_WRBYTE(a,d)	        eeprom_write_byte((PU8)&a,d)
#define	EEP_WRWORD(a,d)	        eeprom_write_word((PU16)&a,d)
#define	EEP_WRDWRD(a,d)	        eeprom_write_dword((PU32)&a,d)
#define	EEP_WRBLOCK(a,b,c)	    eeprom_write_block((PVOID)&a,(PVOID)&b,c)
#define	EEP_WRBYTE_PTR(a,d)	    eeprom_write_byte((PU8)a,d)
#define	EEP_WRWORD_PTR(a,d)	    eeprom_write_word((PU16)a,d)
#define	EEP_WRDWRD_PTR(a,d)	    eeprom_write_dword((PU32)a,d)
#define	EEP_WRBLOCK_PTR(a,b,c)	eeprom_write_block((PVOID)a,(PVOID)b,c)
#define	EEP_UPBYTE(a)	          eeprom_update_byte((PU8)&a)
#define	EEP_UPWORD(a)	          eeprom_update_word((PU16)&a)
#define	EEP_UPDWRD(a)	          eeprom_update_dword((PU32)&a)
#define	EEP_UPBLOCK(a,b,c)	    eeprom_update_block((PVOID)&a,(PVOID)&b,c)
#define	EEP_UPBYTE_PTR(a)	      eeprom_update_byte((PU8)a)
#define	EEP_UPWORD_PTR(a)	      eeprom_update_word((PU16)a)
#define	EEP_UPDWRD_PTR(a)	      eeprom_update_dword((PU32)a)
#define	EEP_UPBLOCK_PTR(a,b,c)	eeprom_update_block((PVOID)a,(PVOID)b,c)
#define EEP_IS_RDY()            eeprom_is_ready()

// define the printf macro
#define	PRINTF_P	printf_P
#define SPRINTF_P sprintf_P
#define	PUTS_P		puts_P
#define	STRNCMP_P	strncmp_P
#define	STRCMP_P	strcmp_P
#define	STRCPY_P	strcpy_P
#define	STRSEP_P	strsep_P
#define STRTOK_P  strtok_P
#define MEMCPY_P  memcpy_P
#define STRLEN_P  strlen_P
#define FPRINTF_P fprintf_P

// define the macro to determine the number of elements of an array of structure
#define NUMELEMENTS( array ) \
  ( sizeof( array ) / sizeof( array[0] ))

/// define the macro to write protected I/O registers
#define WRITE_PROTECTED_REGISTER( puAddr, nValue ) \
  __asm__ __volatile__( \
  "in     __tmp_reg__, %0"    "\n\t" \
  "out    %0, __zero_reg__"   "\n\t" \
  "movw   r30, %3"            "\n\t" \
  "out    %1, %2"             "\n\t" \
  "st     Z, %4"              "\n\t" \
  "out    %0, __tmp_reg__"    "\n\t" \
  : \
  : "M" (_SFR_MEM_ADDR(RAMPZ)), \
    "M" (_SFR_MEM_ADDR(CCP)), \
    "r" ((U8)0xD8), \
    "e" (puAddr), \
    "r" (nValue) \
  : "r0" \
  )

/// define the parameter manipulation for IOCTL functions
#define PARAMU32( a )   (*((PU32)a ))
#define PARAMU16( a )   (*((PU16)a ))
#define PARAMU8( a )    (*((PU8)a ))
#define PARAMBOOL( a )  (*((BOOL*)a ))

/// define the macro to swap bytes 
#define SWAP_BYTES_16( value )  ((U16)(((value & 0xFF) << 8) | ((value & 0xFF00 ) >> 8 )))

/// define the macro for creating large numbers
#ifndef _UL_
#define _UL_(a)   (a ## ul)
#endif

/// define a round function
#define ROUND( a )      ((a)>=0 ? (long)((a)+0.5) : (long)((a)-0.5))

// enumerations ---------------------------------------------------------------

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
typedef C8 PROGMEM          FC8;
typedef U8 PROGMEM          FU8;
typedef S16 PROGMEM	 	      FS16;
typedef U16 PROGMEM  		    FU16;
typedef S32 PROGMEM		      FS32;
typedef U32 PROGMEM		      FU32;
typedef S64 PROGMEM		      FS64;
typedef U64 PROGMEM 		    FU64;
typedef PC8 PROGMEM         PFC8;
typedef PU8 PROGMEM         PFU8;
typedef const char*         PCC8;

// define the value	union
typedef	union
{
  U16	wValue;
  S16 iValue;
  U8	anValue[ 2 ];
} U16UN, *PU16UN;

typedef	union
{
  FLOAT	fValue;
  U32		uValue;
  S32   lValue;
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

/**@} EOF Types.h */

#endif  // _TYPES_H

