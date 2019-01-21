#include <iostream>
#include "classes.hh"

using namespace::std;

// Address::Address(unsigned block_size, unsigned cache_size) {
//   // Increment  offset_size
//   for ( ; block_size & 0x1; block_size = block_size >> 1)
//     ++offset_size;
//   // Increment index_size
//   unsigned bucs = cache_size / block_size;
//   for ( ; bucs & 0x1; bucs = bucs >> 1)
//     ++index_size;
//
//   tag_size = ADDRESS_LEN - offset_size - index_size;
//
//   return add;
// }

/* Initialize the addressing rule from the parameters */
Rule::Rule(Parameters &p) {
  word_bit = getBit(WORD_SIZE);
  block_size = p.getBlockSize();
  set_size = p.getSetSize();
  set_count = p.getSetCount();
  offset_len = getBit(block_size);
  index_len = getBit(set_count);
  tag_len = ADDRESS_LEN - offset_len - index_len;
}

/* Get bit-length of a value */
unsigned Rule::getBit(unsigned val) {

  unsigned ret = 0;
  for ( ; val != 0; val = val>>1)
    ++ret;

  return ret-1;
}

/* Print out the addressing rule */
ostream &print(ostream &os, const Rule &rule) {
  // os << "=======================================\n";
  // os << "Address: " << address.address << "\n";
  // os << "=======================================\n";
  os << "###### Address Rule ######\n";
  os << "Block size: " << rule.block_size << " bytes\n";
  os << "Set size: " << rule.set_size << " bins\n";
  os << "Offset length: " << rule.offset_len << "\n";
  os << "Index length: " << rule.index_len << "\n";
  os << "Tag length: " << rule.tag_len << endl;

  return os;
}

/* Print out the addressing information */
ostream &print(ostream &os, const Rule &rule, Address address) {
  os << "###### Print out address information ######\n";
  os << "Address " << address << '\n';
  os << "Offset " << rule.getBlockOffset(address) << '\n';
  os << "Index(RAM) " << rule.getBlockIndexRAM(address) << '\n';
  os << "Tag " << rule.getTag(address) << endl;

  return os;
}
