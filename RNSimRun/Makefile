CXX=g++
EXE=RN_SimRun

OPTIONS=-lMinuit -lSpectrum


ifndef OPTIMIZE
DEBUG=-g -Wall
else
DEBUG=-O2
endif

CFLAGS=`root-config --cflags` -c ${DEBUG} -I../RNCore/include -I./include
GLIBS=`root-config --glibs`
LINK_DIR=${PWD}/../lib
LFLAGS=-L${LINK_DIR} ${GLIBS} ${OPTIONS} ${DEBUG} -I${ROOTSYS}/include 
 

all: ${EXE}	

RN_SimRun: RN_SimRun.o 
	${CXX} -o ../bin/RN_SimRun $< ${LFLAGS} -lRNCore
	@echo "Cleaning up: \n .o files"
	rm RN_SimRun.o

RN_SimRun.o: src/RN_SimRun.cpp
	${CXX} ${CFLAGS} $<

clean: 
	rm  ../bin/RN_SimRun

install:
	ln -s ${PWD}/../bin/RN_SimRun ${HOME}/bin	


uninstall:	
	rm ${HOME}/bin/RN_SimRun

help:
	@echo "Optimization Flags:\n \
	Make OPTIMIZE : -O2 flag\n  "
	@echo "Otherwise, debug flags are added -g -Wall\n"
