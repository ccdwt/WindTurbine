all: build

build: weather turbine 
#modbus

clean: weather/clean turbine/clean
######### weather/ #####################
weather: weather/client
	
weather/client:
	+$(MAKE) -C weather

weather/clean:
	+$(MAKE) clean -C weather
					
######## turbine/ ######################

turbine: turbine/turbine turbine/logger

turbine/turbine:
	+$(MAKE) -C turbine

turbine/logger:
	+$(MAKE) logger -C turbine

turbine/clean:
	+$(MAKE) clean -C turbine
######## modbus/ #######################
