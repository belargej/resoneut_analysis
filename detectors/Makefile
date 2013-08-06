CXX=g++
SOURCES=RN_BaseDetector.cpp RN_NeutDetectorArray.cpp RN_IonChamber.cpp\
	 RN_SiliconDetectors.cpp RN_Timing.cpp 

HEADERS=${SOURCES:.cpp=.hpp}

OBJECTS=${SOURCES:.cpp=.o}
LIBS=${addprefix -l,${SOURCES:.cpp=}}
OPTIONS=-lMinuit


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
LFLAGS=-L${LINK_DIR} ${GLIBS} ${OPTIONS} ${DEBUG}
INC_DIR=-I$(PWD)/../ -I$(PWD)/../sak -I$(PWD) -I${ROOTSYS}/include 


all: libRNdetectors.so	



RNdetectors_d.cpp: ${HEADERS} my_LinkDef.h
	rootcint RNdetectors_d.cpp -c $(INC_DIR) `root-config --cflags` -p $^

%.o: %.cpp
	${CXX} ${CFLAGS} $< $(INC_DIR)

libRNdetectors.so: ${OBJECTS} RNdetectors_d.cpp 
	 g++ -shared -o ${addprefix ../lib/,$@} $^ `root-config --ldflags --libs --cflags`${DEBUG} $(INC_DIR)


#####################################################################
###Other Functions###################################################
#####################################################################

clean: 
	rm  ${OBJECTS} RNdetectors_d.*  ../lib/libRNdetectors.so

newdict:
	rm RNdetectors_d.*

install:
	ln -s ${PWD}/../lib/libRNdetectors.so ${HOME}/lib

uninstall:	
	rm ${HOME}/lib/libRNdetectors.so


help:
	@echo "Optimization Flags:\n \
	Make OPTIMIZE=01 : -O1 flag\n \
	Make OPTIMIZE=02 : -O2 flag\n  "
	@echo "Otherwise, debug flags are added -g -Wall\n"
	@echo "without doing Make clean it is necessary\n \
	to do make newdict to delete the root dictionary files before \n \
	recompiling and creating the new shared library\n"