#include "sketch.h" 


// CountMinSketch constructor
// ep -> error 0.01 < ep < 1 (the smaller the better)
// gamma -> probability for error (the smaller the better) 0 < gamm < 1



void ensureCapacity(float ep, float gamm) {

  eps = ep;
  gamm = gamm;
  w = ceil(exp(1)/eps);
  d = ceil(log(1/gamm));
  //w=512;
  //d=4;
  //printf("w:%d d:%d\n",w,d);;
  total = 0;
  times=0;
  // initialize counter array of arrays, C
  //C = new int *[d];
  table = zmalloc(sizeof(uint8_t*)*d);

  unsigned int i, j;
  for (i = 0; i < d; i++) {
    //C[i] = new int[w];
    table[i]=zmalloc(sizeof(uint8_t)*w);
    for (j = 0; j < w; j++) {
      table[i][j] = 0;
    }
  }
  // initialize d pairwise independent hashes
  srand(time(NULL));
  hashes = zmalloc(sizeof(int*)*d);
  for (i = 0; i < d; i++) {
    hashes[i] = zmalloc(sizeof(int)*2);
    genajbj(hashes, i);
  }
}

void reset()
{
  int i;
  for(i=0;i<d;i++)
    memset(table[i],0,sizeof(uint8_t)*w);
  times++;
}

// CountMinSkectch destructor
void freeCountMinSketch() {
  // free array of counters, C
  unsigned int i;
  for (i = 0; i < d; i++) {
    free (table[i]);
  }
  free (table);
  
  // free array of hash values
  for (i = 0; i < d; i++) {
    free (hashes[i]);
  }
  free (hashes);
}


// countMinSketch update item count (int)
void updateint(int item, int c) {
  total = total + c;
  if(total==100000)
  {
    reset();
    total=0;
  }
  //printf("%d\n",times);
  unsigned int hashval = 0;
  int j;
  for (j = 0; j < d; j++) {
  hashval = ((long)hashes[j][0]*item+hashes[j][1])%LONG_PRIME;
    
	hashval = hashval%w;
  //printf("up%d:%d\n",j,hashval);
   table[j][hashval]=table[j][hashval] + c;
  }
}


// countMinSketch update item count (string)
void increment(const char *str, int c) {
  double r = (double)rand()/RAND_MAX;
  int counter=frequency(str);
  
  double p = 1.0/(counter*10+1);
  if (r < p)
  {
  int hashval = hashstr(str);
  updateint(hashval, c);
  }
  //int hashval = hashstr(str);
  //updateint(hashval, c);
}

// CountMinSketch estimate item count (int)
unsigned int estimateint(int item) {
  int minval = MAX_VALUE;
  unsigned int hashval = 0;
  int j;
  for (j = 0; j < d; j++) {
    hashval = ((long)hashes[j][0]*item+hashes[j][1])%LONG_PRIME;
    hashval = hashval%w;
    //printf("es:%d\n",hashval);
    minval = MIN(minval, table[j][hashval]);
  }
  return minval;
}

// CountMinSketch estimate item count (string)
unsigned int frequency(const char *str) {
  int hashval = hashstr(str);
  return estimateint(hashval);
}

// generates aj,bj from field Z_p for use in hashing
void genajbj(int** hashes, int i) {
  hashes[i][0] = (int)((float)(rand())*(float)(LONG_PRIME)/(float)(RAND_MAX) + 1);
  hashes[i][1] = (int)((float)(rand())*(float)(LONG_PRIME)/(float)(RAND_MAX) + 1);
}

// generates a hash value for a sting
// same as djb2 hash function
unsigned int hashstr(const char *str) {
  unsigned long hash = 5381;
  int c;
  while (c = *str++) {
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
  }
  return hash;
}

/*int main()
{
	CountMinSketch(0.1, 0.001);
	char str[5]={'0'};

	int err=0,i,j;

	for(i=0;i<100;i++)

	{ 

	sprintf(str, "lru:%d",i ); 

    updatestr(str,1);

	err+=abs(estimatestr(str)-1);
    //printf("%d\n",estimatestr(str));
	} 
	//printf("%d\n",estimatestr("0"));
	//printf("%d\n",estimatestr("1"));
	//printf("%d\n",estimatestr("1"));

	printf("%d\n",err);
	printf("w:%d d:%d\n",w,d);
	freeCountMinSketch();
	return;
	
}*/
