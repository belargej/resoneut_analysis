CXX=g++
SOURCES_RN=RN_Unpack2Root.cpp RN_BaseDetector.cpp RN_NeutDetectorArray.cpp \
	RN_VariableMap.cpp RN_IonChamber.cpp RN_SiliconDetectors.cpp\
	RN_Calibrator.cpp RN_Timing.cpp RN_Analyzer.cpp

HEADERS_RN=${SOURCES_RN:.cpp=.hpp}

SOURCES_OTHER=sak_ReadBuffer.cpp
HEADERS_OTHER=${SOURCES_OTHER:.cpp=.hpp}

SOURCES=$(SOURCES_RN) $(SOURCES_OTHER)
HEADERS=$(HEADERS_RN) $(HEADERS_OTHER)

OBJECTS=${SOURCES:.cpp=.o}
LIBS=${addprefix -l,${SOURCES:.cpp=}}
OPTIONS=-lMinuit

EXE=RN_BatchMode RN_RootDebug

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
LINK_DIR=${PWD}/../lib
LFLAGS=-L${LINK_DIR} ${GLIBS} ${OPTIONS} ${DEBUG} -I${ROOTSYS}/include 
 

all: ${EXE}	


RN_RootDebug: RN_RootDebug.o libRNeut.so
	${CXX} -o ../bin/RNRootDebug $< ${OBJECTS} RN_RootDict.cpp ${LFLAGS}

RN_BatchMode: RN_BatchMode.o libRNeut.so
	${CXX} -o ../bin/RNBatchMode $< ${OBJECTS} RN_RootDict.cpp ${LFLAGS}

####################################################################
###Framework Dependencies###########################################
####################################################################
RN_BatchMode.o: RN_BatchMode.cpp
	${CXX} ${CFLAGS} $<

RN_RootDict.cpp: ${HEADERS} my_LinkDef.h
	rootcint RN_RootDict.cpp -c `root-config --cflags` -p $^

%.o: %.cpp
	${CXX} ${CFLAGS} $< 

libRNeut.so: ${OBJECTS} RN_RootDict.cpp 
	 g++ -shared -o ${addprefix ../lib/,$@} $^ `root-config --ldflags --libs --cflags`${DEBUG} -I$(ROOTSYS)/include


#####################################################################
###Other Functions###################################################
#####################################################################

clean: 
	rm  ../bin/RNBatchMode RNRootDebug ${OBJECTS} RN_RootDict.* ${addsuffix .o,${EXE}} ../lib/libRNeut.so

newdict:
	rm RN_RootDict.*

install:
	ln -s ${PWD}/../bin/RNBatchMode ${HOME}/bin
	ln -s ${PWD}/../lib/libRNeut.so ${HOME}/lib

uninstall:	
	rm ${HOME}/bin/RNBatchMode
	rm ${HOME}/lib/libRNeut.so


help:
	@echo "Optimization Flags:\n \
	Make OPTIMIZE=01 : -O1 flag\n \
	Make OPTIMIZE=02 : -O2 flag\n  "
	@echo "Otherwise, debug flags are added -g -Wall\n"
	@echo "without doing Make clean it is necessary\n \
	to do make newdict to delete the root dictionary files before \n \
	recompiling and creating the new shared library\n"