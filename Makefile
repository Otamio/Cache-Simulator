################################################################################
# Compiling Instructions
################################################################################
CC= clang++
CFLAGS= -Wall -g -O3 -std=c++11 -o

################################################################################
# Source codes, Object files, and temporary files
################################################################################
src= emulator.cc parameters.cc Rule.cc Ram.cc DataBlock.cc Cache.cc algorithm.cc
# Library Modules
lib_test= lib/lib_test.cc
lib= $(filter-out $(lib_test), $(wildcard lib/*.cc))
# Source Files
src= $(wildcard src/*.cc)
# Executables
exe= ./cache-sim
lib_exe= ./library_test

################################################################################
# Execution commands
################################################################################
# Execution flags
exeflags= -r LRU -a mxm_block -d 400
daxpyflags= -d 9 -a daxpy -p
mxmflags= -d 3 -a mxm

################################################################################
# Compiling commands
################################################################################
all: main
	#	$(exe) $(exeflags)

# Produce the executable
main: $(src) $(lib)
	${CC} $(src) $(lib) ${CFLAGS} $(exe)

test-lib: $(lib_test) $(lib)
	${CC} $(lib_test) $(lib) ${CFLAGS} $(lib_exe)
	$(lib_exe)

test-daxpy:
	$(exe) $(daxpyflags)
test-mxm:
	$(exe) $(mxmflags)
test-associativity:
	$(exe) -n 1
	$(exe) -n 2
	$(exe) -n 4
	$(exe) -n 8
	$(exe) -n 8
	$(exe) -n 16
	$(exe) -n 1024
test-block-size:
	$(exe) -b 8
	$(exe) -b 16
	$(exe) -b 32
	$(exe) -b 64
	$(exe) -b 128
	$(exe) -b 256
	$(exe) -b 512
	$(exe) -b 1024
test-cache-size:
	$(exe) -c 4096
	$(exe) -c 8192
	$(exe) -c 16384
	$(exe) -c 32768
	$(exe) -c 65536
	$(exe) -c 131072
	$(exe) -c 262144
	$(exe) -c 524288
test-cache-thrashing-1:
	$(exe) -a mxm -d 480 -n 2
	$(exe) -a mxm_block -d 480 -f 32 -n 2
	$(exe) -a mxm -d 488 -n 2
	$(exe) -a mxm_block -d 488 -f 8 -n 2
	$(exe) -a mxm -d 512 -n 2
	$(exe) -a mxm_block -d 512 -f 32 -n 2
test-cache-thrashing-2:
	$(exe) -a mxm -d 480 -n 8
	$(exe) -a mxm_block -d 480 -f 32 -n 8
	$(exe) -a mxm -d 488 -n 8
	$(exe) -a mxm_block -d 488 -f 8 -n 8
	$(exe) -a mxm -d 512 -n 8
	$(exe) -a mxm_block -d 512 -f 32 -n 8
test-cache-thrashing-3:
	$(exe) -a mxm -d 480 -n 1024
	$(exe) -a mxm_block -d 480 -f 32 -n 1024
	$(exe) -a mxm -d 488 -n 1024
	$(exe) -a mxm_block -d 488 -f 8 -n 1024
	$(exe) -a mxm -d 512 -n 1024
	$(exe) -a mxm_block -d 512 -f 32 -n 1024
test-replacement-policy:
	$(exe) -r random
	$(exe) -r FIFO
	$(exe) -r LRU

clean:
	rm -f $(exe) $(lib_exe)
