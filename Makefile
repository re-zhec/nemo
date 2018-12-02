EXEDIR := build
OBJDIR := obj
SRCDIR := src

EXE := $(EXEDIR)/game.exe
LOG := $(wildcard $(EXEDIR)/*.log)
SRC := $(shell find $(SRCDIR) -name *.cpp)
OBJ := $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRC))

CPPFLAGS := -I$(SRCDIR)
CPPFLAGS += -Ijson/single_include
CPPFLAGS += -Itype_safe/include
CPPFLAGS += -Itype_safe/external/debug_assert

CPPFLAGS += -IC:/SFML/include
CPPFLAGS += -IC:/MinGW/include
CPPFLAGS += -MMD -MP -DSFML_STATIC

CXXFLAGS := -std=c++17 -Wall -Wno-parentheses -pedantic

LDFLAGS := -LC:MinGW/lib/
LDFLAGS += -LC:/SFML/lib

LDLIBS := -lsfml-graphics-s -lsfml-window-s -lsfml-system-s
LDLIBS += -lopengl32 -lwinmm -lgdi32 -lfreetype

.PHONY: all clean

all: setup $(EXE)

setup:
	mkdir -p $(OBJDIR)
	mkdir -p $(EXEDIR)
	mkdir -p $(EXEDIR)/font
	cp $(wildcard font/*/fonts/ttf/*-Regular.ttf) $(EXEDIR)/font/
	cp -r data $(EXEDIR)
	
clean:
	rm -rf $(EXEDIR)
	rm -rf $(OBJDIR)

$(EXE): $(OBJ)
	$(CXX) $^ $(LDFLAGS) $(LDLIBS) -o $@ 

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@