include ../../make.inc

all: make

install: make monitrc startup
	$(MAKE) install -C monit-5.5.1
	$(CP) monitrc /etc/monitrc
	chmod 700 /etc/monitrc

startup:
	@echo "coppying startup file"
	@$(CP) startup.sh /etc/init.d/monit
	@chmod +x /etc/init.d/monit
	@echo "adding to runlevels 3,4,5"
	@$(SYM) /etc/init.d/monit /etc/rc3.d/S99monit
	@$(SYM) /etc/init.d/monit /etc/rc4.d/S99monit
	@$(SYM) /etc/init.d/monit /etc/rc5.d/S99monit


monitrc: rc.m4
	m4 -D LOGDIR=$(LOGDIR) -D PIDDIR=$(PIDDIR) -D DBDIR=$(DBDIR) -D DBFILE=$(DB_NAME) rc.m4 > monitrc
	chmod 700 monitrc

make: patch
	$(MAKE) all -C monit-5.5.1

patch: monit-5.5.1/Makefile monit-bbb.patch
	cd monit-5.5.1; if patch -N --dry-run --silent < ../monit-bbb.patch &>/dev/null; \
		then patch < ../monit-bbb.patch; fi

monit-5.5.1/Makefile: monit-5.5.1/ deps
	if [ ! -e monit-5.5.1/Makefile ]; then cd monit-5.5.1; ./configure --with-ssl-static=/usr/lib; fi

deps:
	@echo -n "checking openssl .. "
	@if pkg-config openssl --static --exists; then echo "Found!"; else opkg install openssl-dev openssl-staticdev; fi

monit-5.5.1/: 
	tar -zxf monit-5.5.1.tar.gz

clean:
	rm -rf *~ *.swp monitrc monit-5.5.1/
