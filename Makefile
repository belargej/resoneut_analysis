CXX=g++
OPTIONS=-lMinuit

EXE=RN_Root

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
DIRS=include plugins/sak plugins/analyzers plugins/simulator
OBJLIBS=libRNeut.so libanalyzers.so libSAK.so libsimulator.so
LIBS= -lRNeut -lanalyzers -lSAK -lsimulator


all: ${EXE}	


RN_Root: RN_Root.o ${OBJLIBS}
	${CXX} -o bin/RNRoot $< ${LFLAGS} ${LIBS}

####################################################################
###Framework Dependencies###########################################
####################################################################

%.o: %.cpp
	${CXX} ${CFLAGS} $< 


libRNeut.so: force_look
	cd include; ${MAKE}


libSAK.so: force_look
	cd plugins/sak; ${MAKE}

libanalyzers.so: force_look
	cd plugins/analyzers; ${MAKE}

libsimulator.so:force_look
	cd plugins/simulator; ${MAKE}

force_look :
	true

#####################################################################
###Other Functions###################################################
#####################################################################

clean: 
	#rm  bin/RNBatchMode bin/RNRoot RN_Root.o
	-for d in ${DIRS}; do (cd $$d;$(MAKE) clean); done


help:
	@echo "Optimization Flags:\n \
	Make OPTIMIZE=01 : -O1 flag\n \
	Make OPTIMIZE=02 : -O2 flag\n  "
	@echo "Otherwise, debug flags are added -g -Wall\n"
	@echo "without doing Make clean it is necessary\n \
	to do make newdict to delete the root dictionary files before \n \
	recompiling and creating the new shared library\n"