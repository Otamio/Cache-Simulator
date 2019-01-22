#include "containers.hh"

BlockLRU::BlockLRU(unsigned n_sets) {
  numSets = n_sets;

  list<unsigned> l;
  map<unsigned, list<unsigned>::iterator> ma;
  for (unsigned i=0; i!=n_sets; ++i) {
    lists.push_back(l);
    maps.push_back(ma);
  }
}

void BlockLRU::initialize(unsigned n_sets) {
  this->numSets = n_sets;

  list<unsigned> l;
  map<unsigned, list<unsigned>::iterator> ma;
  for (unsigned i=0; i!=n_sets; ++i) {
    this->lists.push_back(l);
    this->maps.push_back(ma);
  }
}

unsigned BlockLRU::pop(unsigned set_id) {
  // Remove from list
  unsigned ret = this->lists[set_id].back();
  this->lists[set_id].pop_back();
  // Remove from map
  auto ret_it = this->maps[set_id].find(ret);
  this->maps[set_id].erase(ret_it);

  return ret;
}

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

void BlockLRU::push(unsigned set_id, unsigned block_id) {
  // Push to list
  this->lists[set_id].push_front(block_id);
  this->maps[set_id].insert(make_pair(block_id, this->lists[set_id].begin()));
}
