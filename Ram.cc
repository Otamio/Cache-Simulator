#include "classes.hh"

/* Initialize the Ram with the parameters */
Ram::Ram(Parameters &p, Rule *r) {
  // calculate the number of blocks
  numBlocks = p.getRamSize() / p.getBlockSize();
  // build the DataBlock and insert it into data
  DataBlock d(p);
  vector<DataBlock> tmp(numBlocks+1, d); // Use upper ceiling
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
    // if (ct >= 100) break;
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

  // cout << "Block id:" << BlockIndex << " Address: " << address << endl;
  if (BlockIndex >= numBlocks+1) {
    cerr << "Error: Block Index: " << BlockIndex << " numBlocks: " << numBlocks << endl;
    throw runtime_error("Block Index out of Range (Code: 004).");
  }

  return this->data[BlockIndex];
}

void Ram::setBlock(Address address, DataBlock &block) {
  unsigned BlockIndex = this->rule->getBlockIndexRAM(address);

  // cout << "Address " << address << " Block size " << this->data[BlockIndex].getBlockSize() << endl;
  this->data[BlockIndex].replace(block);
}
