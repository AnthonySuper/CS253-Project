OBJDIR := ./obj
SRCDIR := ./src
TESTDIR := ./test_srcs
CXXFLAGS := -Iinclude -std=c++14 -Wall
SOURCES := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
TESTSRCS := $(wildcard $(TESTDIR)/*.cpp)
TESTOBJS := $(TESTSRCS:$(TESTDIR)/%.cpp=$(OBJDIR)/%.o)
TESTTARGET := test
TARGET := PA5


$(TARGET): main.cpp $(OBJECTS)
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) $^ -o $@


.PHONY: clean docs

all: $(OBJDIR) $(TARGET) $(TESTTARGET)

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TESTTARGET): $(TESTOBJS) $(OBJECTS) test.cpp
	$(CXX) $(CXXFLAGS) -Itest_srcs $^ -o $@

$(TESTOBJS): $(OBJDIR)/%.o : $(TESTDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -Itest_srcs -c $< -o $@

clean:
	rm -rf obj
	rm -f $(TARGET)
	rm -f $(TESTTARGET)

docs:
	Doxygen ./Doxyfile
