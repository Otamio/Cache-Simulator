#include <iostream>
#include "classes.hh"

using namespace::std;

int main(int argc, char *argv[]) {

  Parameters p;

  p.read(argc, argv);

  print(cout, p);

  return 0;
}
