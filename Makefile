CFLAGS = -std=c11 -Wall -Wextra -g -static
LDFLAGS = -lm

atrcc: src/atrcc.c
	cc $(CFLAGS) src/atrcc.c -o atrcc $(LDFLAGS)

test: atrcc
		./test/test.sh

clean:
		rm -f atrcc *.o tmp*

.PHONY: test clean
