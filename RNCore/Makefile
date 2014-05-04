CXX=g++

########.cpp files in the src file to be compiled#############################################
########these files need .hpp files in the include folder#####################

SRCFILES=RN_BaseDetector.cpp RN_NeutDetectorArray.cpp \
	RN_VariableMap.cpp RN_IonChamber.cpp RN_SiliconDetectors.cpp\
	RN_Calibrator.cpp RN_Timing.cpp RN_Analyzer.cpp sak_ReadBuffer.cpp\
	RN_MassTable.cpp RN_Particle.cpp RN_NaIArray.cpp RN_TriggerBit.cpp\
        RN_Root.cpp RN_Sim.cpp RN_SimPhysics.cpp RN_BaseClass.cpp RN_Module.cpp\
	RN_Parameters.cpp RN_EventProcessor.cpp RN_ReactionInfo.cpp
########Directories and Files###########################################################
SRCDIR=src
HDRDIR=include

HDRFILES=${SRCFILES:.cpp=.hpp}
OBJECTS=${SRCFILES:.cpp=.o}

SOURCES=${addprefix $(SRCDIR)/,$(SRCFILES)}
HEADERS=${addprefix $(HDRDIR)/,$(HDRFILES)}
############################################################################

######Flags################################################################
OPTIONS=-lMinuit -lSpectrum

ifndef OPTIMIZE
DEBUG=-g -Wall
else 
DEBUG=-O2
endif

CFLAGS=`root-config --cflags` -c ${DEBUG} -I$(HDRDIR)
GLIBS=`root-config --glibs`
LINK_DIR=${PWD}/../lib
LFLAGS=-L${LINK_DIR} ${GLIBS} ${OPTIONS} ${DEBUG} -I${ROOTSYS}/include -I$(HDRDIR)


##########################################################################
all: libRNCore.so

libRNCore.so: ${OBJECTS} RNCore_d.cpp 
	g++ -shared -o ${addprefix ../lib/,$@} $^ ${LFLAGS} -fPIC
#	@echo "cleaning up: \n .o files"  
#	rm ${OBJECTS}
	@echo "cleaning up: \n _d dictionary files"  	
	rm RNCore_d.*

RNCore_d.cpp: ${HEADERS} $(HDRDIR)/my_LinkDef.h
	rootcint $@ -c `root-config --cflags` -p $^

%.o: ${addprefix $(SRCDIR)/,%.cpp} ${addprefix $(HDRDIR)/,%.hpp}
	${CXX} ${CFLAGS} $< -fPIC

#####################################################################
###Other Functions###################################################
#####################################################################

clean: 
	rm  ${OBJECTS} RNCore_d.* ../lib/libRNCore.so

newdict:
	rm RNCore_d.*


help:
	@echo "Optimization Flags:\n \
	Make OPTIMIZE : -O2 flag\n  "
	@echo "Otherwise, debug flags are added -g -Wall\n"
