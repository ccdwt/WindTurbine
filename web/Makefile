include ../make.inc

install: httpd files

files: Web/
	@cd Web/; cp -r * $(HTML_DIR)

httpd: disable_default_server lighttpd.conf
	@echo -n "Checking for web server .. "
	@if [ "x`opkg list-installed | grep lighttpd`" != "x" ];\
	       	then echo "Found.";\
		else opkg install lighttpd; fi
	@$(CP) lighttpd.conf /etc/lighttpd.conf
	@/etc/init.d/lighttpd restart

lighttpd.conf: lighttpd.m4
	@m4 $(HTML_DEF) lighttpd.m4 > lighttpd.conf

disable_default_server: disable_bonescript disable_cloud9
	systemctl stop bonescript.service
	systemctl stop bonescript-autorun.service
	systemctl stop cloud9.service
	systemctl stop bonescript.socket

disable_bonescript:
	@echo -n "Checking if bonscript server is up .. "
	@if [ "x`systemctl list-units |grep bonescript`" == "x" ]; then\
		echo "disabled."; else\
		echo " removing .. ";\
		systemctl disable bonescript.service;\
		systemctl disable bonescript-autorun.service;\
		systemctl disable bonescript.socket;\
		echo "removed."; fi
disable_cloud9:
	@echo -n "checking for cloud9 service .. "
	@if [ "x`systemctl list-units |grep bonescript`" == "x" ]; then\
		echo "disabled."; else\
		echo "removing .. ";\
		systemctl disable cloud9.service;\
		echo "removed."; fi

clean: 
	rm -f *~ *.swp lighttpd.conf 
