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

CFLAGS = -Wall -O2 -fPIC # -DDEBUG_BUILD

EXE = qvdreader
SO = libqvdreader.so

all: $(EXE) $(SO)

$(EXE): $(OBJS)
	$(CXX) $(CFLAGS) -o $(EXE) $(OBJS) $(DEP_LIBS)

$(SO): $(OBJS)
	$(CXX) -shared -Wl,-soname,$@ $(OBJS) -o $@  $(DEP_LIBS)
#	$(CXX) -shared -Wl,-install_name,$@ $(OBJS) -o $@  $(DEP_LIBS)

.cpp.o:
	$(CXX) $(CFLAGS) $(DEP_CFLAGS) -MMD -MP -MT $@ -o $@ -c $<

clean:
	$(RM) $(OBJS)
	$(RM) $(DEPS)
	$(RM) $(EXE)
	$(RM) $(SO)

# Include automatically generated dependency files
-include $(wildcard *.d)
