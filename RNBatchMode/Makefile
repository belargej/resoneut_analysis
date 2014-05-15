CXX=g++
EXE=RNBatchMode

OPTIONS=-lMinuit -lSpectrum

ifndef OPTIMIZE
DEBUG=-g -Wall
else
DEBUG=-O2
endif

INCLUDE_DIRS=-I${ROOTSYS}/include -I${ROOTSYS}/include/root -I../RNCore/include
CFLAGS=`root-config --cflags` -c ${DEBUG} ${INCLUDE_DIRS}
GLIBS=`root-config --glibs`
LINK_DIR=${PWD}/../lib
LFLAGS=-L${LINK_DIR} ${GLIBS} ${OPTIONS} ${DEBUG} ${INCLUDE_DIRS}

all: ${EXE}	

RNBatchMode: RN_BatchMode.o
	${CXX} -o ../bin/RNBatchMode $< ${LFLAGS} -lRNCore -fPIC
	@echo "Cleaning up \n .o files"
	rm RN_BatchMode.o

RN_BatchMode.o: src/RN_BatchMode.cpp
	${CXX} ${CFLAGS} $< -fPIC

#####################################################################
###Other Functions###################################################
#####################################################################

clean: 
	rm  ../bin/RNBatchMode ${addsuffix .o,${EXE}}

install:
	ln -s ${PWD}/../bin/RNBatchMode ${HOME}/bin

uninstall:	
	rm ${HOME}/bin/RNBatchMode

help:
	@echo "Optimization Flags:\n \
	Make OPTIMIZE=01 : -O1 flag\n \
	Make OPTIMIZE=02 : -O2 flag\n  "
	@echo "Otherwise, debug flags are added -g -Wall\n"
