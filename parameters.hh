using namespace::std;

class Parameters {
  friend ostream &print(ostream &os, const Parameters &p);
public:
  Parameters &read(int argc, char *argv[]);
private:
  // Default parameters
  unsigned long ram_size = 3840000ul;
  unsigned cache_size = 65536u, block_size = 64u, n_map = 2u, n_sets = 64u;
  string replacement_policy = "LRU";
  string algorithm = "mxm_block";
  unsigned dimension = 480u, blocking_factor = 32u;
  bool output = false;
};

ostream &print(ostream &os, const Parameters &p);
