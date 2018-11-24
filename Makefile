EXE := ./build/game.exe
LOG := ./build/*.log
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

all: $(EXE)

$(EXE): $(OBJ)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

clean:
	rm -rf $(EXE) $(LOG) $(OBJ) $(DEP)

ifeq "$(MAKECMDGOALS)" ""
-include $(DEP)
endif