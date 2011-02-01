/*
 * Copyright 2019 Harley Laue <losinggeneration@gmail.com>
 */
#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <stddef.h>

/**
 * @brief Clear the SDSC debug console
 *
 * @param mctl The current value of the mctl to not clobber what's aleady there
 * @return void
 **/
void sdsc_clear(uint8_t mctl);

/**
 * @brief Prints a message out to an SDSC debug console
 *
 * @param fmt A standard printf formatted string
 * @param  ... The options to put in the string.
 * @return void
 **/
void sdsc_printf(const char *fmt, ...);

/**
 * @brief Prints a message out to an SDSC debug console
 *
 * @param fmt A basic message. Shouldn't be larger than 80*25.
 * @return void
 **/
void sdsc_print(const char *fmt);

/**
 * @brief Prints a message out to an SDSC debug console
 *
 * @param fmt A standard printf formatted string
 * @param  n  The length of the formatted string
 * @return void
 **/
void sdsc_nprint(const char *fmt, size_t n);

#endif
