#ifndef __SKETCH_H
#define __SKETCH_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <malloc.h>
#include <string.h>
#include <stdint.h>
#include "zmalloc.h"


// define some constants
//# define LONG_PRIME 4294967311L
# define LONG_PRIME 29123
# define MIN(a,b) (a < b ? a : b)
#define MAX_VALUE 2147483647

// width, depth 
  unsigned int w,d;
  
  // eps (for error), 0.01 < eps < 1
  // the smaller the better
  float eps;
  
  // gamma (probability for accuracy), 0 < gamma < 1
  // the bigger the better
  float gamm;
  
  // aj, bj \in Z_p
  // both elements of fild Z_p used in generation of hash
  // function
  unsigned int aj, bj;

  // total count so far
  unsigned int total; 

  // array of arrays of counters
  uint8_t **table;

  // array of hash values for a particular item 
  // contains two element arrays {aj,bj}
  int **hashes;

  int times;

  // generate "new" aj,bj
  void genajbj(int **hashes, int i);


  // constructor
  void ensureCapacity(float eps, float gamma);
  
  // update item (int) by count c
  void updateint(int item, int c);
  // update item (string) by count c
  void increment(const char *item, int c);

  // estimate count of item i and return count
  unsigned int estimateint(int item);
  unsigned int frequency(const char *item);

  // return total count
  //unsigned int totalcount();

  // generates a hash value for a string
  // same as djb2 hash function
  unsigned int hashstr(const char *str);

  // destructor
  void freeCountMinSketch();

#endif


 
