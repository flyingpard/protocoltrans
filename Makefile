CC = gcc
CFLAGS = -c -g
OBJS = hl7_con.o hl7_api.o iniparser.o dictionary.o decode.o
SRCPATH = src/
EXECUTABLE = hl7_con

default: build

build: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CC) -o $@ $^
	@(chmod ugo=rx,u+w $@)

hl7_api.o: $(SRCPATH)hl7_api.c $(SRCPATH)hl7_api.h
	$(CC) $(CFLAGS) $(SRCPATH)hl7_api.c

hl7_con.o: $(SRCPATH)hl7_con.c $(SRCPATH)hl7_api.h $(SRCPATH)hl7_con.h $(SRCPATH)iniparser.h $(SRCPATH)decode.h
	$(CC) $(CFLAGS) $(SRCPATH)hl7_con.c

decode.o: $(SRCPATH)decode.c $(SRCPATH)decode.h $(SRCPATH)hl7_con.h
	$(CC) $(CFLAGS) $(SRCPATH)decode.c

iniparser.o: $(SRCPATH)iniparser.c $(SRCPATH)dictionary.h $(SRCPATH)iniparser.h 
	$(CC) $(CFLAGS) $(SRCPATH)iniparser.c 

dictionary.o: $(SRCPATH)dictionary.c $(SRCPATH)dictionary.h
	$(CC) $(CFLAGS) $(SRCPATH)dictionary.c

clean:
	rm -f *.o
	rm -f hl7_com
	rm -f *.hl7