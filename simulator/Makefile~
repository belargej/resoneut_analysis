CXX=g++
SOURCES=RN_Sim.cpp RN_SimPhysics.cpp RN_SimRun.cpp
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
LINK_DIR=${PWD}/../../lib
LFLAGS=-L${LINK_DIR} ${GLIBS} ${OPTIONS} ${DEBUG} -I${ROOTSYS}/include 

all: libsimulator.so	

libsimulator.so: ${OBJECTS} simulator_d.cpp 
	 g++ -shared -o ${addprefix ../../lib/,$@} $^ `root-config --ldflags --libs --cflags` ${DEBUG} -I$(PWD)/../../include -I$(PWD)/../sak -I$(PWD)

simulator_d.cpp: ${HEADERS} my_LinkDef.h
	rootcint simulator_d.cpp -c `root-config --cflags` -I$(PWD)/../../include -I$(PWD)/../sak -I$(PWD) -p $^

%.o: %.cpp
	${CXX} ${CFLAGS} $< -I$(PWD)/../../include -I$(PWD)/../sak -I$(PWD)


#####################################################################
###Other Functions###################################################
#####################################################################

clean: 
	rm ${OBJECTS} simulator_d.* ../../lib/libsimulator.so

newdict:
	rm simulator_d.*

install:	
	ln -s ${PWD}/../../lib/libsimulator.so ${HOME}/lib

uninstall:	
	rm ${HOME}/lib/libsimulator.so


help:
	@echo "Optimization Flags:\n \
	Make OPTIMIZE=01 : -O1 flag\n \
	Make OPTIMIZE=02 : -O2 flag\n  "
	@echo "Otherwise, debug flags are added -g -Wall\n"
	@echo "without doing Make clean it is necessary\n \
	to do make newdict to delete the root dictionary files before \n \
	recompiling and creating the new shared library\n"
