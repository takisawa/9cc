
CFLAGS=-std=c11 -g -fno-common
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

# $@ variable is set to the name of the target
# the value $^ is a list of all the prerequisites
chibicc: $(OBJS)
	$(CC) -o chibicc -o $@ $^ $(LDFLAGS)

$(OBJS): chibicc.h

test: chibicc
	./test.sh

clean:
	rm -f chibicc *.o tmp*

.PHONY: test clean
