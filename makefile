matrix: matrix.o
	g++ -g -Wall -pedantic $^ -o $@

matrix.o: matrix.cpp matrix.h
	g++ -g -c -Wall -pedantic $< -o $@

.PHONY: clean

clean:
	-rm matrix.o matrix
