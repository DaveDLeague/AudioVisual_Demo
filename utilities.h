#pragma once

#define s8 char
#define s16 short
#define s32 int
#define s64 long
#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long
#define f32 float
#define f64 double

#include <stdio.h>
#define DEBUG_PRINT(...) printf(__VA_ARGS__)

u32 xorshift32(u32 *state){
	u32 x = *state;
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;
	*state = x;
	return x;
}

u64 xorshift64(u64* state){
	u64 x = *state;
	x ^= x << 13;
	x ^= x >> 7;
	x ^= x << 17;
	*state = x;
	return x;
}
