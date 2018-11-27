EXEDIR := build
OBJDIR := obj
SRCDIR := src

EXE := $(EXEDIR)/game.exe
SRC := $(wildcard $(SRCDIR)/*.cpp)
OBJ := $(SRC:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
LOG := $(wildcard $(EXEDIR)/*.log)

CPPFLAGS := -IC:/SFML/include 
CPPFLAGS += -IC:/pugixml/include
CPPFLAGS += -IC:/MinGW/include
CPPFLAGS += -MMD -DSFML_STATIC

CXXFLAGS := -std=c++17 -Wall -Wno-parentheses -pedantic

LDFLAGS := -LC:MinGW/lib/
LDFLAGS += -LC:/SFML/lib
LDFLAGS += -LC:/pugixml/lib

LDLIBS := -lsfml-graphics-s -lsfml-window-s -lsfml-system-s
LDLIBS += -lopengl32 -lwinmm -lgdi32 -lfreetype
LDLIBS += -lpugixml

.PHONY: all clean

all: $(EXEDIR) $(OBJDIR) $(EXE)
	
clean:
	rm -rf $(EXEDIR)
	rm -rf $(OBJDIR)

$(EXEDIR):
	mkdir -p $(EXEDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(EXE): $(OBJ)
	cp -r C:/pugixml/bin/libpugixml.dll $(EXEDIR)
	cp -r font $(EXEDIR)
	cp -r xml $(EXEDIR)
	$(CXX) $^ $(LDFLAGS) $(LDLIBS) -o $@ 

$(OBJ): $(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<