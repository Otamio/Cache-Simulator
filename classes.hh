#define ADDRESS_LEN 32
#define WORD_SIZE 8

#include <cstdint>
#include <iostream>
#include <climits>
#include <vector>
#include "containers.hh"

using namespace::std;

/* Type declarations: General */
struct Algorithms; // Contains the three algorithms used in this assignment
class Parameters; // Input parameters, and is able to be printed to stdout (has default values)
class Result; // Results (IC, Hit, Miss), and is able to be printed to stdout
typedef uint32_t Address; // Address has 32 bits
typedef double Register; // Register stores a double
/* Type declarations: The CPU components */
class Rule; // Addressing rule
class DataBlock; // DataBlock (with a vector of bytes)
class Ram; // Ram (a vector of DataBlocks)
class Cache; // Cache (a vector of Ram)
struct CPU; // CPU (instruction handler)


/* Algorithms (algorithm handler) */
struct Algorithms {
  Algorithms() {}
  void enter(Parameters &params); // Initializer and branch handler
  void daxpy(Parameters &params); // Daxpy algorithm
  void mxmMult(Parameters &params); // mxmMult algorithm
  void mxmMultBlock(Parameters &params); // mxmMultBlock algorithm
private:
  unsigned testsize;
  Rule *rule;
  Ram *ram;
  Result *result;
  Cache *cache;
};

/* Parameters */
class Parameters {
  friend ostream &print(ostream &os, const Parameters &p);
public:
  // Constructors
  Parameters() {};
  Parameters(int argc, char *argv[]);
  // Fetch specific parameters
  inline unsigned getRamSize() const { return this->ram_size; }
  inline unsigned getCacheSize() const { return this->cache_size; }
  inline unsigned getBlockSize() const { return this->block_size; }
  inline unsigned getSetCount() const { return this->n_sets; }
  inline unsigned getSetSize() const { return this->n_map; }
  inline unsigned getTestSize() const { return this->dimension; }
  inline bool printOutput() const { return this->output; }
  inline string &getReplacePolicy() { return this->replacement_policy; }
  inline string &getAlgorithm() { return this->algorithm; }
  // Parameters &read(int argc, char *argv[]); // Update the parameters after initialization
private:
  // The field values are filled with default parameters
  unsigned long ram_size = 3840000ul;
  unsigned cache_size = 65536u, block_size = 64u, n_map = 2u, n_sets = 512u;
  string replacement_policy = "LRU";
  string algorithm = "mxm_block";
  unsigned dimension = 480u, blocking_factor = 32u;
  bool output = false;
};

ostream &print(ostream &os, const Parameters &p);

/* Results */
class Result {
  friend ostream &print(ostream &os, const Result &results);
  friend struct CPU;
public:
  // Constructor (default)
  Result() {}
  // Increase counters
  inline void instruction() { ++this->instruction_count; }
  inline void read_hit() { ++this->read_hits; }
  inline void read_miss() { ++this->read_misses; }
  inline void write_hit() { ++this->write_hits; }
  inline void write_miss() { ++this->write_misses; }
  // Counting member functions (for printing out the result)
  inline unsigned count_instructions() const { return this->instruction_count; }
  inline unsigned count_read_hits() const { return this->read_hits; }
  inline unsigned count_read_misses() const { return this->read_misses; }
  inline double count_read_miss_rate() const { return static_cast<double>(this->read_misses) / (this->read_hits + this->read_misses); }
  inline unsigned count_write_hits() const { return this->write_hits; }
  inline unsigned count_write_misses() const { return this->write_misses; }
  inline double count_write_miss_rate() const { return static_cast<double>(this->write_misses) / (this->write_hits + this->write_misses); }
private:
  // Counting variables
  unsigned instruction_count = 0;
  unsigned read_hits = 0;
  unsigned read_misses = 0;
  unsigned write_hits = 0;
  unsigned write_misses = 0;
  // Reset the results (all field values set to 0)
  void reset();
};

ostream &print(ostream &os, const Result &results);

