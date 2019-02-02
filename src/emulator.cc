/* ./src/emulator.cc
 * The src file includes the main function
 *  The main function create a Parameters instances by the commandline arguments,
 *    afterwards it transfer the control to the Algorithms class.
 */

#include <iostream>
#include "classes.hh"

using namespace::std;

int main(int argc, char *argv[]) {

  // Initialize the Parameters
  Parameters params(argc, argv);

  Algorithms algo;

  // Run the algorithm
  algo.enter(params);

  return 0;
}
