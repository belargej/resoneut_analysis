CXX=g++

SUB_DIRS=RNCore RNAnalyzers RNBatchMode RNSimRun RNFilters RNFitting plugins/sak

.PHONY: ALL_SUBDIRS clean newdict

all: ALL_SUBDIRS

ALL_SUBDIRS:	
	-for d in ${SUB_DIRS}; do (cd $$d;$(MAKE)); done	

clean: 
	-for d in ${SUB_DIRS}; do (cd $$d;$(MAKE) clean); done

newdict: 
	-for d in ${SUB_DIRS}; do (cd $$d;$(MAKE) newdict); done

help:
	@echo "Optimization Flags:\n \
	Make OPTIMIZE=01 : -O1 flag\n \
	Make OPTIMIZE=02 : -O2 flag\n  "
	@echo "Otherwise, debug flags are added -g -Wall\n"

