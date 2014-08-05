CXX=g++

SRCFILES= Th228FitIndividual.cpp

########Directories and Files###########################################################
SRCDIR=src
HDRDIR=include

HDRFILES=${SRCFILES:.cpp=.hpp}
OBJECTS=${SRCFILES:.cpp=.o}

SOURCES=${addprefix $(SRCDIR)/,$(SRCFILES)}
HEADERS=${addprefix $(HDRDIR)/,$(HDRFILES)}
############################################################################
OPTIONS=-lMinuit -lSpectrum
ifndef OPTIMIZE
DEBUG=-g -Wall
else 
DEBUG=-O2
endif

CFLAGS=`root-config --cflags` -c ${DEBUG}
GLIBS=`root-config --glibs`
LINK_DIR=${PWD}/../lib
LFLAGS=-L${LINK_DIR} ${GLIBS} ${OPTIONS} ${DEBUG} 
INC_DIR=-I${ROOTSYS}/include -I$(PWD)/../RNCore/include -I$(PWD)/../ -I$(HDRDIR)

all: libRNfitting.so

libRNfitting.so: ${OBJECTS} RNfitting_d.cpp 
	 g++ -shared -o ${addprefix ../lib/,$@} $^ `root-config --ldflags --libs --cflags` ${DEBUG} $(INC_DIR) -fPIC
#	@echo "cleaning up: \n .o files"  
#	rm ${OBJECTS}
	@echo "cleaning up: \n _d dictionary files"  	
	rm RNfitting_d.*



RNfitting_d.cpp: ${HEADERS} $(HDRDIR)/my_LinkDef.h
	rootcint RNfitting_d.cpp -c $(INC_DIR) `root-config --cflags` -p $^

%.o: ${addprefix $(SRCDIR)/,%.cpp} ${addprefix $(HDRDIR)/,%.hpp}	
	${CXX} ${CFLAGS} $< $(INC_DIR) -fPIC

 #####################################################################
###Other Functions###################################################
#####################################################################

clean: 
	rm ../lib/libRNfitting.so RNfitting_d.* ${OBJECTS}

newdict:
	rm RNfitting_d.*

install:	
	ln -s ${PWD}/../lib/libRNfitting.so ${HOME}/lib

uninstall:	
	rm ${HOME}/lib/libRNfitting.so


help:
	@echo "Optimization Flags:\n \
	Make OPTIMIZE : -O2 flag\n  "
	@echo "Otherwise, debug flags are added -g -Wall\n"
