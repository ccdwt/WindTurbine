include ../../make.inc
all: upload

install: download dropbox_python upload deps.py token_store.txt
	@echo "installing binary"
	@$(CP) upload $(BIN)/db_upload
	@$(CP) download $(BIN)/db_download
	@echo "installing token store"
	@$(CP) token_store.txt $(CONFIGDIR)/$(DB_TOKEN_FILE)
	@echo "updating cron"
	@$(SCRIPTS)/cron_update "drop_box upload" "0 * * * * $(BIN)/db_upload"
	@echo "pulling remote database file"
	@$(BIN)/db_download


upload: upload.py
	@echo "Adding configuration variables to upload"
	@$(SCRIPTS)/python_vars.sh upload.py upload "BINDIR=\"$(BIN)\"" "CONFIGDIR=\"$(CONFIGDIR)\"" "DBDIR=\"$(DBDIR)\"" "DB_NAME=\"$(DB_NAME)\"" "APP_KEY=$(DB_APP_KEY)" "APP_SECRET=$(DB_APP_SECRET)" "DB_TOKEN_FILE=\"$(DB_TOKEN_FILE)\"";\
	chmod +x upload

download: download.py
	@echo "Adding configuration variables to download"
	@$(SCRIPTS)/python_vars.sh download.py download "BINDIR=\"$(BIN)\"" "CONFIGDIR=\"$(CONFIGDIR)\"" "DBDIR=\"$(DBDIR)\"" "DB_NAME=\"$(DB_NAME)\"" "APP_KEY=$(DB_APP_KEY)" "APP_SECRET=$(DB_APP_SECRET)" "DB_TOKEN_FILE=\"$(DB_TOKEN_FILE)\"";\
	chmod +x download

dropbox_python: setup-tools
	@echo -n "Testing for DropBox api .. "
	@if python deps.py &> /dev/null; then echo "Found!";\
	       	else echo "not found. Installing DropBox API";\
	        cd dropbox-python-sdk-1.6;\
	       	python setup.py install;\
		fi

setup-tools: https-curl python deps.py
	@echo -n "Testing for Python setuptools .. "
	@if python setuptools-check.py &> /dev/null; then echo "Found!";\
		else echo "not found. Installing python setuptools.";\
		opkg install python-distutils python-compile python-compiler;\
		curl -k https://bitbucket.org/pypa/setuptools/raw/bootstrap/ez_setup.py -O;\
		. ~/.profile; python ez_setup.py; fi

https-curl: curl
	@echo -n "Testing for https support in curl .. "
	@if curl "https://www.google.com" &> /dev/null; then echo "Found!"; else\
		echo "not found. Installing https support"; \
		echo "export CURL_CA_BUNDLE=/etc/ssl/certs/ca-certificates.crt" >> ~/.profile;\
		fi

deps.py: upload.py
	@echo "generating deps.py"
	@grep import upload.py > deps.py
	@echo "from setuptools import setup" >> deps.py
	@echo "import distutils" >> deps.py
curl: 
	@echo -n "Testing for curl .. "
	@if which curl; then true ; else opkg install curl ;  fi

python:
	@echo -n "Testing for python .. "
	@if which python; then true; else opkg install python; fi

clean: 
	rm -f *.swp *~ ez_setup.py upload deps.py
