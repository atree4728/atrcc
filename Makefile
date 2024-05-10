CFLAGS = -std=c11 -Wall -Wextra -g -static

atrcc: src/atrcc.c
	cc $(CFLAGS) src/atrcc.c -o atrcc

test: atrcc
		./test/test.sh

clean:
		rm -f atrcc *.o tmp*

.PHONY: test clean
