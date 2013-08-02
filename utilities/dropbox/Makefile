include ../../make.inc
all: upload

install: dropbox_python upload deps.py
	@echo "installing binary"
	@$(CP) upload $(BIN)
	@echo "installing token store"
	@$(CP) token_store.txt $(CONFIG)/$(DB_TOKEN_FILE)
	@echo "updating cron"
	@$(SCRIPTS)/cron_update "drop_box upload" "0 * * * * $(BIN)/upload"

upload: upload.py
	@echo "Adding configuration variables to upload"
	@$(SCRIPTS)/python_vars.sh upload.py upload "BINDIR=\"$(BIN)\"" "CONFIGDIR=\"$(CONFIGDIR)\"" "DBDIR=\"$(DBDIR)\"" "APP_KEY=$(DB_APP_KEY)" "APP_SECRET=$(DB_APP_SECRET)" "DB_TOKEN_FILE=\"$(DB_TOKEN_FILE)\"";\
	chmod +x upload

dropbox_python: setup-tools
	@echo -n "Testing for DropBox api .. "
	@if python deps.py &> /dev/null; then echo "Found!";\
	       	else echo "not found. Installing DropBox API";\
	        cd dropbox-python-sdk-1.6 python setup.py install;\
		fi

setup-tools: https-curl python deps.py
	@echo -n "Testing for Python setuptools .. "
	@if python deps.py &> /dev/null; then echo "Found!";\
		else echo "not found. Installing python setuptools.";\
		curl -k https://bitbucket.org/pypa/setuptools/raw/bootstrap/ez_setup.py -O;\
		python ez_setup.py; fi

https-curl: curl
	@echo -n "Testing for https support in curl .. "
	@if curl "https://www.google.com" &> /dev/null; then echo "Found!"; else\
		echo "not found. Installing https support"; \
		opkg install ca-certificates;\
		echo "export CURL_CA_BUNDLE=/etc/ssl/certs/ca-certificates.crt" >> ~/.profile;\
		fi

deps.py: upload.py
	@echo "generating deps.py"
	@grep import upload.py > deps.py
	@echo "from setuptools import setup" >> deps.py

curl: 
	@echo -n "Testing for curl .. "
	@if which curl; then true ; else opkg install curl ;  fi

python:
	@echo -n "Testing for python .. "
	@if which python; then true; else opkg install python; fi

clean: 
	rm -f *.swp *~ ez_setup.py upload deps.py