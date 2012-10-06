CC=gcc
AR=ar
CFLAGS=-g -Wall -I..
LDFLAGS=

STATIC_LIB=libgcl.a

OBJS = error.o malloc.o

$(STATIC_LIB): $(OBJS)
	$(AR) rcs $(STATIC_LIB) $(OBJS)

error.o: error.c error.h compat.h
	$(CC) $(CFLAGS) -c error.c

malloc.o: malloc.c malloc.h error.h compat.h
	$(CC) $(CFLAGS) -c malloc.c

clean:
	rm -f $(OBJS) *~
