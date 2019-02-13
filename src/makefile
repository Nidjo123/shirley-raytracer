CXX=g++
CXX_FLAGS=-O3

.PHONY: clean

all: rt interactive

rt: main.o vec3.hpp ray.hpp hitable.hpp sphere.hpp hitable_list.hpp camera.hpp material.hpp
	$(CXX) $(CXX_FLAGS) -o rt main.o

interactive: rt interactive.cpp
	$(CXX) $(CXX_FLAGS) -o interactive interactive.cpp -lsfml-graphics -lsfml-window -lsfml-system -lpthread

main.o: main.cpp
	$(CXX) $(CXX_FLAGS) -c main.cpp

clean:
	rm -rf *.o rt interactive
