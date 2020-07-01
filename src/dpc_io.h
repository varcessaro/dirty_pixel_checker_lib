#ifndef dpc_io_h
#define dpc_io_h

#include <stdio.h>
#include <arpa/inet.h>
#include "dpc_num.h"

typedef FILE * dpc_file;

/**
 * Reads a byte from the dpc_file.
 *
 * @return TRUE if the byte is read from the file, otherwise FALSE.
 */
dpc_bool dpc_read_byte(dpc_file f, dpc_byte * r);

/**
 * Writes a byte to the dpc_file.
 *
 * @return TRUE if the byte is written to the file, otherwise FALSE.
 */
dpc_bool dpc_write_byte(dpc_file f, dpc_byte v);

/**
 * Reads a word from the dpc_file and converts it to the appropriate endianness.
 *
 * @return TRUE if the word is read from the file, otherwise FALSE.
 */
dpc_bool dpc_read_short(dpc_file f, dpc_short * r);

/**
 * Converts the word to the appropriate endianness and writes it to the dpc_file.
 *
 * @return TRUE if the word is written to the file, otherwise FALSE.
 */
dpc_bool dpc_write_short(dpc_file f, dpc_short v);

/**
 * Reads an int from the dpc_file and converts it to the appropriate endianness.
 *
 * @return TRUE if the int is read from the file, otherwise FALSE.
 */
dpc_bool dpc_read_int(dpc_file f, dpc_int * r);

/**
 * Converts the int to the appropriate endianness and writes it to the dpc_file.
 *
 * @return TRUE if the int is written to the file, otherwise FALSE.
 */
dpc_bool dpc_write_int(dpc_file f, dpc_int v);

#endif /* dpc_io_h */
