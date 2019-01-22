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
  // if (this->size != block.size)

  if (this->size != block.size) {
    cerr << "Old size " << this->size << " New size " << block.size << "." << endl;
    throw runtime_error("Error: Dimension Unmatch when updating the block (Code: 002).");
  }

  this->size = block.size;
  this->data = block.data;
}
