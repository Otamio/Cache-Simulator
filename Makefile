CC= clang++
CFLAGS= -Wall -g -o
src= emulator.cc parameters.cc Rule.cc Ram.cc DataBlock.cc Cache.cc algorithm.cc LRUQueues.cc BlockQueues.cc
exe= ./cache-sim
exeflags= -r random -a mxm -d 400

daxpyflags= -d 9 -a daxpy -p
mxmflags= -d 3 -a mxm

all: main
		$(exe) $(exeflags)

test-daxpy:
	$(exe) $(daxpyflags)
test-mxm:
	$(exe) $(mxmflags)

main: $(src)
	${CC} $(src) ${CFLAGS} $(exe)

clean:
	rm -f $(exe)
