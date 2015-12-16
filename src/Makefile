CC=g++ -std=c++11 -Wall -pthread -O3

xyz: main.cpp input.o state.o search.o
	$(CC) input.o state.o search.o main.cpp -o xyz

%.o: %.cpp %.h
	$(CC) -c $*.cpp

.PHONY: clean
clean:
	rm -f *.o xyz
