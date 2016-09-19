OBJDIR := ./obj
SRCDIR := ./src
CXXFLAGS := -Iinclude -std=c++14 -Wall
SOURCES := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
TARGET := PA3

.PHONY: $(OBJDIR) clean

all: $(OBJDIR) $(TARGET)

$(TARGET): main.cpp $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@


$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $@

clean:
	rm -rf obj
	rm $(TARGET)

