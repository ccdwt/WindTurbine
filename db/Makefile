include ../make.inc

all: write

write: write_toDB.c sqlite3
	gcc $(CCFLAGS) $(LOG_DEF) $(DB_DEF) -o write write_toDB.c $(shell pkg-config --libs sqlite3)

debug: write_toDB.c sqlite3
	gcc $(CCFLAGS) $(LOG_DEF) $(DB_DEF) -o debug write_toDB.c $(shell pkg-config --libs sqlite3) -D _DEBUG

install_cron:
	@echo "Installing cron entries for write_db"
	@$(SCRIPTS)/cron_update "db_write" "* * * * * $(BIN)/db_write $(DBDIR)/test.db"

install_db: write
	@echo "Installing write_db"
	@$(CP) write $(BIN)/db_write
sqlite3:
	@echo -n "checking for sqlite3 .. "
	@if pkg-config --exists sqlite3; then echo "Found!"; else \
		echo "not found. installing libsqlite3 libsqlite3-dev";\
		opkg install libsqlite3 libsqlite3-dev;\
		fi
clean: 
	rm -f *.swp write debug *.o *~  

