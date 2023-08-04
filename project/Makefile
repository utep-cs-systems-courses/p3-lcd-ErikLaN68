# makfile configuration
COMMON_OBJECTS 		= main.o wdt_handler.o shipdraw.o background.o buzzer.o switchcontrol.o shapecontrol.o collision.o machines.o
CPU             	= msp430g2553
CFLAGS          	= -mmcu=${CPU} -Os -I../h
LDFLAGS		= -L../lib -L/opt/ti/msp430_gcc/include/ 

#switch the compiler (for the internal make rules)
CC              = msp430-elf-gcc
AS              = msp430-elf-gcc -mmcu=${CPU} -c

all:game-c.elf game-s.elf

#additional rules for files
game-c.elf: ${COMMON_OBJECTS} switchstate.o
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^ -lTimer -lLcd
game-s.elf: ${COMMON_OBJECTS} switchstate-s.o
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^ -lTimer -lLcd

load-c: game-c.elf
	msp430loader.sh $^

load-s: game-s.elf
	msp430loader.sh $^

clean:
	rm -f *.o *.elf

switchstate.o : switchstate.c

switchstate-s.o: switchstate-s.s
