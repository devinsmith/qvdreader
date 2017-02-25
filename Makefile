# Makefile for qvdreader (compiled by Mingw64)

.PHONY: all clean

SRCS = LineageInfo.cpp main.cpp QvdField.cpp QvdFile.cpp QvdTableHeader.cpp \
	utils/conversions.cpp
OBJS = $(SRCS:.cpp=.o)
DEPS = $(SRCS:.cpp=.d)

RM = rm -f

CXX? = g++

# Dependencies
DEP_CFLAGS = $(shell pkg-config expat --cflags)
DEP_LIBS = $(shell pkg-config expat --libs)

CFLAGS = -Wall -O2 -I.

EXE = qvdreader

all: $(EXE)

$(EXE): $(OBJS)
	$(CXX) $(CFLAGS) -o $(EXE) $(OBJS) $(DEP_LIBS)

.cpp.o:
	$(CXX) $(CFLAGS) $(DEP_CFLAGS) -MMD -MP -MT $@ -o $@ -c $<

clean:
	$(RM) $(OBJS)
	$(RM) $(DEPS)
	$(RM) $(EXE)

# Include automatically generated dependency files
-include $(wildcard *.d)
