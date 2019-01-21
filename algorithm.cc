#include "classes.hh"

/* Algorithm Start Function */
void Algorithms::enter(Parameters &params) {
  /* Print the parameters to stdout */
  print(cout, params);

  /* Build the components of the CPU */
  Rule rule(params);
  Ram ram(params, &rule);
  Result result;
  Cache cache(params, &rule, &ram, &result);
  // Bind the components with the class
  this->testsize = params.getTestSize();
  this->rule = &rule;
  this->ram = &ram;
  this->result = &result;
  this->cache = &cache;

  /* Go to the algorithm */
  if (params.getAlgorithm() == "mxm_block")
    this->mxmMultBlock(params);
  else if (params.getAlgorithm() == "mxm")
    this->mxmMult(params);
  else if (params.getAlgorithm() == "daxpy")
    this->daxpy(params);
  else
    throw "Unknow Algorithm (Code: 001). Abort.\n";
}

void Algorithms::daxpy(Parameters &params) {
  /* Assemble my CPU */
  CPU myCpu(this->cache, this->result);
  /* Determine the test size */
  const unsigned test_size = this->testsize;

  /* Initialize the addresses */
  unsigned ct = 0;
  vector<Address> a(test_size);
  vector<Address> b(test_size);
  vector<Address> c(test_size);
  for (auto &add : a)
    add = ct++ * WORD_SIZE;
  for (auto &add : b)
    add = ct++ * WORD_SIZE;
  for (auto &add : c)
    add = ct++ * WORD_SIZE;

  /* Initialize some dummy values */
  unsigned i = 0;
  for (auto it=a.begin(); it!=a.end(); ++it)
    myCpu.storeDouble(*it, i++);

  i = 0;
  for (auto it=b.begin(); it!=b.end(); ++it)
    myCpu.storeDouble(*it, 2*i++);

  for (auto it=c.begin(); it!=c.end(); ++it)
    myCpu.storeDouble(*it, 0);

  // runtime inspecter
  // this->ram->show();

  /* Reset the CPU to restart counting */
  myCpu.reset();

  // Put a random 'D' into a register
  Register r0 = 3, r1, r2, r3, r4;

  // Start iterating
  for (auto ind=0; ind!=test_size; ++ind) {
    r1 = myCpu.loadDouble(a[ind]);
    r2 = myCpu.multDouble(r0, r1);
    r3 = myCpu.loadDouble(b[ind]);
    r4 = myCpu.addDouble(r2, r3);
    myCpu.storeDouble(c[ind], r4);
  }

  // Print out the result
  print(cout, *(this->result));

  // Correctness check
  if (params.printOutput()) {
    cout << "##############################################\n";
    cout << "-Daxpy- Correctness Check, Multiplier D=" << r0 << "\n";
    cout << "Vector A: ";
    for (auto address : a)
      cout << myCpu.loadDouble(address) << " ";
    cout << "\nVector B: ";
    for (auto address : b)
      cout << myCpu.loadDouble(address) << " ";
    cout << "\nVector C: ";
    for (auto address : c)
      cout << myCpu.loadDouble(address) << " ";
    cout << endl;
  }

  return;
}

