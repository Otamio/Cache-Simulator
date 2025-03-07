/* ./src/parameters.cc
 *
 *  class Parameters records all the information needed from commandline arguments,
 *    it has to be initialized before the function calls algorithm class methods.
 *
 *  This file also includes two methods of the class Result.
 */

#define PRECISION_DOUBLE 1

#include <iostream>
#include <unistd.h>
#include <cctype>
#include <iomanip>
#include "classes.hh"

/* print out the parameters to stdout */
ostream &print(ostream &os, const Parameters &p) {
  os << "INPUTS=======================================\n";
  os << "Ram Size = \t\t\t" << p.ram_size << " bytes\n";
  os << "Cache Size = \t\t\t" << p.cache_size << " bytes\n";
  os << "Block Size = \t\t\t" << p.block_size << " bytes\n";
  os << "Total Blocks in Cache = \t" << p.cache_size / p.block_size << '\n';
  os << "Associativity = \t\t" << p.n_map << '\n';
  os << "Number of Sets = \t\t" << p.n_sets << '\n';
  os << "Replacement Policy = \t\t" << p.replacement_policy << '\n';
  if (p.algorithm == "mxm_block") {
    os << "Algorithm = \t\t\t" << "blocked mxm" << '\n';
    os << "MXM Blocking Factor =\t\t" << p.blocking_factor << '\n';
    os << "Matrix Dimension = \t\t" << p.dimension << '\n';
  } else if (p.algorithm == "mxm"){
    os << "Algorithm = \t\t\t" << p.algorithm << '\n';
    os << "Matrix Dimension = \t\t" << p.dimension << '\n';
  } else {
    os << "Algorithm = \t\t\t" << p.algorithm << '\n';
    os << "Vector Dimension = \t\t" << p.dimension << '\n';
  }
  os << flush;

  return os;
}

/* Initialize parameters from commandline arguments */
Parameters::Parameters(int argc, char *argv[]) {
  extern char *optarg;
  extern int optind, opterr, optopt;

  int c;

  while ((c = getopt(argc, argv, "c:b:n:r:a:d:pf:l")) != -1) {
    switch (c) {
      case 'c':
        // read size of cache
        cache_size = atoi(optarg);
        break;
      case 'b':
        // read size of data block
        block_size = atoi(optarg);
        break;
      case 'n':
        // read associativity
        n_map = atoi(optarg);
        break;
      case 'r':
        // read replacement policy
        replacement_policy = string(optarg);
        break;
      case 'a':
        // read the algorithm to simulate
        algorithm = string(optarg);
        break;
      case 'd':
        // read in the dimension
        dimension = atoi(optarg);
        break;
      case 'p':
        // enables the printing of the output matrix
        output = true;
        break;
      case 'f':
        // read in the blocking factor
        blocking_factor = atoi(optarg);
        break;
      case 'l':
        load = true;
        break;
    // end of switch
    }
  // end of while (commandline arguments)
  }
  // Infer the number of sets from given
  n_sets = cache_size / block_size / n_map;

  if (algorithm == "daxpy")
    ram_size = dimension * WORD_SIZE * 3;
  else
    ram_size = dimension * dimension * WORD_SIZE * 3;
}

/* Print out the results to stdout */
ostream &print(ostream &os, const Result &result) {
  os << "RESULTS=======================================\n";
  os << "Instruction count:\t" << result.count_instructions() << "\n";
  os << "Read hits:\t\t" << result.count_read_hits() << "\n";
  os << "Read misses:\t\t" << result.count_read_misses() << "\n";
  os << "Read miss rate:\t\t" << setprecision(PRECISION_DOUBLE+2) << result.count_read_miss_rate()*100 << "%\n";
  os << "Write hits:\t\t" << result.count_write_hits() << "\n";
  os << "Write misses:\t\t" << result.count_write_misses() << "\n";
  os << "Write miss rate:\t" << setprecision(PRECISION_DOUBLE+2) << result.count_write_miss_rate()*100 << "%\n";
  os << flush;

  return os;
}

/* Restting the result: Set all field values to 0 */
void Result::reset() {
  this->instruction_count = 0;
  this->read_hits = 0;
  this->read_misses = 0;
  this->write_hits = 0;
  this->write_misses = 0;
}
