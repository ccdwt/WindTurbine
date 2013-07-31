include ../make.inc

all: write

write: write_toDB.c
	gcc $(CCFLAGS) $(LOG_DEF) -o write write_toDB.c -lsqlite3

debug: write_toDB.c
	gcc $(CCFLAGS) -o debug write_toDB.c -lsqlite3 -D _DEBUG

clean: 
	rm -f *.swp write *.o *~ 

