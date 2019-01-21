#include <iostream>
#include "classes.hh"

using namespace::std;

int main(int argc, char *argv[]) {

  // Initialize the Parameters
  Parameters params(argc, argv);

  Algorithms algo;

  // // get parameters
  // Parameters p(argc, argv);
  //
  // // initialize address rule
  // Rule rule(p);
  //
  // // print(cout, rule);
  // // print(cout, rule, 800);
  //
  // // DataBlock d(p);
  // // d.show();
  //
  // Ram r(p, &rule);
  // // r.show();
  // Cache c(p, &rule, &r);
  // // c.show();
  //
  // CPU mycpu(&c);

  algo.enter(params);

  return 0;
}
