###############################################################################
#                          433-380 Project 2 makefile                         #
#=============================================================================#
# This file is used to build Project 2, and related files.                    #
# Note: this makefile was intended to be used with GNU make.                  #
#                                                                             #
#                             VISUAL STUDIO IS GAY                            #
#                                                                             #
#                                                                             #
# Targets:                                                                    #
#   o all                                                                     #
#   o clean - deletes object files                                            #
#   o clobber - deletes object files, and binaries                            #
#   o project1 - creates the `project1' executable                            #
#   o doc - generates API documentation                                       #
###############################################################################

###############################################################################
#                                   SOURCES                                   #
###############################################################################
# Source files
SRC		:= $(SRC) Window.c
SRC		:= $(SRC) GameState.c
SRC		:= $(SRC) mechanics.c
SRC		:= $(SRC) Player.c
SRC		:= $(SRC) Landscape.c
SRC		:= $(SRC) Object.c
SRC		:= $(SRC) render.c
SRC		:= $(SRC) input.c
SRC		:= $(SRC) Camera.c
SRC		:= $(SRC) Viewport.c
SRC		:= $(SRC) maths.c

# Infer header and object files from source files
HDR      = $(SRC:.c=.h)
OBJ      = $(SRC:.c=.o)

# Source directories
SRCDIRS  = 
# Infer make search paths from source directories
VPATH    = $(SRCDIRS)


###############################################################################
#                                GENERAL SETUP                                #
###############################################################################
# Executable name
EXEC     = project2
# Declare phony rules
.PHONY: all clean clobber doc


###############################################################################
#                              COMPILER OPTIONS                               #
###############################################################################
# Compiler
CC       = gcc
# Infer include paths from SRCDIRS
INCLUDE  = $(SRCDIRS:%=-I%)
# C compiler flags
CFLAGS = $(INCLUDE) -ggdb -Wall -pedantic -fbounds-check
# Libraries
LIB      = -lglut -lGLU -lGL -lXmu -lXi -lXext -lX11 -lm
# Linker options
LINKER   = 
# Implicit variable for linker
LDFLAGS  = $(LINKER) $(LIB)


###############################################################################
#                                     RULES                                   #
###############################################################################
all: $(OBJ) $(EXEC) doc
	
#doc:
#	@doxygen >> .doxygen.out

clean:
	rm *.o
	@echo
	@echo "Clean complete."
	@echo

clobber: clean
	rm $(EXEC)
	@echo
	@echo "Clobber complete."
	@echo

$(EXEC): $(OBJ) main.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(EXEC) $(OBJ) main.c
	@echo
	@echo "Project 2 successfully compiled."
	@echo

# General     #################################################################
Window.o: Window.h Window.c
Player.o: Player.h Player.c
input.o: Window.h input.h input.c
GameState.o: Player.h GameState.h GameState.c
mechanics.o: Player.h Object.h Landscape.h mechanics.h mechanics.c
Landscape.o: Object.h Player.h Landscape.h Landscape.c
Object.o: Object.h Object.c
render.o: Viewport.h GameState.h render.h render.c
Camera.o: Camera.h Camera.c
Viewport.o: Camera.h Viewport.h Viewport.c
maths.o: maths.h maths.c

debug:
	@echo "SOURCES"
	@echo "================================="
	@echo "	SRC		=		$(SRC)"
	@echo "	HDR		=	 $(HDR)"
	@echo "	OBJ		=	 $(OBJ)"
	@echo "	SRCDIRS		=	 $(SRCDIRS)"
	@echo "	VPATH		=	 $(VPATH)"
	@echo
	@echo "GENERAL SETUP"
	@echo "================================="
	@echo "	EXEC		=	 $(EXEC)"
	@echo
	@echo "COMPILER OPTIONS"
	@echo "================================="
	@echo "	CC		=	 $(CC)"
	@echo "	CPPFLAGS	=	 $(CPPFLAGS)"
	@echo "	INCLUDE		=	 $(INCLUDE)"
	@echo "	LIB		=	 $(LIB)"
	@echo "	LINKER		=	 $(LINKER)"
	@echo "	CXXFLAGS	=	 $(CXXFLAGS)"
	@echo "	LDFLAGS		=	 $(LDFLAGS)"