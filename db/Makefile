include ../make.inc

all: write

write: write_toDB.c
	gcc $(CCFLAGS) $(LOG_DEF) -o write write_toDB.c -lsqlite3

debug: write_toDB.c
	gcc $(CCFLAGS) -o debug write_toDB.c -lsqlite3 -D _DEBUG

install_cron:
	@echo "Installing cron entries for write_db"
	@$(SCRIPTS)/cron_update "db_write" "* * * * * $(BIN)/db_write $(DBDIR)/test.db"

install_db: write
	@echo "Installing write_db"
	@$(CP) write $(BIN)/db_write

clean: 
	rm -f *.swp write debug *.o *~  

