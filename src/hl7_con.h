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

#include "iniparser.h"
#include "decode.h"
#include "hl7_api.h"

//Struct to save the general config.
typedef struct genconf{
  char *str_sndapp;
  char *str_sndfcl;
  char *str_rcvapp;
  char *str_rcvfcl;
  char *str_curtime;
  char *str_ver;
} GenConf;

//Struct to save the patient info.
typedef struct patient{
  char *str_PID;
  char *str_PEID;
  char *str_PIID;
  char *str_APID;
  char *str_patientname;
  char *str_mothermaidenname;
  char *str_birthdate;
  char *str_sex;
  char *str_address;
} Patient;

GenConf* GetGenConf( char *filename );

Patient* GetPatientInfo( char *filename );

int GetCurTimeFmt(char *FmtTime);

static int decode_switches( int argc, char** argv );

#endif
