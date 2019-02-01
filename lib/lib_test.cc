/* The test file for the library */

#define TEST_SIZE 3
#define TEST_ARRAY 10

#include "containers.hh"
#include <iostream>

using namespace::std;

int main() {
  /* Part 1: BlockQueues */

  cout << "---BlockQueues test start---" << endl;

  // Initialize the queues
  BlockQueues qs;
  qs.initialize(TEST_SIZE);

  cout << "Putting 0 to " << TEST_ARRAY-1 << " into the BlockQueues" << endl;

  for (unsigned i=0; i<TEST_ARRAY; ++i)
    qs.push(i%TEST_SIZE, i);

  // Show the memory status
  qs.show();

  // Pop the elements
  cout << "Popping out the elements\n";
  for (unsigned i=0; i<TEST_SIZE; ++i) {
    cout << "Set " << i << " : " << qs.pop(i) << "\n";
  }

  cout << "---BlockQueues test complete---" << endl;

  /* Part 2: LRUQueues */


  // Initialize the queues

  cout << "\n---BlockLRU test start---" << endl;

  // Initialize the queues
  BlockLRU ql;
  ql.initialize(TEST_SIZE);

  cout << "Putting 0 to " << TEST_ARRAY-1 << " into the BlockLRU" << endl;

  for (unsigned i=0; i<TEST_ARRAY; ++i)
    ql.push(i%TEST_SIZE, i);

  // Show the memory status
  ql.show();

  // Update the priority
  cout << "Updating the priorities\n";
  for (unsigned i=0; i<TEST_SIZE; ++i) {
    ql.update(i%TEST_SIZE, i);
  }

  ql.show();

  // Pop the elements
  cout << "Popping out the elements\n";
  for (unsigned i=0; i<TEST_SIZE; ++i) {
    cout << "Set " << i << " : " << ql.pop(i) << "\n";
  }

  cout << "---BlockLRU test complete---" << endl;

  return 0;

}
