DIR := ./build/

EXE := $(addprefix $(DIR), game.exe)
LOG := $(wildcard $(addprefix $(DIR), *.log))

SRC := $(wildcard ./src/*.cpp)
OBJ := $(SRC:.cpp=.o)
DEP := $(OBJ:.o=.d)

CPPFLAGS	:= -IC:/SFML/include 
CPPFLAGS += -IC:/pugixml/include
CPPFLAGS += -IC:/MinGW/include
CPPFLAGS	+= -MMD -DSFML_STATIC

CXXFLAGS := -std=c++17 -Wall -Wno-parentheses -pedantic

LDFLAGS	:= -LC:MinGW/lib/
LDFLAGS  += -LC:/SFML/lib
LDFLAGS	+= -LC:/pugixml/lib

# LDLIBS	:= -lreversed
LDLIBS   := -lsfml-graphics-s -lsfml-window-s -lsfml-system-s
LDLIBS   += -lopengl32 -lwinmm -lgdi32 -lfreetype
LDLIBS	+=	-lpugixml

.PHONY: all clean

all: $(DIR) $(EXE)

$(DIR):
	mkdir -p $(DIR)
	cp -r C:/SFML/bin/* $(DIR)
	cp -r C:/pugixml/bin/* $(DIR)
	cp -r font $(DIR)
	cp -r xml $(DIR)

$(EXE): $(OBJ)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

clean:
	rm -rf $(DIR) 
	rm -rf $(OBJ) $(DEP)