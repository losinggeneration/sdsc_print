;--------------------------------------------------------------------------
;  crt0.s - crt0.s for a Z80 based Sega Master System
;
;  Copyright (C) 2000, Michael Hope
;  Copyright (C) 2019, Harley Laue
;
;  This library is free software; you can redistribute it and/or modify it
;  under the terms of the GNU General Public License as published by the
;  Free Software Foundation; either version 2.1, or (at your option) any
;  later version.
;
;  This library is distributed in the hope that it will be useful,
;  but WITHOUT ANY WARRANTY; without even the implied warranty of
;  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
;  GNU General Public License for more details.
;
;  You should have received a copy of the GNU General Public License
;  along with this library; see the file COPYING. If not, write to the
;  Free Software Foundation, 51 Franklin Street, Fifth Floor, Boston,
;   MA 02110-1301, USA.
;
;  As a special exception, if you link this library with other files,
;  some of which are compiled with SDCC, to produce an executable,
;  this library does not by itself cause the resulting executable to
;  be covered by the GNU General Public License. This exception does
;  not however invalidate any other reasons why the executable file
;   might be covered by the GNU General Public License.
;--------------------------------------------------------------------------

	.module crt0
	.globl _main

	.area _HEADER (ABS)
	;; Reset vector
	.org 0
	jp init

	;; V-Sync interrupt
	.org 0x38
	ei
	reti

	;; Our NMI that happens on the SMS pause button being pressed
	.org 0x66
	ei
	retn

	.org 0x100
init:
	di    ; disable interrupts during initialization

	;; Stack at the top of memory.
	ld sp, #0xDFF0

	;; Initialise global variables
	call _main
	jp   _exit

__clock::
	ld  a, #2
	rst 0x08
	ret

_exit::
	;; Exit - special code to the emulator
	ld  a,#0
	rst 0x08
1$:
	halt
	jr 1$

; vim: set ts=4 sts=4 sw=4 et ft=z8a:
