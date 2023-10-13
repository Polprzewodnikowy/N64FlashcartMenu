/* 
 * File:   types.h
 * Author: KRIK
 *
 * Created on 16 јпрель 2011 г., 2:24
 */

#include <stdint.h>

#ifndef _TYPES_H
#define	_TYPES_H

#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned long
#define u64 unsigned long long

#define vu8 volatile unsigned char
#define vu16 volatile unsigned short
#define vu32 volatile unsigned long
#define vu64 volatile unsigned long long

#define s8 signed char
#define s16 short
#define s32 long
#define s64 long long


typedef volatile uint64_t sim_vu64;
typedef volatile uint64_t sim_vu64;
typedef unsigned int sim_u32;
typedef uint64_t sim_u64;


#endif	/* _TYPES_H */

