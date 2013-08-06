CXX=g++
SOURCES=RN_Calibrator.cpp
OBJECTS=${SOURCES:.cpp=.o}
HEADERS=${SOURCES:.cpp=.hpp}
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


all: libRNcalibrator.so	

libRNcalibrator.so: ${OBJECTS} RNcalibrator_d.cpp 
	 g++ -shared -o ${addprefix ../lib/,$@} $^ `root-config --ldflags --libs --cflags` ${DEBUG} $(INC_DIR)

RNcalibrator_d.cpp: ${HEADERS} my_LinkDef.h
	rootcint RNcalibrator_d.cpp -c $(INC_DIR) `root-config --cflags` -p $^

%.o: %.cpp
	${CXX} ${CFLAGS} $< $(INC_DIR)


#####################################################################
###Other Functions###################################################
#####################################################################

clean: 
	rm ${OBJECTS} RNcalibrator_d.* ../lib/libRNcalibrator.so

newdict:
	rm RNcalibrator_d.*

install:	
	ln -s ${PWD}/../lib/libRNcalibrator.so ${HOME}/lib

uninstall:	
	rm ${HOME}/lib/libRNcalibrator.so


help:
	@echo "Optimization Flags:\n \
	Make OPTIMIZE=01 : -O1 flag\n \
	Make OPTIMIZE=02 : -O2 flag\n  "
	@echo "Otherwise, debug flags are added -g -Wall\n"
	@echo "without doing Make clean it is necessary\n \
	to do make newdict to delete the root dictionary files before \n \
	recompiling and creating the new shared library\n"
