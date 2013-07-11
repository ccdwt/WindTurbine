install: install_startup install_config

install_startup: uart1 install_uart
	@echo "coppying startup file"
	@cp uart1 /etc/init.d/uart1
	@echo "adding to runlevels 3,4,5"
	@ln -s /etc/init.d/uart1 /etc/rc3.d/s89uart1
	@ln -s /etc/init.d/uart1 /etc/rc4.d/s89uart1
	@ln -s /etc/init.d/uart1 /etc/rc5.d/s89uart1

install_uart: enable-uart1-ooAo.dtbo
	cp enable-uart1-ooAo.dtbo /lib/firmware/

enable-uart1-ooAo.dtbo: enable-uart1.dts
	dtc -O dtb -o enable-uart1-ooAo.dtbo -b 0 -@ enable-uart1.dts
