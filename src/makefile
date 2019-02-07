CXX=g++
CXX_FLAGS=-g

.PHONY: clean

rt: main.o vec3.hpp ray.hpp
	$(CXX) -o rt main.o

main.o: main.cpp
	$(CXX) -c main.cpp

clean:
	rm -rf *.o rt
