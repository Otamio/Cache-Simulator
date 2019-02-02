/* Assumes each word is 8 bytes,
 *  and address is 32 bits */
#define ADDRESS_LEN 32
#define WORD_SIZE 8

#include <cstdint>
#include <iostream>
#include <climits>
#include <vector>
#include "../lib/containers.hh"

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
class Cache; // Cache (a vector of sets)
struct CPU; // CPU (instruction handler)


/* Struct Algorithms (algorithm brancher)
 *
 *  member functions (public):
 *    enter: Initialize the algorithm parameters, and enter the algorithm function (daxpy, mxmMult, or mxmMultBlock)
 *    daxpy: The daxpy algorithm
 *    mxmMult: The naive matrix by matrix multiplication algorithm
 *    mxmMultBlock: The blocking algorithm of matrix by matrix multiplication
 *    doBlock: Subroutine of mxmMultBlock
 *  private members:
 *    testsize (dim), pointers to rule, ram, result, and cache
 */
struct Algorithms {
  Algorithms() {}
  void enter(Parameters &params); // Initializer and branch handler
  void daxpy(Parameters &params); // Daxpy algorithm
  void mxmMult(Parameters &params); // mxmMult algorithm
  void mxmMultBlock(Parameters &params); // mxmMultBlock algorithm
  void doBlock(CPU &myCpu, const unsigned test_size, const unsigned blocking_factor,
    unsigned si, unsigned sj, unsigned sk, vector<Address> &a, vector<Address> &b, vector<Address> &c);
private:
  unsigned testsize;
  Rule *rule;
  Ram *ram;
  Result *result;
  Cache *cache;
};


/* class Parameters (stores the input parameters)
 *
 *  friend function: print(), show the input parameters
 *  public members:
 *    Parameters() {}: default constructor
 *    Parameters(int, char) {}: Constructor, produced by commandline arguments
 *    unsigned getRamSize(): Obtain the ram size
 *    unsigned getCacheSize(): Obtain the cache size
 *    unsigned get BlockSize(): Obtain the block size
 *    unsigned getSetCount(): Get the number of sets in the cache
 *    unsigned getSetSize(): Get associativity of the cache
 *    unsigned getTestSize(): Get the dimension of the test array/matrix
 *    unsigned getBlockingFactor(): Get the blocking factor of the function
 *    unsigned printOutput(): Print the output (results) to stdout
 *    unsigned getReplacePolicy(): Get the replacement policy as std::string
 *    unsigned getAlgorithm(): Get the algorithm as std::string
 *  private (the default values are specified by the problem set):
 *    ram_size (unsigned long), the ram sie in bytes, defaults to 3840000ul;
 *    cache_size (unsigned), the cache_size in bytes, defaults to 65536u;
 *    block_size (unsigned), the block_size in bytes, defaults to 64u;
 *    n_map (unsigned), the associativity, defaults to 2u;
 *    n_sets (unsigned), the number of sets, defaults to 512u;
 *    replacement_policy (std::string), the block in set replacement policy, defaults to LRU;
 *    algorithm (std::string), the algorithm used, defaults to mxm_block;
 *    dimension (unsigned), the dimension of the vector/matrix, defaults to 480u;
 *    blocking_factor (unsigned), the blocking_factor of the mxm_block algorithm;
 *    output (bool), whether to print out the vector/matrix;
 */
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
  inline unsigned getBlockingFactor() const { return this->blocking_factor; }
  inline bool printOutput() const { return this->output; }
  inline bool resetResult() const { return this->reset; }
  inline string &getReplacePolicy() { return this->replacement_policy; }
  inline string &getAlgorithm() { return this->algorithm; }
private:
  // The field values are filled with default parameters
  unsigned long ram_size = 3840000ul;
  unsigned cache_size = 65536u, block_size = 64u, n_map = 2u, n_sets = 512u;
  string replacement_policy = "LRU";
  string algorithm = "mxm_block";
  unsigned dimension = 480u, blocking_factor = 32u;
  bool output = false, reset=false;
};
/* Print the parameters to stdout */
ostream &print(ostream &os, const Parameters &p);

/* class Results
 *
 *  friend function: print()
 *  friend struct CPU, which has access to the reset() function;
 *
 *  public members:
 *   incremental functions: void instruction(), void read_hit(), void read_miss(), void write_hit(), void write_miss();
 *   The functions start with count_ returns the correponding statistics to the callee;
 *  private:
 *   The field values (instruction_count, read_hits, read_misses, write_hits, write_misses) that records runtime statistics.
 */
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
  // Reset the results (all valid bits set to 0)
  void reset(); // Only the CPU has access to this function
};
/* Print the results to stdout */
ostream &print(ostream &os, const Result &results);

/* class Rule, which shows the Addressing Rule
 *
 *  friend functions print the addressing rule or the information of address to stdout
 *
 *  public:
 *    Rule: Constructor;
 *    unsigned getBlockSize(), returns the block size, same as Parameters::getBlockSize()
 *    unsigned getBlockOffset(Address), returns the block offset (byte-precision)
 *    unsigned getBlockIndexRAM(Address), returns the block index of an address in the ram (ram is treated as directly mapped)
 *    unsigned getSetIndex(Address), returns the set inde of an address in the cache
 *    unsigned getTag(Address), returns the tag value of an address
 *  private:
 *    word_bit (unsigned): The word offset (by bits)
 *    block_size (unsigned): The size of a block (by bytes)
 *    set_size (unsigned): The size of a set (by blocks)
 *    set_count (unsigned): The number of sets in the cache (by sets)
 *    index_len (unsigned): The length of index (by bits)
 *    tag_len (unsigned): The length of tag (by bits)
 *    unsigned getBit(unsigned): Get the bit length when given the value size
 */
