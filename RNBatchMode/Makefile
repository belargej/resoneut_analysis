CXX=g++
EXE=RNBatchMode

OPTIONS=-lMinuit -lSpectrum

ifndef OPTIMIZE
DEBUG=-g -Wall
else
DEBUG=-O2
endif

CFLAGS=`root-config --cflags` -c ${DEBUG} -I../RNCore/include
GLIBS=`root-config --glibs`
LINK_DIR=${PWD}/../lib
LFLAGS=-L${LINK_DIR} ${GLIBS} ${OPTIONS} ${DEBUG} -I${ROOTSYS}/include

all: ${EXE}	

RNBatchMode: RN_BatchMode.o
	${CXX} -o ../bin/RNBatchMode $< ${LFLAGS} -lRNCore
	@echo "Cleaning up \n .o files"
	rm RN_BatchMode.o

RN_BatchMode.o: src/RN_BatchMode.cpp
	${CXX} ${CFLAGS} $< 

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
