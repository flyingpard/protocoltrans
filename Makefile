hl7_con: hl7_con.o hl7_api.o iniparser.o dictionary.o decode.o
	cc -g -o hl7_con hl7_con.o hl7_api.o iniparser.o dictionary.o decode.o
	chmod ugo=rx,u+w hl7_con

hl7_api.o: hl7_api.c hl7_api.h
	cc -c hl7_api.c

hl7_con.o: hl7_con.c hl7_api.h hl7_con.h iniparser.h decode.h
	cc -c -g hl7_con.c

decode.o: decode.c decode.h hl7_con.h
	cc -c -g decode.c

iniparser.o: iniparser.c dictionary.h iniparser.h 
	cc -c -g iniparser.c 

dictionary.o: dictionary.c dictionary.h
	cc -c -g dictionary.c