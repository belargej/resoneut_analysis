CXX=g++

SOURCES=RNGates.cpp RNHistograms.cpp
OBJECTS=${SOURCES:.cpp=.o}
HEADERS=${SOURCES:.cpp=.hpp}
LIBS=${addprefix -l,${SOURCES:.cpp=}}


CFLAGS=`root-config --cflags` -c ${DEBUG}
GLIBS=`root-config --glibs`
LINK_DIR=${PWD}/../lib

LFLAGS=-L${LINK_DIR} ${GLIBS} ${OPTIONS} ${DEBUG} 
INC_DIR=-I${ROOTSYS}/include -I$(PWD)/../../include -I$(PWD)/../../

all: libSAK.so	

libSAK.so: ${OBJECTS} SAK_d.cpp 
	 g++ -shared -o ${addprefix ../../lib/,$@} $^ `root-config --ldflags --libs --cflags` ${DEBUG} $(INC_DIR)

SAK_d.cpp: ${HEADERS} my_LinkDef.h
	rootcint SAK_d.cpp -c $(INC_DIR) `root-config --cflags` -p $^

%.o: %.cpp	
	${CXX} ${CFLAGS} $< $(INC_DIR)

 #####################################################################
###Other Functions###################################################
#####################################################################

clean: 
	rm ${OBJECTS} SAK_d.* ../../lib/libSAK.so

newdict:
	rm SAK_d.*

install:	
	ln -s ${PWD}/../../lib/libSAK.so ${HOME}/lib

uninstall:	
	rm ${HOME}/lib/libSAK.so


help:
	@echo "Optimization Flags:\n \
	Make OPTIMIZE=01 : -O1 flag\n \
	Make OPTIMIZE=02 : -O2 flag\n  "
	@echo "Otherwise, debug flags are added -g -Wall\n"
	@echo "without doing Make clean it is necessary\n \
	to do make newdict to delete the root dictionary files before \n \
	recompiling and creating the new shared library\n"