/* Class - Address_Rule */
class Rule {
  friend ostream &print(ostream &os, const Rule &rule);
  friend ostream &print(ostream &os, const Rule &rule, Address address);
public:
  Rule(Parameters &p);
  // Generate the tag, index, and offset
  // inline unsigned getTag();
  // inline unsigned getIndex();
  // inline unsigned getOffset();
  inline unsigned getBlockSize() const { return this->block_size; }
  inline unsigned getBlockOffset(Address address) const { return address % this->block_size; }
  inline unsigned getBlockIndexRAM(Address address) const { return address>>offset_len; }
  inline unsigned getSetIndex(Address address) const { return (address>>offset_len) % this->set_count; }
  inline unsigned getTag(Address address) const { return address>>offset_len>>index_len; }
  // inline unsigned getBlockIndex(Address address) const { return this->getOffset(address) / WORD_SIZE; }
  // inline unsigned getBlockIndexRAM(Address address) const { return address & (this->block_size-1); }

  // inline unsigned getTag(Address address) const {
  //   return (address>>(ADDRESS_LEN - this->tag_len)) & UINT_MAX; }
  // inline unsigned getIndex(Address address) const {
  //   return (address>>this->offset_len) & (this->set_size-1); }
  // inline unsigned getOffset(Address address) const {
  //   return address % (this->block_size * WORD_SIZE); }
private:
  unsigned word_bit;
  unsigned block_size;
  unsigned set_size;
  unsigned set_count;
  unsigned offset_len;
  unsigned index_len;
  unsigned tag_len;
  // Get the number of bits from the input
  unsigned getBit(unsigned val);
};

// Print the address rule to stdout
ostream &print(ostream &os, const Rule &rule);
// Print the information of an address to stdout
ostream &print(ostream &os, const Rule &rule, Address address);

/* Class DataBlock */
class DataBlock {
  friend class Cache;
  friend class Ram;
public:
  DataBlock(Parameters &p);
  void show(); // debugging only
private:
  unsigned size;
  vector<double> data;
  // Restricted function that can only be accessed by cache and ram
  void replace(DataBlock &block);
  inline double get(unsigned block_offset) const { return this->data[block_offset]; }
  inline void set(unsigned block_offset, double val) { this->data[block_offset] = val; }
};

/* Class Ram */
class Ram {
  friend class Cache;
public:
  Ram(Parameters &p, Rule *r);
  void show(); // debugging only
  void summary(); // debugging only
private:
  unsigned numBlocks;
  vector<DataBlock> data;
  Rule *rule;
  // Restricted function that can only be accessed by cache only
  DataBlock &getBlock(Address address);
  void setBlock(Address address, DataBlock &block);
};

/* Class Cache */
class Cache {
  friend struct CPU;
public:
  Cache(Parameters &p, Rule *r, Ram *rm, Result *resu);
  void show(); // debug only
  double getDouble(Address address); // Get the double from cache
  void setDouble(Address address, double value);
  // DataBlock &getBlock(Address address); // Get the block from cache
  // void setBlock(Address address, DataBlock &block);
private:
  // Cache Parameters
  unsigned numSets;
  unsigned numBlocks;
  unsigned replacement_rule;
  // Data (Stored in vectors)
  vector<vector<bool>> validBits;
  vector<vector<unsigned>> tags;
  vector<vector<DataBlock>> blocks;
  // Address rule, ram, and result
  Rule *rule;
  Ram *ram;
  Result *result;
  // Block
  BlockQueues queue;
  BlockLRU lru_queue;
  // Helper (private) functions
  DataBlock &findBlock(Address address, bool write); // Find the address entry in the block
  DataBlock &updateBlock(Address address); // Update the block containing the address into the cache
  inline DataBlock &fetchBlock(Address address) const {
    return this->ram->getBlock(address); } // Fetch the block from RAM
  DataBlock &replaceBlock(Address address); // Replace one block and return its reference
  // void setBlock(Address address, double value);
  inline unsigned replaceRandom() const { return rand() % this->numBlocks; }
  // unsigned replaceFIFO(unsigned set_id);
  // unsigned replaceLRU(unsigned set_id);
  // void updateLRU(unsigned set_id, unsigned block_id);
  // Reset the cache (only accessible from CPU::reset())
  void reset();
};

/* Class Cpu */
struct CPU {
  CPU(Cache *c, Result *r) { cache = c; result = r; }
  inline double loadDouble(Address &address) {
    this->result->instruction(); return this->cache->getDouble(address); }
  inline void storeDouble(Address &address, double value) {
    this->result->instruction(); this->cache->setDouble(address, value); }
  inline Register addDouble(Register value1, Register value2) {
    this->result->instruction(); return value1 + value2; }
  inline Register multDouble(Register value1, Register value2) {
    this->result->instruction(); return value1 * value2; }
  inline void reset() { this->cache->reset(); this->result->reset(); }
private:
  Cache *cache;
  Result *result;
};
