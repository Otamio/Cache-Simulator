#include <vector>
#include <deque>
#include <list>
#include <unordered_map>

using namespace::std;

/* Type declarations: Advanced Data Structures */

/* BlockQueue is a vector of set queues (of block numbers),
 *  set queue uses the policy: FIFO,
 *  it records the sequence of the block numbers being used.
 * It has the following members:
 *  Public:
 *    BlockQueues(): Default constructor, should be used with caution;
 *    BlockQueues(unsigned): Constructor by specifying the number of sets in the BlockQueue
 *    void initialize(unsigned): Fill a BLockQueue with the number of sets given, can only be used with numSets=0
 *    unsigned pop(unsigned): Pop a block from the set number given
 *    void push(unsigned, unsigned): Push a block into the set queue given
 *    void show(): Print out the memory status of the data structure at runtime
 *  Private:
 *    queues: collection of set queues, each queue is represented as deque
 *    numSets: the number of sets in the data structures
 */
class BlockQueues {
public:
  BlockQueues() {};
  BlockQueues(unsigned n_sets);
  void initialize(unsigned n_sets);
  unsigned pop(unsigned set_id);
  void push(unsigned set_id, unsigned block_id);
  void show();
private:
  vector<deque<unsigned>> queues;
  unsigned numSets = 0;
};

/* BlockLRU is a list of set queues (of block numbers), with unordered_map for fast indexing
 *  set queue uses the policy: LRU,
 *  it records the sequence of the block numbers being used.
 * It has the following members:
 *  Public:
 *    BlockLRU(): Default constructor, should be used with caution;
 *    BlockLRU(unsigned): Constructor by specifying the number of sets in the BlockLRU
 *    void initialize(unsigned): Fill a BlockLRU with the number of sets given, can only be used with numSets=0
 *    unsigned pop(unsigned): Pop a block from the set number given
 *    void update(unsigned, unsigned): Update the order of a block of a set sequence (to lowest)
 *    void push(unsigned, unsigned): Push a block into the set queue given
 *    void show(): Print out the memory status of the data structure at runtime
 *  Private:
 *    lists: collection of set queues, each queue is represented as a list
 *    maps: an unordered_map of <block_id, block_id iterator> pairs
 *    numSets: the number of sets in the data structures
 */
class BlockLRU {
public:
  BlockLRU() {};
  BlockLRU(unsigned n_sets);
  void initialize(unsigned n_sets);
  unsigned pop(unsigned set_id);
  void update(unsigned set_id, unsigned block_id);
  void push(unsigned set_id, unsigned block_id);
  void show();
private:
  vector<list<unsigned>> lists;
  vector<unordered_map<unsigned, list<unsigned>::iterator>> maps;
  unsigned numSets = 0;
};
