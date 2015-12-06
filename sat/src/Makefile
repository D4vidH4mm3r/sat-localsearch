CC=g++ -std=c++11 -Wall

xyz: main.cpp input.o state.o
	$(CC) input.o state.o main.cpp -o xyz

%.o: %.cpp %.h
	$(CC) -c $*.cpp

.PHONY: clean
clean:
	rm -f *.o xyz
