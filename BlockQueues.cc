#include "containers.hh"

BlockQueues::BlockQueues(unsigned n_sets) {
  numSets = n_sets;

  vector<deque<unsigned>> queues;

  deque<unsigned> q;
  for (unsigned i=0; i!=n_sets; ++i)
    queues.push_back(q);
}

void BlockQueues::initialize(unsigned n_sets) {
  this->numSets = n_sets;

  deque<unsigned> q;
  for (unsigned i=0; i!=n_sets; ++i)
    this->queues.push_back(q);
}

unsigned BlockQueues::pop(unsigned set_id) {
  unsigned ret = this->queues[set_id].back();
  this->queues[set_id].pop_back();
  return ret;
}

void BlockQueues::push(unsigned set_id, unsigned block_id) {
  this->queues[set_id].push_front(block_id);
}
