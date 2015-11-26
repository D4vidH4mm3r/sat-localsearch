CC=g++ -std=c++11 -Wall

main: main.cpp input.o
	$(CC) input.o main.cpp -o main

%.o: %.cpp %.h
	$(CC) -c $*.cpp

.PHONY: clean
clean:
	rm -f *.o main
