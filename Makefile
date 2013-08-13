CXX=g++
OPTIONS=-lMinuit

EXE=RN_Root RN_BatchMode


CFLAGS=`root-config --cflags` -c ${DEBUG}
GLIBS=`root-config --glibs`
LINK_DIR=${PWD}/lib/
LFLAGS=-L${LINK_DIR} ${GLIBS} ${OPTIONS} ${DEBUG}
INC_DIR=-I${ROOTSYS}/include -I.

SUB_DIRS=include plugins/sak plugins/analyzers plugins/simulator

OBJLIBS=libRNanalyzers.so libSAK.so libRNsimulator.so libRNeut.so

LIBS= -lRNeut -lRNanalyzers -lSAK -lRNsimulator

all: ${EXE} ${OBJLIBS}	


RN_Root: force_look
	cd include; $(MAKE)

RN_BatchMode: force_look
	cd include; ${MAKE}

####################################################################
###Framework Dependencies###########################################
####################################################################

%.o: %.cpp
	${CXX} ${CFLAGS} $< 

libRNeut.so:force_look
	cd include; ${MAKE}


libSAK.so: force_look
	cd plugins/sak; ${MAKE}

libRNanalyzers.so: force_look
	cd plugins/analyzers; ${MAKE}

libRNsimulator.so:force_look
	cd plugins/simulator; ${MAKE}


force_look :
	true

#####################################################################
###Other Functions###################################################
#####################################################################

clean: 
	-for d in ${SUB_DIRS}; do (cd $$d;$(MAKE) clean); done

help:
	@echo "Optimization Flags:\n \
	Make OPTIMIZE=01 : -O1 flag\n \
	Make OPTIMIZE=02 : -O2 flag\n  "
	@echo "Otherwise, debug flags are added -g -Wall\n"
	@echo "without doing Make clean it is necessary\n \
	to do make newdict to delete the root dictionary files before \n \
	recompiling and creating the new shared library\n"