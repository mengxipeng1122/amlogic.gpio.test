

CC=arm-linux-gnueabihf-gcc
LD=arm-linux-gnueabihf-gcc

all: testgpio
	cp testgpio /tmp


testgpio:odroidc2.o testgpio.o
	$(CC) -o $@ $^ ${LDFLAGS}

clean:
	rm -fr *.o testgpio
