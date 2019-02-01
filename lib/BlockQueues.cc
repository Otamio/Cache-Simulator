/* BlockQueues is a data structure in the library
 *  BlockQueues is a vector of deque of block_ids,
 *    each element of the vector is the block_id sequence of the set
 * Cache with n sets will have a vector of BLockQueues with n deques
 */

#include "containers.hh"
#include <iostream>

/* Constructor: Initialize the BlockQueues with the number of sets (n_sets) */
BlockQueues::BlockQueues(unsigned n_sets) {
  numSets = n_sets;

  deque<unsigned> q;
  for (unsigned i=0; i!=n_sets; ++i) {
    vector<deque<unsigned>> queues;
    queues.push_back(q);
  }
}

/* Fill the number of queues sepecified by nsets into the data structure */
void BlockQueues::initialize(unsigned n_sets) {
  // if (this->numSets)
  //   throw runtime_error("Invalid call of BlockQueues::initialize()");

  this->numSets = n_sets;

  for (unsigned i=0; i!=n_sets; ++i) {
    deque<unsigned> q;
    this->queues.push_back(q);
  }
}

/* Pop the block number from the queue specified by set_id */
unsigned BlockQueues::pop(unsigned set_id) {
  if (this->queues[set_id].empty())
    throw runtime_error("Try to pop from an empty queue (Code: 005)");

  unsigned ret = this->queues[set_id].back();
  this->queues[set_id].pop_back();
  return ret;
}

/* Push the block number to the queue specified by set id */
void BlockQueues::push(unsigned set_id, unsigned block_id) {
  this->queues[set_id].push_front(block_id);
}

/* Show the runtime memory status of the data structure */
void BlockQueues::show() {
  unsigned ct = 0;

  for (auto &set : this->queues) {

    cout << "Set " << ct++ << ": ";

    for (auto &block_id : set)
      cout << block_id << " ";
    cout << endl;
  }
}
