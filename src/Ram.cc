/* ./src/Ram.cc
 *
 *  class Ram is represented as a vector of DataBlocks.
 *  class Cache has direct control on class Ram.
 */
#include "classes.hh"

/* Initialize the Ram with the parameters */
Ram::Ram(Parameters &p, Rule *r) {
  // calculate the number of blocks, use upper ceiling to padding
  if (p.getRamSize() % p.getBlockSize() == 0)
    numBlocks = p.getRamSize() / p.getBlockSize();
  else
    numBlocks = p.getRamSize() / p.getBlockSize() + 1;
  // build the DataBlock and insert it into data
  DataBlock d(p);
  vector<DataBlock> tmp(numBlocks, d);
  data.insert(data.end(), tmp.begin(), tmp.end());
  // Pass pointer of rule to the constructor
  rule = r;
}

/* Print the ram state to stdout, debugging only */
void Ram::show() {
  unsigned ct = 0;
  for (auto &block : this->data) {
    cout << "block " << ct++ << " (size:" << block.getBlockSize() << "):";
    block.show();
  }
  cout << endl;
}

/* Print the summary statistics to stdout, debugging only */
void Ram::summary() {
  cout << "###### RAM Statistics ######\n";
  cout << "numBlocks: " << this->numBlocks << endl;
}

/* Obtain a block by giving an address */
DataBlock &Ram::getBlock(Address address) {
  unsigned BlockIndex = this->rule->getBlockIndexRAM(address);

  if (BlockIndex >= numBlocks+1) {
    cerr << "Error: Block Index: " << BlockIndex << " numBlocks: " << numBlocks << endl;
    throw runtime_error("Block Index out of Range (Code: 004).");
  }

  return this->data[BlockIndex];
}

/* Replace the block at the given address, with the given block reference */
void Ram::setBlock(Address address, DataBlock &block) {
  unsigned BlockIndex = this->rule->getBlockIndexRAM(address);

  this->data[BlockIndex].replace(block);
}
