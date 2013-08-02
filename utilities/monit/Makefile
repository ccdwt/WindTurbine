include ../../make.inc

all: make

install: make rc_file
	$(MAKE) install -C monit-5.5.1

rc_file: rc.m4
	m4 rc.m4 $(LOG_DEF) $(PID_DEF) $(DB_DEF) 
make: patch
	$(MAKE) all -C monit-5.5.1

patch: monit-5.5.1/Makefile monit-bbb.patch
	cd monit-5.5.1; patch < ../monit-bbb.patch

monit-5.5.1/Makefile: monit-5.5.1/ deps
	cd monit-5.5.1; ./configure --with-ssl-static=/usr/lib

deps:
	@echo -n "checking openssl .. "
	@if pkg-config openssl --static --exists; then echo "Found!"; else opkg install openssl-dev openssl-staticdev; fi

monit-5.5.1/: 
	tar -zxf monit-5.5.1.tar.gz

clean:
	rm -rf *~ *.swp monit-5.5.1/
