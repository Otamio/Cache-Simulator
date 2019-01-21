#include <vector>
#include <deque>
#include <list>
#include <map>

using namespace::std;

/* Type declarations: Advanced Data Structures */
class BlockQueues {
public:
  BlockQueues() {};
  BlockQueues(unsigned n_sets);
  void initialize(unsigned n_sets);
  unsigned pop(unsigned set_id);
  void push(unsigned set_id, unsigned block_id);
private:
  vector<deque<unsigned>> queues;
  unsigned numSets;
};

class BlockLRU {
public:
  BlockLRU() {};
  BlockLRU(unsigned n_sets);
  void initialize(unsigned n_sets);
  unsigned pop(unsigned set_id);
  void update(unsigned set_id, unsigned block_id);
  void push(unsigned set_id, unsigned block_id);
private:
  vector<list<unsigned>> lists;
  vector<map<unsigned, list<unsigned>::iterator>> maps;
  unsigned numSets;
};
