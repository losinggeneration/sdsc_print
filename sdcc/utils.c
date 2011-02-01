/*
 * Copyright 2019 Harley Laue <losinggeneration@gmail.com>
 */
#include <stdio.h>
#include <string.h>

#include "utils.h"

__sfr __at 0x3E io_mem_ctrl;
__sfr __at 0xFD sdsc_write;
__sfr __at 0xFC sdsc_ctrl;

void sdsc_clear(uint8_t mctl) {
	mctl |= 0x4;
	io_mem_ctrl = mctl;
	sdsc_ctrl = 2;
	mctl ^= 0x4;
	io_mem_ctrl = mctl;
}

void sdsc_nprint(const char *fmt, size_t n) {
	io_mem_ctrl = 0xC;

	int x;
	for(x = 0; x < n; ++x) {
		sdsc_write = fmt[x];
	}

	io_mem_ctrl = 0x8;
}

void sdsc_print(const char *fmt) {
	sdsc_nprint(fmt, strlen(fmt));
}

void sdsc_printf(const char *fmt, ...) {
	va_list arg;
	char str[80*25];
	va_start(arg, fmt);
	vsprintf(str, fmt, arg);

	sdsc_print(str);
}

