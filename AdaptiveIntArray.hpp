#pragma once
#include "BitHelper.hpp"
#include <iostream>
#include <cstdlib>

using namespace std;


/* Current Limitations: 
 * 1. Only unsigned ints handled
 * 2. Only ints <  32 bits handled
 * (so there is atmost one boundary 
 * crossing)
 * 3. Indexing begins at 0!
 * 4. Very high overhead for boolean 
 * 5. Big Endian assumption
 * use-case
 */
class AdaptiveIntArray
{
public:
  // Configured for int32_t storage
  enum {POWER_OF_2_USED_IN_STORAGE_ELEMENT = 5,
        NUM_BITS_PER_STORAGE_ELEMENT =32};
  // Takes two parameters - max 
  // int to store, and number of elements
  // in the array
  AdaptiveIntArray(int rangeMax, int numElements)
  {
    m_numBitsPerInt = getNumBitsNeeded(rangeMax);
    int64_t totalBits = m_numBitsPerInt*numElements;   
    if (totalBits % 32 == 0)
      m_storageSize = totalBits >> 5;
    else
      m_storageSize = (totalBits >> 5) + 1;
    m_storage = (int32_t*)malloc(sizeof(int32_t)*m_storageSize);
  }

  ~AdaptiveIntArray()
  {
    free(m_storage);
  }

  // inserts (append, not set) at specific index 
  // (last+1 expands)
  void insert(size_t index, int value);
  
  // return value at index
  int get(size_t index) const;         
  
  // returns -1 for not found
  size_t find(int value) const;     

  // Misc - for debug,test..
  void print(size_t startIndex, size_t endIndex);  

  int32_t getStorageSizeInInts()
  {
    return m_storageSize;
  }
    
  int8_t getNumBitsPerInt()
  {
    return m_numBitsPerInt;
  }
private:
  int8_t  m_numBitsPerInt; 
  int32_t m_storageSize;
  int32_t* m_storage;
};
