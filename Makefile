EXEDIR := build
OBJDIR := obj
SRCDIR := src

EXE := $(EXEDIR)/game.exe
LOG := $(wildcard $(EXEDIR)/*.log)
SRC := $(wildcard $(SRCDIR)/**/*.cpp) $(wildcard $(SRCDIR)/*.cpp)
OBJ := $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRC))

CPPFLAGS += -I$(SRCDIR)
CPPFLAGS += -IC:/SFML/include
CPPFLAGS += -IC:/MinGW/include
CPPFLAGS += -MMD -MP -DSFML_STATIC

CXXFLAGS := -std=c++17 -Wall -Wno-parentheses -pedantic

LDFLAGS := -LC:MinGW/lib/
LDFLAGS += -LC:/SFML/lib

LDLIBS := -lsfml-graphics-s -lsfml-window-s -lsfml-system-s
LDLIBS += -lopengl32 -lwinmm -lgdi32 -lfreetype

.PHONY: all clean

all: data $(EXE)

data:
	mkdir -p $(EXEDIR)
	mkdir -p $(OBJDIR)
	cp -r font $(EXEDIR)
	cp -r json $(EXEDIR)
	
clean:
	rm -rf $(EXEDIR)
	rm -rf $(OBJDIR)

$(EXE): $(OBJ)
	$(CXX) $^ $(LDFLAGS) $(LDLIBS) -o $@ 

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@