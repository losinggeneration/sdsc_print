/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <getopt.h>

FILE *sms_open(char *filename) {
	FILE *f = fopen(filename, "r+b");
	int size;

	if(!f) {
		fprintf(stderr, "Cannot open %s\n", filename);
		fclose(f);
		exit(-1);
	}

	fseek(f, 0, SEEK_END);
	size = ftell(f);

	if(size < 0x8000) {
		fprintf(stderr, "Probably not an SMS/GG ROM. It's too small.\n");
		fclose(f);
		exit(-2);
	}

	return f;
}

uint16_t sms_checksum(FILE *f) {
	/* The Z80 code used by the bios to check the checksump
	 * Checksum:
	 *   ld     a,e          ; add (hl) to de
	 *   add    (hl)
	 *   ld     e,a
	 *   ld     a,d
	 *   adc    $00
	 *   ld     d,a
	 *   inc    hl           ; move pointer on and decrement counter
	 *   dec    bc
	 *   ld     a,b          ; repeat until counter is zero
	 *   or     c
	 *   jr     nz,Checksum
	 *   ret
	 *
	 * It sums bc bytes from offset hl into de. This is used to checksum
	 * different regions of ROM according to the ROM header. A lower region is
	 * checked, intended to cover all of the ROM before the header (last 16
	 * bytes); an upper region is then checked (if applicable) assuming a
	 * standard Sega mapper.
	 */

	int x;
	uint8_t byte;
	uint16_t checksum = 0;

	/* for now we mearly check the lower 32k */
	fseek(f, 0, 0);

	for(x = 0; x < 0x7ff0; ++x) {
		byte = fgetc(f);
		if(byte == EOF) {
			fprintf(stderr, "prematurely hit EOF\n");
			fclose(f);
			exit(-3);
		}
		checksum += byte;
	}

	/* The checksum is two bytes, little endian. So we need to swap the two bytes */
	byte = checksum&0x00ff; /* mask off the first byte */
	checksum >>= 8; /* Move the high byte to the low byte */
	checksum |= byte << 8; /* insert the former low byte into the high byte */

	return checksum;
}

int sms_check(char *filename) {
	FILE *f;
	uint8_t header[16], sega[9], checksum[2], product[3], region;
	uint16_t calculated_checksum, header_checksum = 0;
	int x;

	f = sms_open(filename);

	calculated_checksum = sms_checksum(f);

	fseek(f, 0x7ff0, 0);

	// The header is 16 bytes
	for(x = 0; x < 16; ++x) {
		header[x] = fgetc(f);
		if(header[x] == EOF) {
			fprintf(stderr, "failed to read header: EOF");
			fclose(f);
			exit(-3);
		}
	}

	fclose(f);

	// Check TMR SEGA
	for(x = 0; x < 8; ++x) {
		sega[x] = header[x];
	}
	if(strncmp(sega, "TMR SEGA", 8) != 0) {
		fprintf(stderr, "Sega trademark: FAILED\n");
	}
	else {
		printf("Sega trademark: OK\n");
	}

	// checksum
	for(x = 0; x < 2; ++x) {
		checksum[x] = header[x+10];
	}
	header_checksum = checksum[0] << 8 | checksum[1];

	if(header_checksum == calculated_checksum) {
		printf("checksum: OK\n");
	}
	else {
		fprintf(stderr, "checksum: FAILED\n");
	}

	// product code
	printf("product code: 0x");
	for(x = 0; x < 3; ++x) {
		product[x] = header[x+12];
		printf("%x", product[x]);
	}
	printf("\n");

	// Region code and ROM size
	region = header[15];
	printf("region: 0x%02x\n", region);

	return 0;
}

int sms_change(char *filename, uint8_t *product_code, uint8_t *version, uint8_t *region, uint8_t *size) {
	FILE *f;
	uint8_t sega[9] = "TMR SEGA", rom_checksum[2], rom_product[3], rom_region;
	uint16_t calculated_checksum, header_checksum = 0;
	int x;

	f = sms_open(filename);

	calculated_checksum = sms_checksum(f);

	rom_checksum[0] = (calculated_checksum & 0xFF00) >> 8;
	rom_checksum[1] = (calculated_checksum & 0x00FF);

	/* go to the rom header */
	fseek(f, 0x7ff0, 0);

	/* Trademark */
	for(x = 0; x < 8; ++x) {
		fputc((int)sega[x], f);
	}
	printf("Wrote 'TMR SEGA'\n");

	fgetc(f); fgetc(f);

	/* checksum */
	for(x = 0; x < 2; ++x) {
		fputc((int)rom_checksum[x], f);
	}
	printf("Wrote 0x%x\n", calculated_checksum);

	fclose(f);
}

void usage(char *program) {
	printf("usage: \n");
	printf("%s [options]\n", program);
	printf("\t--check [-c] <filename>\n");
	printf("\t--fix [-f] <filename>\n");
	printf("\t--help [-h]\n");
}

int main(int argc, char *argv[]) {
	struct option long_opts[] = {
		{ "check", required_argument, 0, 'c' },
		{ "fix", required_argument, 0, 'f' },
		{ "product-code", required_argument, 0, 'p' },
		{ "product-version", required_argument, 0, 'v' },
		{ "region", required_argument, 0, 'r' },
		{ "size", required_argument, 0, 's' },
		{ "help", no_argument, 0, 'h' },
		{ 0, 0, 0 ,0 }
	};

	int c;

	if(argc == 1) {
		usage(argv[0]);
		exit(1);
	}

	/* read options */
	while(1) {
		int index;
		c = getopt_long(argc, argv, "c:f:p:v:r:s:h", long_opts, &index);
		if(c == -1) {
			break;
		}

		switch (c) {
			case 'c':
				sms_check(optarg);
				break;
			case 'f':
				sms_change(optarg, NULL, NULL, NULL, NULL);
				printf("\nNow checking the header:\n");
				sms_check(optarg);
				break;
			case 'p':
				break;
			case 'v':
				break;
			case 'r':
				break;
			case 's':
				break;
			case 'h':
				usage(argv[0]);
				exit(0);
				break;
			default:
				usage(argv[0]);
				exit(1);
				break;
		}
	}

	return 0;
}

