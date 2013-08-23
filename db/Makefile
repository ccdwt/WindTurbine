include ../make.inc

all: write

write: write_toDB.c sqlite3 libjson
	gcc $(CCFLAGS) $(LOG_DEF) $(DB_DEF) $(JSON_DEF) -o write write_toDB.c $(shell pkg-config --libs sqlite3) $(shell pkg-config --cflags --libs json)

debug: write_toDB.c sqlite3 libjson
	gcc $(CCFLAGS) $(LOG_DEF) $(DB_DEF) -o debug write_toDB.c $(shell pkg-config --libs sqlite3) $(shell pkg-config --cflags --libs json) -D DEBUG

install_cron:
	@echo "Installing cron entries for write_db"
	@$(SCRIPTS)/cron_update "db_write" "* * * * * $(BIN)/db_write $(DBDIR)/test.db"

install_db: write
	@echo "Installing write_db"
	@$(CP) write $(BIN)/db_write
	#@echo "installing Database File"
	#@$(CP) test.db $(DBDIR)
sqlite3:
	@echo -n "checking for sqlite3 .. "
	@if pkg-config --exists sqlite3; then echo "Found!"; else \
		echo "not found. installing libsqlite3 libsqlite3-dev";\
		opkg install libsqlite3 libsqlite3-dev;\
		fi
libjson:
	@echo -n "checking for libjson .. "
	@if pkg-config --exists json; then echo "Found!"; else\
		echo "not found. installing libjson-dev";\
		opkg install libjson-dev;\
		fi
clean: 
	rm -f *.swp write debug *.o *~  

