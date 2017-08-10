INCLUDE=
CFLAG = -c -Wall -O2 -pg -Wall -fPIC -march=core2 -mtune=core2
GCC = g++
CC = gcc
.SUFFIXES: .o .cpp .c

OBJS = MyLogMessage.o ReadConfig.o
	   

all: ./a/libmylib.a

./a/libmylib.a: $(OBJS)
	ar -crv $@ $(OBJS);
.cpp.o:
	$(GCC) $(CFLAG) -o $@ $< $(INCLUDE)

.c.o:
	$(CC) $(CFLAG) -o $@ $< $(INCLUDE)

clean:
	rm -f *.o
	rm ./a/libmylib.a
