INCLUDES=-I./include -L./lib
SRC_DIR=$(wildcard ./src/*.cpp)
OBJECTS:=$(patsubst %.cpp,%.o,$(SRC_DIR))
DEPFILES:=$(patsubst %.cpp,%.d,$(SRC_DIR))
OPT=$(INCLUDES) -std=c++17 -g -Wall -MP -MMD -lm -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32


all:$(OBJECTS)
	@echo -e main.exe
	@g32 $(OBJECTS) $(OPT)  -o main
	@echo -e Running
	@main.exe


-include $(DEPFILES)

%.o:%.cpp
	@echo -e  $<
	@g32 -std=c++17 $(INCLUDES) -c -o $@ $<

clean:
	@echo -e *.o
	@echo -e *.d
	@rm -r *.o *.d *.exe

run:
	@echo -e Running...
	main.exe