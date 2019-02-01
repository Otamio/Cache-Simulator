/* BlockLRU is a data structure in the library
 *  BlockLRU is a vector of list of block_ids,
 *    each element of the vector is the block_id sequence of the set
 * Cache with n sets will have a vector of BLockQueues with n lists
 */

#include "containers.hh"
#include <iostream>

/* Constructor: Initialize the BlockLRU with the number of sets (n_sets) */
BlockLRU::BlockLRU(unsigned n_sets) {
  numSets = n_sets;

  list<unsigned> l;
  unordered_map<unsigned, list<unsigned>::iterator> ma;
  for (unsigned i=0; i!=n_sets; ++i) {
    lists.push_back(l);
    maps.push_back(ma);
  }
}

/* Fill the number of queues specified by nsets into the data structure */
void BlockLRU::initialize(unsigned n_sets) {
  this->numSets = n_sets;

  list<unsigned> l;
  unordered_map<unsigned, list<unsigned>::iterator> ma;
  for (unsigned i=0; i!=n_sets; ++i) {
    this->lists.push_back(l);
    this->maps.push_back(ma);
  }
}

/* Pop the block number from the queue specified by set_id */
unsigned BlockLRU::pop(unsigned set_id) {
  if (this->lists[set_id].empty())
    throw runtime_error("Try to pop from an empty queue (Code: 005).\n");
  // Remove from list
  unsigned ret = this->lists[set_id].back();
  this->lists[set_id].pop_back();
  // Remove from map
  auto ret_it = this->maps[set_id].find(ret);
  this->maps[set_id].erase(ret_it);

  return ret;
}

/* Update the priority of the block in the queue specified by set_id */
void BlockLRU::update(unsigned set_id, unsigned block_id) {
  // Remove from list
  auto it = this->maps[set_id][block_id];
  this->lists[set_id].erase(it);
  // Remove from map
  auto map_it = this->maps[set_id].find(block_id);
  this->maps[set_id].erase(map_it);
  // Reinsert to the beginning of list, and add to map
  this->lists[set_id].push_front(block_id);
  this->maps[set_id].insert(make_pair(block_id, this->lists[set_id].begin()));
}

/* Push the block number to the queue specified by set id */
void BlockLRU::push(unsigned set_id, unsigned block_id) {
  // Push to list
  this->lists[set_id].push_front(block_id);
  this->maps[set_id].insert(make_pair(block_id, this->lists[set_id].begin()));
}

/* Show the runtime memory status of the data structure */
void BlockLRU::show() {
  unsigned ct = 0;

  for (auto &set : this->lists) {

    cout << "Set " << ct++ << ": ";

    for (auto &block_id : set)
      cout << block_id << " ";
    cout << endl;
  }
}
