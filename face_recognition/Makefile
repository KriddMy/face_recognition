########################################################################
####################### Makefile Template ##############################
########################################################################

# Makefile settings - Can be customized.
APPNAME = myapp
EXT = .cpp
OBJDIR = obj
DIFS = -DDLIB_USE_CUDA=1

# Include Folders
OPENCV_INCLUDE_PATH = -I/usr/local/include/opencv4

# Library Linkings
OPENCV_LIB_PATH = -L/usr/local/lib
OPENCV_LIBS = -lopencv_core -lopencv_highgui -lopencv_videoio -lopencv_imgproc
DLIB_LIB = -ldlib -lpthread -ljpeg -lpng -lX11
CUDA_LIB_PATH = -L/usr/local/cuda-11.7/lib64
CUDA_LIBS = -lcudart -lcudnn -lblas -lcublas -lcurand -lcusolver -llapack

# Compiler settings - Can be customized.
CC = g++
CXXFLAGS = -std=c++11 -Wall -O2 ${OPENCV_INCLUDE_PATH} ${DIFS}
LDFLAGS = ${OPENCV_LIB_PATH} ${OPENCV_LIBS} ${DLIB_LIB} ${CUDA_LIB_PATH} ${CUDA_LIBS}

############## Do not change anything from here downwards! #############
SRC = $(wildcard *$(EXT))
OBJ = $(SRC:%$(EXT)=$(OBJDIR)/%.o)
DEP = $(OBJ:$(OBJDIR)/%.o=%.d)
# UNIX-based OS variables & settings
RM = rm	
DELOBJ = $(OBJ)
# Windows OS variables & settings
DEL = del
EXE = .exe
WDELOBJ = $(SRC:%$(EXT)=$(OBJDIR)\\%.o)

########################################################################
####################### Targets beginning here #########################
########################################################################

all: $(APPNAME)

# Builds the app
$(APPNAME): $(OBJ)
	$(CC) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Creates the dependecy rules
%.d: $(SRCDIR)/%$(EXT)
	@$(CPP) $(CFLAGS) $< -MM -MT $(@:%.d=$(OBJDIR)/%.o) >$@

# Includes all .h files
-include $(DEP)

# Building rule for .o files and its .c/.cpp in combination with all .h
$(OBJDIR)/%.o: %$(EXT)
	$(CC) $(CXXFLAGS) -o $@ -c $<

################### Cleaning rules for Unix-based OS ###################
# Cleans complete project
.PHONY: clean
clean:
	$(RM) $(DELOBJ) $(DEP) $(APPNAME)

# Cleans only all files with the extension .d
.PHONY: cleandep
cleandep:
	$(RM) $(DEP)

#################### Cleaning rules for Windows OS #####################
# Cleans complete project
.PHONY: cleanw
cleanw:
	$(DEL) $(WDELOBJ) $(DEP) $(APPNAME)$(EXE)

# Cleans only all files with the extension .d
.PHONY: cleandepw
cleandepw:
	$(DEL) $(DEP)