#include "simple_huffman.h"
#include "byte.h"
class node{
  private:
     node *left, *right;
     int count;       // ref. count
     int code;        // code in bits
     int codelength;  // code length
     BYTE chr;        // ASCII char, serves in compression and then in writing of frequencies
     node(void) {memset(this, 0, sizeof(node));} // constructor, memset is faster than individ.
     ~node() {if (left) delete left; if (right) delete right;}
     friend class simple_Huffman;
};
