OBJDIR := ./obj
SRCDIR := ./src
TESTDIR := ./test
CXXFLAGS := -Iinclude -std=c++14 -Wall
SOURCES := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
TESTSRCS := $(wildcard $(TESTDIR)/*.cpp)
TESTOBJS := $(TESTSRCS:$(TESTDIR)/%.cpp=$(OBJDIR)/%.o)
TESTTARGET := test
TARGET := PA4

.PHONY: $(OBJDIR) clean

all: $(OBJDIR) $(TARGET)

$(TARGET): main.cpp $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TESTTARGET): $(TESTOBJS) $(OBJS) test.cpp
	$(CXX) $(CXXFLAGS) -Itest $^ -o $@


$(OBJDIR):
	mkdir -p $@

clean:
	rm -rf obj
	rm $(TARGET)