class Rule {
  friend ostream &print(ostream &os, const Rule &rule);
  friend ostream &print(ostream &os, const Rule &rule, Address address);
public:
  Rule(Parameters &p);
  inline unsigned getBlockSize() const { return this->block_size; }
  inline unsigned getBlockOffset(Address address) const { return address % this->block_size; }
  inline unsigned getBlockIndexRAM(Address address) const { return address>>offset_len; }
  inline unsigned getSetIndex(Address address) const { return (address>>offset_len) % this->set_count; }
  inline unsigned getTag(Address address) const { return address>>offset_len>>index_len; }
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

/* Print the address rule to stdout */
ostream &print(ostream &os, const Rule &rule);
/* Print the information of an address to stdout */
ostream &print(ostream &os, const Rule &rule, Address address);

/* Class DataBlock
 *
 *  friend class are cache and ram, which means CPU cannot directly access the datablock
 *
 *  public method:
 *    DataBlock(Parameters&): Constructor
 *    void show(): Print out the datablock status at runtime
 *    unsigned getBlockSize()
 *  private method:
 *    size (unsigned): The number of words in the block
 *    data (vector of double): data is stored in the vector as an array of doubles
 *    double get(unsigned): get a word from the block
 *    void set(unsigned, double): set the value of a word in the block
 */
class DataBlock {
  friend class Cache;
  friend class Ram;
public:
  DataBlock(Parameters &p);
  void show(); // debugging only
  inline unsigned getBlockSize() { return this->size; }
private:
  unsigned size;
  vector<double> data;
  // Restricted function that can only be accessed by cache and ram
  void replace(DataBlock &block);
  inline double get(unsigned block_offset) const { return this->data[block_offset/WORD_SIZE]; }
  inline void set(unsigned block_offset, double val) { this->data[block_offset/WORD_SIZE] = val; }
};

/* Class Ram
 *
 *  friend class Cache, which means only the cache can access the ram
 *
 *  public members:
 *    Ram (Parameters&. Rule*): Constructor
 *    void show(), print out the ram status at runtime
 *    unsigned summary(), print out the summary status of the ram (currently shows number of blocks only)
 *  private members:
 *    numBlocks (unsigned)
 *    data (vectors of DataBlock), this is how the data is designed
 *    rule (Rule *), this is the pointer to the addressing rule
 *    DataBlock &getBlock(Address), obtain a DataBlock from the RAM
 *    void setBlock(Address, DataBlock&), set a value of a DataBlock in the RAM
 */
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

/* Class Cache
 *
 *  friend struct CPU, CPU can directly controls Cache
 *
 *  public members:
 *    Cache(Parameters&, Rule*, Ram*, Result*): Construtor
 *    void show(), print the runtime status of the cache to stdout
 *    double getDouble(Address);
 *    void setDouble(Address);
 *  private members:
 *    Cache parameters: numSets (unsigned), numBlocks (unsigned), replacement_rule (unsigned: 0, 1, 2);
 *    Data: validBits, tags, blocks;
 *    Pointers to other class instance: rule, ram, result;
 *    BlockQueues: queue (FIFO) and lru_queue (LRU);
 *
 *    DataBlock &findBlock(Address, bool), find the datablock from the cache and returns its reference
 *    DataBlock &updateBlock(Address), finds the block in RAM and put it into Cache
 *    DataBlock &fetchBlock(Address), subroutine of updateBlock, fetch the target block from the RAM, and return its reference
 *    DataBlock &replaceBlock(Address), subroutine of updateBlock, replace one block in the cache, and return the new block's reference
 *    unsigned replaceRandom(), subroutine of replaceBlock, returns the random block number to replace
 */
class Cache {
  friend struct CPU;
public:
  Cache(Parameters &p, Rule *r, Ram *rm, Result *resu);
  void show(); // debug only
  double getDouble(Address address); // Get the double from cache
  void setDouble(Address address, double value);
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
  // Block queue (to trigger the replacement rule)
  BlockQueues queue;
  BlockLRU lru_queue;
  // Helper (private) functions
  DataBlock &findBlock(Address address, bool write); // Find the address entry in the block
  DataBlock &updateBlock(Address address); // Update the block containing the address into the cache
  inline DataBlock &fetchBlock(Address address) const {
    return this->ram->getBlock(address); } // Fetch the block from RAM
  DataBlock &replaceBlock(Address address); // Replace one block and return its reference
  inline unsigned replaceRandom() const { return rand() % this->numBlocks; }
  // Reset the cache (only accessible from CPU::reset())
  void reset();
};

/* struct Cpu
 *  public members:
 *   CPU(Cache*, Result*), constructor
 *   double loadDouble(Address), load the value from the address given
 *   void storeDouble(Address, double), store the value to the address given
 *   Register addDouble(Register, Register), return the sum of the two values in the Registers
 *   Register multDouble(Register, Register), return the product of the two values in the Registers
 *   void reset(), reset the result, and the validBits of the Cache
 */
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
