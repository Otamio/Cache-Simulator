#include "classes.hh"

// Constructor function
Cache::Cache(Parameters &p, Rule *r, Ram *rm, Result *resu) {
  // Determine Parameters
  numSets = p.getSetCount();
  numBlocks = p.getSetSize();

  // Insert datablocks
  DataBlock DataOneBlock(p);
  vector<DataBlock> oneSet(numBlocks, DataOneBlock);
  for (unsigned i=0; i != numSets; ++i)
    blocks.push_back(oneSet);

  // Initialize validBits and tags
  vector<bool> ValidBitOneSet(numBlocks, false);
  for (unsigned i=0; i != numSets; ++i)
    validBits.push_back(ValidBitOneSet);

  vector<unsigned> TagOneSet(numBlocks, 0);
  for (unsigned i=0; i != numSets; ++i)
    tags.push_back(TagOneSet);

  // Assign replacement rule
  if (p.getReplacePolicy() == "LRU") {
    replacement_rule = 0;
    lru_queue.initialize(numSets);
  } else if (p.getReplacePolicy() == "FIFO") {
    replacement_rule = 1;
    queue.initialize(numSets);
  } else if (p.getReplacePolicy() == "random")
    replacement_rule = 2;
  else
    throw string("Unknown replacement policy (Code: 003).\n");

  // Pass pointer to the constructor
  rule = r;
  ram = rm;
  result = resu;
}

// Display the memory contents in the cache
void Cache::show() {
  unsigned set_ct = 0;
  for (auto &s : this->blocks) {
    cout << "Set " << set_ct++ << " :\n";
    unsigned block_ct = 0;
    for (auto &b : s) {
      cout << "Block " << block_ct++ << " : ";
      b.show();
    }
  }
  cout << endl;
}

// Get the value from the address supplied (public member function)
double Cache::getDouble(Address address) {
  // Find the data in the blocks
  DataBlock &target = this->findBlock(address, false);
  // Get the in-block index
  unsigned offset = this->rule->getBlockOffset(address);

  // cout << "block_id " << block_id << endl;
  // target.show();

  // Obtain the value from DataBlock
  return target.get(offset);
}

// Write the value to the given address
void Cache::setDouble(Address address, double value) {
  // Find the block
  DataBlock &target = this->findBlock(address, true);
  // Get the in-block index
  unsigned offset = this->rule->getBlockOffset(address);

  // Update the value in the cache
  target.set(offset, value);
  // Also update the value in the ram
  this->ram->setBlock(address, target);

  // cout << "Address is " << address << endl;
  // cout << "Offset is " << offset << endl;

  // set the value
  // this->setBlock(address, value);
}

// Search the block in the cache
DataBlock &Cache::findBlock(Address address, bool write) {
  // get set_id
  unsigned set_id = this->rule->getSetIndex(address);
  // get tag
  unsigned tag = this->rule->getTag(address);

  /* Iterate to find the matching record */
  unsigned block_id = 0;

  for (auto blkTag : this->tags[set_id]) {
    // cout << "activate " << blkTag << " set " << set_id << endl;
    if (this->validBits[set_id][block_id] && blkTag == tag) {
      // New Block Found, record the state of block_id
      // Also Need to update the replacement queue when LRU is applied
      if (this->replacement_rule == 0) this->lru_queue.update(set_id, block_id);
      break;
    }
    ++block_id;
  }

  // cout << "Find result " << "Address " << address;
  // cout << " set_id " << set_id << " tag " << tag << " block_id " << block_id << endl;
  // cout << this->numBlocks << endl;

  /* Record the hit/miss */
  if (block_id != this->numBlocks) {
    // Hit
    if (write)
      this->result->write_hit();
    else
      this->result->read_hit();

    return this->blocks[set_id][block_id];
  } else {
    // Miss, call updateBlock
    if (write)
      this->result->write_miss();
    else
      this->result->read_miss();

    return this->updateBlock(address);
  }

  // End of function
}

// Fetch the block from RAM if miss occurs
DataBlock &Cache::updateBlock(Address address) {
  // get set_id
  unsigned set_id = this->rule->getSetIndex(address);
  // get the target block (that is moved from RAM)
  DataBlock &newblock = this->fetchBlock(address);

  /* Iterate to find the empty block */
  unsigned block_id = 0;

  for (auto blkValidBit : this->validBits[set_id]) {
    // Find an empty block
    if (!blkValidBit) {
      // Block Found
      this->validBits[set_id][block_id] = true; // turn to valid
      this->tags[set_id][block_id] = this->rule->getTag(address);
      this->blocks[set_id][block_id].replace(newblock);

      // Update the queue (if FIFO or LRU)
      // Push the entry into queue (if FIFO)
      if (this->replacement_rule == 1)
        this->queue.push(set_id, block_id);
      // Push the entry into LRU (if LRU)
      else if (this->replacement_rule == 0)
        this->lru_queue.push(set_id, block_id);

      // cout << "endl(2)" << block_id << endl;

      // Update complete, return the reference
      return this->blocks[set_id][block_id];
    }
    ++block_id;
  }

  // No empty block found, must replace one of the blocks
  return this->replaceBlock(address);
}

DataBlock &Cache::replaceBlock(Address address) {
  // get set_id
  unsigned set_id = this->rule->getSetIndex(address);
  // get the target block
  DataBlock &newblock = this->fetchBlock(address);

  unsigned block_id;
  // Determine the block to replace
  if (this->replacement_rule == 2)
    block_id = this->replaceRandom();
  else if (this->replacement_rule == 1)
    block_id = this->queue.pop(set_id);
  else if (this->replacement_rule == 0)
    block_id = this->lru_queue.pop(set_id);
  else
    throw string("Unknow Replacement Method (Code: 003).\n");

  // Update the block
  this->blocks[set_id][block_id].replace(newblock);
  this->tags[set_id][block_id] = this->rule->getTag(address);

  // return the reference
  return this->blocks[set_id][block_id];
}

// void Cache::setBlock(Address address, double value) {
//   // get the target Block, and fetch the block if it is not in the memory
//   DataBlock &target_block = this->findBlock(address, true);
//
//   // get the offset
//   unsigned block_id = this->rule->getBlockIndex(address);
//
//   // cout << "activated!! " << target_block.get(offset) << endl;
//   target_block.show();
//
//   // Write the value both the cache and ram
//   target_block.set(block_id, value);
//
//
//   this->ram->setBlock(address, target_block); // replace the whole block
// }

// unsigned Cache::replaceFIFO(unsigned set_id) {
//   unsigned ret = this->blockQueues[set_id].front();
//   this->blockQueues[set_id].pop_front();
//   return ret;
// }
//
// unsigned Cache::replaceLRU(unsigned set_id) {
//   unsigned ret = this->blockLists[set_id].back();
//   this->blockLists[set_id].pop_back();
//   // Find the map iterator, and remove the pair from map
//   auto it = this->blockMaps[set_id].find(ret);
//   this->blockMaps[set_id].erase(it);
//   return ret;
// }
//
// // Legacy code
// void Cache::updateLRU(unsigned set_id, unsigned block_id) {
//   return;
// }

// Reset (turn all validBits to false)
void Cache::reset() {
  for (auto &BitBlock : this->validBits)
    for (auto it=BitBlock.begin(); it!=BitBlock.end(); ++it)
      (*it) = false;
}
