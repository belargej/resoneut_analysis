CXX=g++
OPTIONS=-lMinuit

CFLAGS=`root-config --cflags` -c ${DEBUG}
GLIBS=`root-config --glibs`
LINK_DIR=${PWD}/lib/
LFLAGS=-L${LINK_DIR} ${GLIBS} ${OPTIONS} ${DEBUG}
INC_DIR=-I${ROOTSYS}/include -I.

SUB_DIRS=include plugins/sak plugins/analyzers

OBJLIBS= libRNeut.so libRNanalyzers.so libSAK.so

LIBS= -lRNeut -lRNanalyzers -lSAK

all: ${OBJLIBS}	


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

force_look :
	true

#####################################################################
###Other Functions###################################################
#####################################################################

clean: 
	-for d in ${SUB_DIRS}; do (cd $$d;$(MAKE) clean); done

newdict: 
	-for d in ${SUB_DIRS}; do (cd $$d;$(MAKE) newdict); done

help:
	@echo "Optimization Flags:\n \
	Make OPTIMIZE=01 : -O1 flag\n \
	Make OPTIMIZE=02 : -O2 flag\n  "
	@echo "Otherwise, debug flags are added -g -Wall\n"
	@echo "without doing Make clean it is necessary\n \
	to do make newdict to delete the root dictionary files before \n \
	recompiling and creating the new shared library\n"
