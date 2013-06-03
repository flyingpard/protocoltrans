/* decode.h
 *
 * This file is a part of healthcare device protocol translate program.
 *
 * Author: Wu Haochen
 * Email: wuhaochen42 AT gmail DOT com
 * Last Edit: 2013.5.12
 */

#ifndef DECODE_H
#define DECODE_H

#include <stdlib.h>
#include "pttype.h"

#define MAXLENGTH 2000
#define IDLENGTH 50

OBR * GetOBR();

/**
   @param   msgfile   Source message name.
   @param   rulefile  Overall rule file name.
   @return  pointer to statically allocated struct OBR.

   This function returns the head pointer of the linked table built by the message file and rule file.
 */
OBR * Decode(const char * msgfile, const char * rulefile);

void setOBR(OBR * obr, unsigned char* msgbuf, dictionary *specrule, const char* secname);

OBX * getOBX(unsigned char* msgbuf, dictionary *specrule, const char * secname, int setID);

int readint(unsigned char * start, int size, int isLittleEndian);

/**
   @param   sec     Section name.
   @param   key     Key name.
   @return  pointer to statically allocated character string.

   This function combines section name and key name into a "section:key" like query string.
 */
char * format_combined_key(const char * sec, const char * key);

/**
   @param    d       Dictionary to search.
   @param    sec     Section name.
   @param    key     Key name.
   @param    def     Default value to return if key not found.
   @return   pointer to statically allocated character string.

   This function queries a dictionary for a section and a key.
   the pointer passed as 'def' is returned.
   The returned char pointer is pointing to a string allocated in
   the dictionary, do not free or modify it.
 */
char * ini_getstring(dictionary * d, const char *sec, const char *key, char *def);

/**
   @param    d       Dictionary to search.
   @param    sec     Section name.
   @param    key     Key name.
   @param    def     Default value to return if key not found.
   @return   integer

   This function queries a dictionary for a section and a key.
*/
int ini_getint(dictionary * d, const char *sec, const char *key, int def);

#endif
