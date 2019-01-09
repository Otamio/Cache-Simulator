CC= clang++
CFLAGS= -o
src= emulator.cc parameters.cc
exe= ./cache-sim

all: main
		$(exe)

main: $(src)
	${CC} $(src) ${CFLAGS} $(exe)

clean:
	rm -f $(exe)
