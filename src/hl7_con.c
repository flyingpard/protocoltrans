/* hl7_con.c
 *
 * This file is a part of healthcare device protocol translate program.
 * 
 * In this part, the program read several config files and put proper
 * information into a HL7MSG struct which is defined by HL7ImEx and
 * write the coded message to the file named ORU.hl7.
 *
 * Author: Wu Haochen
 * Email: wuhaochen42 AT gmail DOT com
 * Last Edit: 2013.5.10
 */

#include "hl7_con.h"

static char* message_file_name = NULL;
static char* rule_file_name = NULL;
static char* output_file_name = NULL;

static struct option const long_options[] = {
  { "message", required_argument, NULL, 'm'},
  { "rule", required_argument, NULL, 'r'},
  { "output", required_argument, NULL, 'o'},
  { NULL,0,NULL,0 }
};

int main(int argc, char** argv) {
  decode_switches( argc, argv );
  construct_message();

  printf("convert success, result saved in %s.\n",output_file_name);
  return 0;
}

int construct_message() {
  HL7MSG *pMsg=0;
  HL7FLVR *pF;
  SIZE Len;
  FILE *fp;
  char str_test[10000];

  // read in and build tables
  pF = HL7Init( "", ".v26");
  if ( pF == NULL) {
    fprintf( stderr, "HL7Init failed!\a\n");
    exit( 0);
  }

  // Start to create new structure
  pMsg = HL7Alloca( pF);

  // Insert and fill MSH 
  HL7InsSegm(pMsg, "MSH");

  // Read general configuration
  GenConf *genConf = GetGenConf("config/genconf.ini");

  HL7PutFld(pMsg, "|", 1);
  HL7PutFld(pMsg, "^~\\&", 2);
  HL7PutFld(pMsg, genConf->str_sndapp, 3);
  HL7PutFld(pMsg, genConf->str_sndfcl, 4);
  HL7PutFld(pMsg, genConf->str_rcvapp, 5);
  HL7PutFld(pMsg, genConf->str_rcvfcl, 6);
  HL7PutFld(pMsg, genConf->str_curtime, 7);
  HL7PutFld(pMsg, "ORU", 9);
  HL7PutComp(pMsg, "R01", 9);
  HL7PutFld(pMsg, "12", 10);
  HL7PutFld(pMsg, "P", 11);
  HL7PutFld(pMsg, genConf->str_ver, 12);

  //Insert and fill PID
  HL7InsSegm(pMsg, "PID");

  //Read patient configuration
  Patient *patientinfo = GetPatientInfo("config/patient.ini");

  HL7PutFld(pMsg, patientinfo->str_PID, 1);
  HL7PutFld(pMsg, patientinfo->str_PEID, 2);
  HL7PutFld(pMsg, patientinfo->str_PIID, 3);
  HL7PutFld(pMsg, patientinfo->str_APID, 4);
  HL7PutFld(pMsg, patientinfo->str_patientname, 5);
  HL7PutFld(pMsg, patientinfo->str_mothermaidenname, 6);
  HL7PutFld(pMsg, patientinfo->str_birthdate, 7);
  HL7PutFld(pMsg, patientinfo->str_sex, 8);
  HL7PutFld(pMsg, patientinfo->str_address, 11);

  //Read date from source message
  OBR *p_curobr;

  if( message_file_name == NULL ) {
    message_file_name = "msg.msg";
  }
  if( rule_file_name == NULL ) {
    rule_file_name = "config/overall.ini";
  }
  p_curobr = Decode( message_file_name, rule_file_name);

  //Insert and fill OBR
  HL7InsSegm(pMsg, "OBR");

  HL7PutFld(pMsg, p_curobr->str_setID, 1);
  HL7PutFld(pMsg, p_curobr->str_plaOrdNum, 2);
  HL7PutFld(pMsg, p_curobr->str_filOrdNum, 3);
  HL7PutFld(pMsg, p_curobr->str_USID, 4);
  HL7PutFld(pMsg, p_curobr->str_obDate, 7);

  OBX *p_curobx = p_curobr->p_first;

  while ( p_curobx != NULL ){
    //Insert and fill OBX
    HL7InsSegm(pMsg, "OBX");

    HL7PutFld(pMsg, p_curobx->str_setID, 1);
    HL7PutFld(pMsg, p_curobx->str_valueType, 2);
    HL7PutFld(pMsg, p_curobx->str_obsID, 3);
    HL7PutFld(pMsg, p_curobx->str_obsSubID, 4);
    HL7PutFld(pMsg, p_curobx->str_value, 5);
    HL7PutFld(pMsg, p_curobx->str_unit, 6);
    HL7PutFld(pMsg, p_curobx->str_resStatus, 11);

    p_curobx = p_curobx->p_next;
  }

  //Write to file
  HL7WriteMsg(pMsg, str_test, 10000, &Len);
  if (output_file_name == NULL) {
    output_file_name = "ORU.hl7";
  }
  
  if ((fp=fopen(output_file_name,"w"))==NULL) {
    printf("cannot open file!");
    exit(0);
  }
  fputs(str_test,fp); 
  fputs("\n",fp); 
  fclose(fp); 

  //Clean the memory
  free(genConf);
  free(patientinfo);
  OBX *temp = NULL;
  p_curobx = p_curobr->p_first;
  free(p_curobr);
  while ( p_curobx != NULL ){
    temp = p_curobx;
    p_curobx = p_curobx->p_next;
    free(temp);
  }

  return 0;
}

