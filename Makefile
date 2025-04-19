#==============================================================================
# DIRECTORIES
#==============================================================================

### Sub-Makefiles directories
DIR_APP := app/
DIR_LIB := app/lib/bcgv_api/

#==============================================================================

default : all

###Verbosity
VERBOSE := $(or $(v), $(verbose))
ifeq ($(VERBOSE),)
	Q := @
else
	Q := 
endif

#-------------------------------------------------
# Makefile's clean rules
#-------------------------------------------------
.PHONY: clean
clean:
	@echo "===== Clean App & Lib ======="
	$(Q)$(MAKE) -C $(DIR_APP) clean
	$(Q)$(MAKE) -C $(DIR_LIB) clean
	@echo "=============================="

.PHONY: cleanlib
cleanlib:
	@echo "===== Clean Lib =============="
	$(Q)$(MAKE) -C $(DIR_LIB) clean
	@echo "=============================="

.PHONY: cleanapp
cleanapp:
	@echo "===== Clean App =============="
	$(Q)$(MAKE) -C $(DIR_APP) clean
	@echo "=============================="

#-------------------------------------------------
# App Makefile
#-------------------------------------------------
.PHONY: app
app:
	@echo "===== Build App =============="
	$(Q)$(MAKE) -C $(DIR_APP)
	@echo "=============================="

#-------------------------------------------------
# Lib Makefile
#-------------------------------------------------
.PHONY: lib
lib:
	@echo "===== Build Lib =============="
	$(Q)$(MAKE) -C $(DIR_LIB)
	@echo "=============================="

#-------------------------------------------------
# App and Lib Makefile
#-------------------------------------------------

.PHONY: all
all: lib app

#-------------------------------------------------
# Project informations
#-------------------------------------------------
.PHONY: info
info:
	@echo 'Directories'
	@echo '-- APP: $(DIR_APP)'
	@echo '-- LIB: $(DIR_LIB)'
