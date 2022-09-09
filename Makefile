INCLUDES=-I./include -L./lib
OPT=-lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32 -std=c++17 $(INCLUDES)

all:
	g32 main.cpp -o main $(OPT)  && ./main

run:
	./main.exe

clean:
	rm -r *.exe