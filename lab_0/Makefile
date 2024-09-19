0_1.out: 0_1.c
	gcc -Wall -Wextra -Ofast -march=native -mtune=native -std=c2x -o 0_1.out 0_1.c

0_1: 0_1.out
	./0_1.out

0_2.out: 0_2.cpp
	g++ -Wall -Wextra -Ofast -march=native -mtune=native -std=c++20 -o 0_2.out 0_2.cpp

0_2: 0_2.out
	./0_2.out

clean:
	rm -rf *.out