/* hl7_con.h
 *
 * This file is a part of healthcare device protocol translate program.
 *
 * Author: Wu Haochen
 * Email: wuhaochen42 AT gmail DOT com
 * Last Edit: 2013.5.8
 */

#ifndef HL7_CON_H
#define HL7_CON_H

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>
#include <getopt.h>

#include "pttype.h"
#include "iniparser.h"
#include "hl7_api.h"

extern OBR * Decode(const char * msgfile, const char * rulefile);

int construct_message();

GenConf* GetGenConf( char *filename );

Patient* GetPatientInfo( char *filename );

int GetCurTimeFmt(char *FmtTime);

static int decode_switches( int argc, char** argv );

#endif
