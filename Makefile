

all: testgpio


testgpio:odroidc2.o testgpio.o
	$(CC) -o $@ $^ ${LDFLAGS}
