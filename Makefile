CFLAGS = -std=c11 -Wall -Wextra -g -static
LDFLAGS = -lm

SRCS = $(wildcard src/*.c)
OBJS = $(SRCS:.c=.o)

atrcc: $(OBJS)
	$(CC) $(CFLAGS) -o atrcc $(OBJS) $(LDFLAGS)

$(OBJS): src/atrcc.h

test: atrcc
		./test/test.sh

clean:
		rm -f atrcc *.o src/*.o tmp*

.PHONY: test clean
