; Copyright 2019 Harley Laue <losinggeneration@gmail.com>

.computesmschecksum
.sdsctag 0.1, "SDSC_DEBUG", "An example of using a SDSC Debug Console for basic printing", "Harley Laue"

.memorymap
slotsize $2000
defaultslot 0
slot 0 $0000
.endme

.banksize $2000
.rombanks 1

.bank $00 SLOT 0
.org 0
    di             ; Disable interrupts during setup
    ld SP, $DFFF   ; Set the stack to the top of the RAM

    im 1  ; set interrupt mode 1 which expects the interrupt to be at
          ; 0x038

    jp start

; VDP interrupt
.org $38
    reti

; NMI
.org $66
    retn

; Main
.org $100

start:
    ld HL, message
    call sdsc_print
    jp _exit

; HL address of message. Must be NULL byte terminated.
;
; This disables the joystick ports from the I/O control
; port (0x3E) & should re-enable them after. This isn't
; really tested, so if you see the joysticks not working
; after calling this, that's why.
sdsc_print:
    push AF  ; save registers we'll be modifying

    ld A, $4     ; write 0100b to the I/O control port
    out ($3E), A ; which is to disable the joystick ports
                 ; by setting bit 2

    call write_string

    ld A, $0       ; write 0 to the I/O control port
    out ($3E), A   ; which is to re-enable the joystick ports
                   ; by clearing bit 2

    pop AF  ; restore registers
    ret

; HL address of message. Must be NULL byte terminated.
; A is overwritten for this
write_string:
-   ld A, (HL)     ; load the value at the address in HL
    cp 0           ; if the character is zero,
    ret Z          ; we're done

    out ($FD), A   ; write that character
    inc HL         ; move to the next character
    jr -           ; and loop until we're out of characters

_exit:
    ; Exit - special code to the emulator
    ld  A, 0
    rst $8
-   halt
    jr -

message:
    .db "Welcome to SDSC print debugging."
    .db $A ; \n
    .db 0  ; null byte to terminate the string

; vim: set ts=4 sts=4 sw=4 et ft=z8a:
