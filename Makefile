PERLIN_NOISE=D:/Projects/c++/cpp_modules/PerlinNoise
INCLUDES=-I./include -I$(PERLIN_NOISE) -L./lib
SRC_DIR=$(wildcard ./src/*.cpp)
OBJECTS:=$(patsubst %.cpp,%.o,$(SRC_DIR))
DEPFILES:=$(patsubst %.cpp,%.d,$(SRC_DIR))
BUILD_OPT=-O3 -mwindows -s setup.res -DBUILD_RELEASE
OPT=$(INCLUDES) $(BUILD_OPT) -std=c++17 -Wall -MP -MMD -lm -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32


all:$(OBJECTS)
	@echo -e main.exe
	@g32 $(OBJECTS) $(OPT)  -o ./bin/main
	@echo -e Running
	@./bin/main.exe


-include $(DEPFILES)

%.o:%.cpp
	@echo -e  $<
	@g32 $(INCLUDES) $(OPT) -c -o $@ $<

clean:
	@rm -r ./src/*.o ./bin/*.exe

build_rc:
	windres setup.rc -O coff -o setup.res

run:
	@echo -e Running...
	@./bin/main.exe