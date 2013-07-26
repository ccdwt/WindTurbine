all: write

write: write_toDB.c
	gcc -o write write_toDB.c -lsqlite3

debug: write_toDB.c
	gcc -o debug write_toDB.c -lsqlite3 -D _DEBUG

clean: 
	rm -f *.swp write *.o *~ 

