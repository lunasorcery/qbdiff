qbdiff: main.cpp
	$(CC) main.cpp -o qbdiff -O3 -lstdc++ -Wall -Wextra -Werror

clean:
	rm -f qbdiff