void Algorithms::mxmMult(Parameters &params) {
  /* Assemble my CPU */
  CPU myCpu(this->cache, this->result);
  /* Determine the test size */
  const unsigned test_size = this->testsize;

  // Initialize the addresses
  unsigned ct = 0;
  vector<Address> a(test_size*test_size);
  vector<Address> b(test_size*test_size);
  vector<Address> c(test_size*test_size);

  // Assigning addresses
  for (auto &add : a)
    add = ct++ * WORD_SIZE;
  for (auto &add : b)
    add = ct++ * WORD_SIZE;
  for (auto &add : c)
    add = ct++ * WORD_SIZE;

  // Initialize some dummy values
  unsigned i = 0;
  for (auto it=a.begin(); it!=a.end(); ++it)
    myCpu.storeDouble(*it, i++);

  i = 0;
  for (auto it=b.begin(); it!=b.end(); ++it)
    myCpu.storeDouble(*it, 2*i++);

  for (auto it=c.begin(); it!=c.end(); ++it)
    myCpu.storeDouble(*it, 0);

  myCpu.reset();

  Register r0, r1, r2, r3;

  // Start iterating
  for (auto row=0; row!=test_size; ++row) {
    for (auto col=0; col!=test_size; ++col) {
      r0 = 0; // r0 is the accumulator;
      for (auto k=0; k!=test_size; ++k) {
        r1 = myCpu.loadDouble(a[row*test_size + k]);
        r2 = myCpu.loadDouble(b[k*test_size + col]);
        r3 = myCpu.multDouble(r1, r2);
        r0 = myCpu.addDouble(r0, r3);
      }
      myCpu.storeDouble(c[row*test_size+col], r0);
    }
  }

  print(cout, *(this->result));

  // Correctness check
  if (params.printOutput()) {
    cout << "### mxm Correctness Check\n";

    cout << "Matrix A:\n";
    for (auto ri=0; ri!=test_size; ++ri) {
      for (auto ci=0; ci!=test_size; ++ci)
        cout << myCpu.loadDouble(a[ri*test_size+ci]) << " ";
      cout << "\n";
    }

    cout << "Matrix B:\n";
    for (auto ri=0; ri!=test_size; ++ri) {
      for (auto ci=0; ci!=test_size; ++ci)
        cout << myCpu.loadDouble(b[ri*test_size+ci]) << " ";
      cout << "\n";
    }

    cout << "Matrix C:\n";
    for (auto ri=0; ri!=test_size; ++ri) {
      for (auto ci=0; ci!=test_size; ++ci)
        cout << myCpu.loadDouble(c[ri*test_size+ci]) << " ";
      cout << "\n";
    }
  }

  return;
}

void Algorithms::mxmMultBlock(Parameters &params) {
  /* Assemble my CPU */
  CPU myCpu(this->cache, this->result);
  /* Determine the test size */
  const unsigned test_size = this->testsize;

  // Initialize the addresses
  unsigned ct = 0;
  vector<Address> a(test_size*test_size);
  vector<Address> b(test_size*test_size);
  vector<Address> c(test_size*test_size);

  // Assigning addresses
  for (auto &add : a)
    add = ct++ * WORD_SIZE;
  for (auto &add : b)
    add = ct++ * WORD_SIZE;
  for (auto &add : c)
    add = ct++ * WORD_SIZE;

  // cout << "Pass(0)" << endl;

  // Initialize some dummy values
  unsigned i = 0;
  for (auto it=a.begin(); it!=a.end(); ++it)
    myCpu.storeDouble(*it, i++);

  // cout << "Pass(1)" << endl;

  i = 0;
  for (auto it=b.begin(); it!=b.end(); ++it)
    myCpu.storeDouble(*it, 2*i++);

  for (auto it=c.begin(); it!=c.end(); ++it)
    myCpu.storeDouble(*it, 0);

  myCpu.reset();

  Register r0, r1, r2, r3;

  // Start iterating
  for (auto row=0; row!=test_size; ++row) {
    for (auto col=0; col!=test_size; ++col) {
      r0 = 0; // r0 is the accumulator;
      for (auto k=0; k!=test_size; ++k) {
        r1 = myCpu.loadDouble(a[row*test_size + k]);
        r2 = myCpu.loadDouble(b[k*test_size + col]);
        r3 = myCpu.multDouble(r1, r2);
        r0 = myCpu.addDouble(r0, r3);
      }
      myCpu.storeDouble(c[row*test_size+col], r0);
    }
  }

  print(cout, *(this->result));

  // Correctness check
  if (params.printOutput()) {
    cout << "### Blocked mxm Correctness Check\n";

    cout << "Matrix A:\n";
    for (auto ri=0; ri!=test_size; ++ri) {
      for (auto ci=0; ci!=test_size; ++ci)
        cout << myCpu.loadDouble(a[ri*test_size+ci]) << " ";
      cout << "\n";
    }

    cout << "Matrix B:\n";
    for (auto ri=0; ri!=test_size; ++ri) {
      for (auto ci=0; ci!=test_size; ++ci)
        cout << myCpu.loadDouble(b[ri*test_size+ci]) << " ";
      cout << "\n";
    }

    cout << "Matrix C:\n";
    for (auto ri=0; ri!=test_size; ++ri) {
      for (auto ci=0; ci!=test_size; ++ci)
        cout << myCpu.loadDouble(c[ri*test_size+ci]) << " ";
      cout << "\n";
    }
  }

  return;
}
