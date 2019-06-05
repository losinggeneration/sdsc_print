/*
 * Copyright 2019 Harley Laue <losinggeneration@gmail.com>
 */
#include <stdio.h>
#include <string.h>

#include "utils.h"

/* SDCC port definitions */
__sfr __at 0x3E io_mem_ctrl;
__sfr __at 0xFD sdsc_write;

void sdsc_print(const char *fmt) {
	/*
	 * write 0100b to the I/O control portwhich is to disable the joystick
	 * ports by setting bit 2
	 */
	io_mem_ctrl = 4;

	for(int x = 0; fmt[x] != '\0'; ++x) {
		/* write to the SDSC port */
		sdsc_write = fmt[x];
	}

	/*
	 * write 0 to the I/O control portwhich is to disable the joystick ports by
	 * clearing bit 2
	 */
	io_mem_ctrl = 0;
}

void sdsc_printf(const char *fmt, ...) {
	va_list arg;
	char str[80*25];
	va_start(arg, fmt);
	vsprintf(str, fmt, arg);

	sdsc_print(str);
}

