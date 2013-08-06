CXX=g++
OPTIONS=-lMinuit

EXE=RN_Root RN_BatchMode

ifndef OPTIMIZE
DEBUG=-g -Wall
else ifeq (${OPTIMIZE},01)
DEBUG=-O1
else ifeq (${OPTIMIZE},02)
DEBUG=-O2
else ifeq (${OPTIMIZE},03)
DEBUG=-O3
endif

CFLAGS=`root-config --cflags` -c ${DEBUG}
GLIBS=`root-config --glibs`
LINK_DIR=${PWD}/lib/
LFLAGS=-L${LINK_DIR} ${GLIBS} ${OPTIONS} ${DEBUG} -I${ROOTSYS}/include

DIRS=detectors calibrator core sak analyzers simulator unpacker

OBJLIBS=libRNanalyzers.so libSAK.so libRNsimulator.so \
	libRNcore.so libRNdetectors.so libRNunpacker.so\
	libRNcalibrator.so

LIBS= -lRNunpacker -lRNanalyzers -lSAK -lRNsimulator -lRNcore \
	-lRNcalibrator -lRNdetectors

all: ${EXE}	


RN_Root: RN_Root.o ${OBJLIBS}
	${CXX} -o bin/RNRoot $< ${LFLAGS} ${LIBS}

RN_BatchMode: force_look
	cd unpacker; ${MAKE}

####################################################################
###Framework Dependencies###########################################
####################################################################

%.o: %.cpp
	${CXX} ${CFLAGS} $< 

libRNcore.so:force_look
	cd core; ${MAKE}

libSAK.so: force_look
	cd sak; ${MAKE}

libRNanalyzers.so: force_look
	cd analyzers; ${MAKE}

libRNsimulator.so:force_look
	cd simulator; ${MAKE}

libRNdetectors.so:force_look
	cd detectors; ${MAKE}

libRNcalibrator.so:force_look
	cd calibrator; ${MAKE}

libRNunpacker.so:force_look
	cd unpacker; ${MAKE}


force_look :
	true

#####################################################################
###Other Functions###################################################
#####################################################################

clean: 
	-for d in ${DIRS}; do (cd $$d;$(MAKE) clean); done
	rm  bin/RNBatchMode bin/RNRoot RN_Root.o

help:
	@echo "Optimization Flags:\n \
	Make OPTIMIZE=01 : -O1 flag\n \
	Make OPTIMIZE=02 : -O2 flag\n  "
	@echo "Otherwise, debug flags are added -g -Wall\n"
	@echo "without doing Make clean it is necessary\n \
	to do make newdict to delete the root dictionary files before \n \
	recompiling and creating the new shared library\n"