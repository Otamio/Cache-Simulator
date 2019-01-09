#include <iostream>
#include <unistd.h>
#include <cctype>
#include "classes.hh"

ostream &print(ostream &os, const Parameters &p) {
  // print out the parameters
  os << "INPUTS=======================================\n";
  os << "Ram Size = \t\t\t" << p.ram_size << " bytes\n";
  os << "Cache Size = \t\t\t" << p.cache_size << " bytes\n";
  os << "Block Size = \t\t\t" << p.block_size << " bytes\n";
  os << "Total Blocks in Cache = \t" << p.cache_size / p.block_size << '\n';
  os << "Associativity = \t\t" << p.n_map << '\n';
  os << "Number of Sets = \t\t" << p.n_sets << '\n';
  os << "Replacement Policy = \t\t" << p.replacement_policy << '\n';
  os << "Algorithm = \t\t\t" << p.algorithm << '\n';
  os << flush;

  return os;
}

Parameters &Parameters::read(int argc, char *argv[]) {
  extern char *optarg;
  extern int optind, opterr, optopt;

  int c;

  while ((c = getopt(argc, argv, "c:b:n:r:a:d:pf:")) != -1) {
    switch (c) {
      case 'c':
        // read size of cache
        this->cache_size = atoi(optarg);
        break;
      case 'b':
        // read size of data block
        this->block_size = atoi(optarg);
        break;
      case 'n':
        // read associativity
        this->n_map = atoi(optarg);
        break;
      case 'r':
        // read replacement policy
        this->replacement_policy = string(optarg);
        break;
      case 'a':
        // read the algorithm to simulate
        this->algorithm = string(optarg);
        break;
      case 'd':
        // read in the dimension
        this->dimension = atoi(optarg);
        break;
      case 'p':
        // enables the printing of the output matrix
        this->output = true;
        break;
      case 'f':
        // read in the blocking factor
        this->blocking_factor = atoi(optarg);
        break;
    // end of switch
    }
  // end of while (commandline arguments)
  }

  return *this;
}
