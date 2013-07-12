CP=cp -u
SYM=ln -f --backup=none -s 
install: install_startup install_config

install_startup: uart1 install_uart
	@echo "coppying startup file"
	@+$(CP) uart1 /etc/init.d/uart1
	@echo "adding to runlevels 3,4,5"
	@+$(SYM) /etc/init.d/uart1 /etc/rc3.d/S89uart1
	@+$(SYM) /etc/init.d/uart1 /etc/rc4.d/S89uart1
	@+$(SYM) /etc/init.d/uart1 /etc/rc5.d/S89uart1

install_uart: enable-uart1-ooAo.dtbo
	+$(CP) enable-uart1-ooAo.dtbo /lib/firmware/

enable-uart1-ooAo.dtbo: enable-uart1.dts
	dtc -O dtb -o enable-uart1-ooAo.dtbo -b 0 -@ enable-uart1.dts

install_config: hosts
	@echo "installing the hosts file (local dns)"
	+$(CP) hosts /etc/hosts

clean:
	rm -f  *~ *.swp *.dtbo