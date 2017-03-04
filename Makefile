# Makefile for qvdreader (for Linux/Unix)

.PHONY: all clean

SRCS = LineageInfo.cpp main.cpp QvdField.cpp QvdFile.cpp QvdTableHeader.cpp \
	utils/conversions.cpp utils/dumphex.cpp
OBJS = $(SRCS:.cpp=.o)
DEPS = $(SRCS:.cpp=.d)

RM = rm -f

CXX? = g++

# Dependencies
DEP_CFLAGS = $(shell pkg-config libxml-2.0 --cflags)
DEP_LIBS = $(shell pkg-config libxml-2.0 --libs)

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
