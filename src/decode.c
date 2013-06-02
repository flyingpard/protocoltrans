/* decode.c
 *
 * This file is a part of healthcare device protocol translate program.
 * 
 * In this part, the program read rule files to decode the source message.
 * The result is saved in a linked table which has a head of struct OBR
 * and several nodes of struct OBX.
 *
 * Author: Wu Haochen
 * Email: wuhaochen42 AT gmail DOT com
 * Last Edit: 2013.5.12
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "iniparser.h"
#include "decode.h"

static int isLittleEndian;

OBR * Decode(const char * msgfile, const char * rulefile) {
  //Read the source message
  FILE * pmsg;
  pmsg = fopen(msgfile,"rb");

  unsigned char msgbuf[MAXLENGTH];
  fread(msgbuf,sizeof(char),MAXLENGTH,pmsg);
  fclose(pmsg);

  //Read the overall rule file
  dictionary * overallrule;
  overallrule = iniparser_load(rulefile);

  isLittleEndian = iniparser_getboolean(overallrule,"overall:littleendian",1);

  int idposition;
  idposition = iniparser_getint(overallrule,"overall:idposition",-1);

  int idsize;
  idsize = iniparser_getint(overallrule,"overall:idsize",-1);

  int id;
  id = readint(&msgbuf[idposition],idsize,isLittleEndian);
  
  char ids[IDLENGTH];
  sprintf(ids,"%d",id);

  char specrulename[IDLENGTH] = "specrules:ID";
  strcat(specrulename,ids);

  char * specrulepath;
  specrulepath = iniparser_getstring(overallrule, specrulename, NULL);

  //Read the proper specified rule
  dictionary * specrule;
  specrule = iniparser_load(specrulepath);

  iniparser_freedict(overallrule);

  //Fill the head OBR
  OBR * obr;
  obr = GetOBR();
  OBX ** nextobx = &(obr->p_first);

  //Fill the node OBX
  int secn = iniparser_getnsec(specrule);

  int i;
  int setID = 1;
  for(i=0;i<secn;i++) {
    char *secname;
    secname = iniparser_getsecname(specrule,i);
    char *position;
    position = ini_getstring(specrule, secname, "position", NULL);
    //illegal section
    if(position == NULL) {
      continue;
    }
    else if(strcmp(position,"OBR") == 0) {
      setOBR(obr, msgbuf, specrule, secname);
    }
    else if(strcmp(position,"OBX") == 0) {
      OBX * curobx = getOBX(msgbuf, specrule, secname, setID);
      *nextobx = curobx;
      nextobx = &(curobx->p_next);
      setID++;
    }
  }

  //Set the end.
  *nextobx = NULL;

  return obr;
}

void setOBR(OBR * obr, unsigned char* msgbuf, dictionary *specrule, const char* secname) {
  //TODO:write this function.
}

OBX * getOBX(unsigned char* msgbuf, dictionary *specrule, const char * secname, int setID) {
  OBX * curobx = (OBX*)malloc(sizeof(OBX));

  char * setids = (char*)malloc(sizeof(char[IDLENGTH]));
  sprintf(setids, "%d", setID);
  curobx->str_setID = setids;

  int startpoint;
  startpoint = ini_getint(specrule, secname, "startpoint", -1);
  int size;
  size = ini_getint(specrule, secname, "size", -1);
  int value;
  value = readint(&msgbuf[startpoint],size,isLittleEndian);
  char *values = (char*)malloc(sizeof(char[IDLENGTH]));
  sprintf(values, "%d", value);
  curobx->str_value = values;

  char * valuetype;
  valuetype = ini_getstring(specrule, secname, "valuetype", NULL);
  curobx->str_valueType = valuetype;

  char * obsid;
  obsid = ini_getstring(specrule, secname, "obsid", NULL);
  curobx->str_obsID = obsid;

  char * obssubid;
  obssubid = ini_getstring(specrule, secname, "obssubid", NULL);
  curobx->str_obsSubID = obssubid;

  char * unit;
  unit = ini_getstring(specrule, secname, "unit", NULL);
  curobx->str_unit = unit;

  char * resstatus;
  resstatus = ini_getstring(specrule, secname, "resstatus", NULL);
  curobx->str_resStatus = resstatus;

  return curobx;
}

char * format_combined_key(const char * sec, const char * key) {
  char * combined ;
  int length ;
  
  //Calculate combined key's length,+1 for the ":".
  length = strlen(sec) + strlen(key) + 1;
  combined = (char*)malloc(length*sizeof(char));

  strcpy(combined,sec);
  strcat(combined,":");
  strcat(combined,key);

  return combined;
}

char * ini_getstring(dictionary * d, const char * sec, const char * key, char * def) {
  char * combined;
  char * sval;

  combined = format_combined_key(sec,key) ;
  sval = iniparser_getstring(d,combined,def) ;

  free(combined);
  return sval;
}

int ini_getint(dictionary * d, const char *sec, const char *key, int def) {
  char * combined;
  int sval;

  combined = format_combined_key(sec,key) ;
  sval = iniparser_getint(d,combined,def) ;

  free(combined);
  return sval;
}

int readint(unsigned char * start, int size, int isLittleEndian) {
  int result = 0;
  int i;
  if(isLittleEndian) {
    start += size;
    for(i=0;i<size;i++) {
      start--;
      result <<= 8;
      result += (int)*start;
    }
  }
  else {
    for(i=0;i<size;i++) {
      result <<= 8;
      result += (int)*start;
      start++;
    }
  }
  return result;
}

OBR* GetOBR(){
  OBR *curobx = (OBR*)malloc(sizeof(OBR));
  static char setID[] = "1";
  curobx->str_setID = setID;
  static char plaordnum[] = "080019FFFF4F6AC020121119162649^IntegrateGateway^080019FFFF4F6AC^EUI-64";
  curobx->str_plaOrdNum = plaordnum;
  static char filordnum[] = "080019FFFF4F6AC020121119162649^IntegrateGateway^080019FFFF4F6AC^EUI-64";
  curobx->str_filOrdNum = filordnum;
  static char usid[] = "080019FFFF4F6AC^monitoring of patient^EUI-64";
  curobx->str_USID = usid;
  static char obdate[] = "20121119162649+0800";
  curobx->str_obDate = obdate;
  curobx->p_first = NULL;
  return curobx;
}
