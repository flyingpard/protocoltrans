/* pttype.h
 *
 * This file is a part of healthcare device protocol translate program.
 *
 * Author: Wu Haochen
 * Email: wuhaochen42 AT gmail DOT com
 * Last Edit: 2013.6.3
 */

#ifndef PTTYPE_H
#define PTTYPE_H

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

//Node struct of linked table
typedef struct obx{
  char *str_setID;
  char *str_valueType;
  char *str_obsID;
  char *str_obsSubID;
  char *str_value;
  char *str_unit;
  char *str_resStatus;
  struct obx * p_next;   //link to next node.
} OBX;

//Head struct of linked table
typedef struct obr{
  char *str_setID;
  char *str_plaOrdNum;
  char *str_filOrdNum;
  char *str_USID;
  char *str_obDate;
  struct obx * p_first;  //link to first node.
} OBR;

#endif
