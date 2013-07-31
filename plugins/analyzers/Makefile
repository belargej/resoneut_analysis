CXX=g++
SOURCES=PSD_Analyzer.cpp NewTree_Analyzer.cpp
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

all: libanalyzers.so	

libanalyzers.so: ${OBJECTS} analyzers_d.cpp 
	 g++ -shared -o ${addprefix ../../lib/,$@} $^ `root-config --ldflags --libs --cflags` ${DEBUG} -I$(ROOTSYS)/include -I$(PWD)/../../include -I$(PWD)/../sak

analyzers_d.cpp: ${HEADERS} my_LinkDef.h
	rootcint analyzers_d.cpp -c `root-config --cflags` -I$(PWD)/../../include -I$(PWD)/../sak -p $^

%.o: %.cpp
	${CXX} ${CFLAGS} $< -I$(PWD)/../../include -I$(PWD)/../sak


#####################################################################
###Other Functions###################################################
#####################################################################

clean: 
	rm ${OBJECTS} analyzers_d.* ../../lib/libanalyzers.so

newdict:
	rm analyzers_d.*

install:	
	ln -s ${PWD}/../../lib/libanalyzers.so ${HOME}/lib

uninstall:	
	rm ${HOME}/lib/libanalyzers.so


help:
	@echo "Optimization Flags:\n \
	Make OPTIMIZE=01 : -O1 flag\n \
	Make OPTIMIZE=02 : -O2 flag\n  "
	@echo "Otherwise, debug flags are added -g -Wall\n"
	@echo "without doing Make clean it is necessary\n \
	to do make newdict to delete the root dictionary files before \n \
	recompiling and creating the new shared library\n"
