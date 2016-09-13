BINDIR := ./bin
SRCDIR := ./src
CXXFLAGS := -Iinclude -std=c++14
SOURCES := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS := $(SOURCES:$(SRCDIR)/%.cpp=$(BINDIR)/%.o)
TARGET := PA3

.PHONY: $(BINDIR) clean

all: $(BINDIR) $(TARGET)

$(TARGET): main.cpp $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@


$(OBJECTS): $(BINDIR)/%.o : $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BINDIR):
	mkdir -p $@

clean:
	rm -rf bin
	rm $(TARGET)