GenConf* GetGenConf( char *filename ) {
  GenConf *genConf = (GenConf *)malloc(sizeof(GenConf));

  dictionary * conf;
  conf = iniparser_load(filename);

  char * sndapp;
  sndapp = iniparser_getstring(conf,"device:sndapp",NULL);
  genConf->str_sndapp = sndapp;

  char * sndfcl;
  sndfcl = iniparser_getstring(conf,"device:sndfcl",NULL);
  genConf->str_sndfcl = sndfcl;

  char * rcvapp;
  rcvapp = iniparser_getstring(conf,"device:rcvapp",NULL);
  genConf->str_rcvapp = rcvapp;

  char * rcvfcl;
  rcvfcl = iniparser_getstring(conf,"device:rcvfcl",NULL);
  genConf->str_rcvfcl = rcvfcl;

  static char CurTime[100];
  if ( GetCurTimeFmt(CurTime) != 0) {
    return NULL;
  }
  genConf->str_curtime = CurTime;

  char * version;
  version = iniparser_getstring(conf,"device:version","2.6");
  genConf->str_ver = version;

  return genConf;
}

Patient* GetPatientInfo( char *filename ) {
  Patient *patientinfo = (Patient *)malloc(sizeof(Patient));

  dictionary * conf;
  conf = iniparser_load(filename);

  char * pid;
  pid = iniparser_getstring(conf,"patient:pid",NULL);
  patientinfo->str_PID = pid;

  char * peid;
  peid = iniparser_getstring(conf,"patient:peid",NULL);
  patientinfo->str_PEID = peid;

  char * piid;
  piid = iniparser_getstring(conf,"patient:piid",NULL);
  patientinfo->str_PIID = piid;

  char * apid;
  apid = iniparser_getstring(conf,"patient:apid",NULL);
  patientinfo->str_APID = apid;

  char * name;
  name = iniparser_getstring(conf,"patient:name",NULL);
  patientinfo->str_patientname = name;

  char * maidenname;
  maidenname = iniparser_getstring(conf,"patient:maidenname",NULL);
  patientinfo->str_mothermaidenname = maidenname;

  char * birthdate;
  birthdate = iniparser_getstring(conf,"patient:birthdate",NULL);
  patientinfo->str_birthdate = birthdate;

  char * sex;
  sex = iniparser_getstring(conf,"patient:sex",NULL);
  patientinfo->str_sex = sex;

  char * address;
  address = iniparser_getstring(conf,"patient:address",NULL);
  patientinfo->str_address = address;

  return patientinfo;
}

//Get Formatted Current Time.
int GetCurTimeFmt(char *FmtTime) {
  time_t *TimeSec = (time_t *)malloc(sizeof(time_t));
  if ( time(TimeSec) == -1) {
    return -1;
  }
  struct tm *CurTime;
  CurTime = localtime(TimeSec);
  strftime( FmtTime, 100, "%Y%m%d%H%M%S%z", CurTime);
  free(TimeSec);
  return 0;
}

static int decode_switches( int argc, char** argv ) {
  for(;;) {
    int oi = -1;
    int c = getopt_long(argc, argv, "mro", long_options, &oi);

    if( c == -1 ) 
      break;

    switch(c) {
    case 'm':
      message_file_name = *(argv + optind);
      break;

    case 'r':
      rule_file_name = *(argv + optind);
      break;

    case 'o':
      output_file_name = *(argv + optind);
      break;
    }
  }
  return 0;
}
