# momo:modified to create class avril 2010

# Extensions des fichiers
ObjSuf        = o
SrcSuf        = cpp
ExeSuf        =
DllSuf        = so
OutPutOpt     = -o

# Optimisation
#OPTFLAGS = -O3 -funroll-loops\ # -floop-optimize2\  Mhd 10Jul2013 not working...
#         -fgcse-lm  -fgcse-sm

# Optimisation
 OPTFLAGS =  -funroll-loops\
          -fgcse-lm  -fgcse-sm 

# Compilateur et options
CXX           = g++
CXXFLAGS      = -O -Wall -fPIC #-DDEBUG #-ggdb
INCLUDE       = -I$(CLHEP_BASE_DIR)/include # Mhd 10 Jul 2013
LD            = g++
LDFLAGS       = -O $(OPTFLAGS)
SOFLAGS       = -shared 


# Options de ROOT
ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLIBS     := $(shell root-config --libs) -lMathMore -lSpectrum
ROOTGLIBS    := $(shell root-config --glibs)
#CLHEPLIBS    := $(shell /home/mhd/Packages/clhep-2.2.0.4/CLHEP/clhep-config --libs)

CXXFLAGS     += $(ROOTCFLAGS) $(OPTFLAGS)
LIBS          = $(ROOTLIBS) $(SYSLIBS) $(CLHEPLIBS)
GLIBS         = $(ROOTGLIBS) $(SYSLIBS)


# Environnement
# CHANGE PATH TO THE DIRECTORY OF THE PROJECT
DIR		= ./
BIN		= ./bin

# CHANGE ADD/REMOVE CLASSES OR TOOLS
SRCS	=  draw_tool.cpp theo_tool.cpp\
	   hist_tool.cpp kine_tool.cpp\
	   error_tool.cpp\
	   get_cs.cpp cstool.cpp

OBJECTS	=  draw_tool.o theo_tool.o\
	   hist_tool.o kine_tool.o\
	   error_tool.o\
	   get_cs.o cstool.o


# CHANGE NAME OF THE EXCUTION 
PROGRAMS = cstool.exe


# Par defaut on compile tout
all:		$(OBJECTS)\
		$(BIN)/$(PROGRAMS)


# Programme principal
$(BIN)/$(PROGRAMS):	$(OBJECTS)
			$(LD) $(LDFLAGS) $^ $(LIBS) -o $@
			@echo "$@ done"

clean : 
	rm  $(OBJECTS)

distclean:
	cd $(DIR); make clean


.$(SrcSuf).$(ObjSuf):
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $<

# dependances
theo_tool.o:	theo_tool.h	theo_tool.cpp	    Defines.h
draw_tool.o:	draw_tool.h	draw_tool.cpp	    Defines.h
hist_tool.o:	hist_tool.h	hist_tool.cpp	    Defines.h
kine_tool.o:	kine_tool.h	kine_tool.cpp	    Defines.h
error_tool.o:	error_tool.h	error_tool.cpp	    Defines.h
get_cs.o:	get_cs.h	get_cs.cpp	    Defines.h
cstool.o:	get_cs.o        Defines.h
