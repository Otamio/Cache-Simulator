#include "classes.hh"

// Initialize a DataBlock instance
DataBlock::DataBlock(Parameters &p) {
  size = p.getBlockSize();
  vector<double> tmp(size, 0);
  data.insert(data.end(), tmp.begin(), tmp.end());
}

// Print the DataBlock information to stdout (for debugging only)
void DataBlock::show() {
  for (auto &val : this->data)
    cout << val << ' ';
  cout << endl;
}

// Replace the DataBlock by the new datablock
void DataBlock::replace(DataBlock &block) {
  // Confirming the two datablocks have the same size
  if (this->size != block.size)
    throw "Unmatched block size when updating the block (code: 002).\n";

  this->data = block.data;
}
