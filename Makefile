DIR := ./build/

EXE := $(addprefix $(DIR), game.exe)
LOG := $(wildcard $(addprefix $(DIR), *.log))

SRC := $(wildcard ./src/*.cpp)
OBJ := $(SRC:.cpp=.o)
DEP := $(OBJ:.o=.d)

CPPFLAGS	:= -IC:/SFML/include -IC:/pugixml/include 
CPPFLAGS	+= -MMD -DSFML_STATIC

CXXFLAGS := -std=c++17 -Wall -pedantic

LDFLAGS  := -LC:/SFML/lib -LC:/pugixml/lib

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
	rm -rf $(EXE) $(LOG) 
	rm -rf $(OBJ) $(DEP)
	rm -rf $(addprefix $(DIR), font)
	rm -rf $(addprefix $(DIR), xml)