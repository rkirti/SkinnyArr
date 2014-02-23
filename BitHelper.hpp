#pragma once
#include <iostream>
#include <cstdlib>
#include <cassert> 

using namespace std;

inline bool getBit(int32_t num,int32_t i)
{
  return ((num & (1 << i)) != 0);
}

inline int32_t setBit(int32_t num, int32_t i)
{
  return num | ( 1 << i);
}

inline int32_t clearBit(int32_t num, int32_t i)
{
  int32_t mask = ~(1 << i);
  return num & mask;
}

inline int32_t updateBit(int32_t num,int32_t i,int32_t v)
{
  int32_t mask = ~(1 << i);
  return (num & mask) | ( v << i);
}

// i inclusive!
inline int32_t clearBitsMSBthroughI(int32_t num, int32_t i) 
{
  int32_t mask = ( 1 << (i+1)) - 1;
  return num & mask;
}

// i inclusive!
inline int32_t clearBitsIThrough0(int32_t num,int32_t i)
{
  int32_t mask = ~((1 << (i+1)) - 1);
  return num & mask;
}

// i inclusive
inline int32_t getBitsMSBThroughI(int32_t num, int32_t i)
{
  int32_t mask = ~(( 1 << (i+1)) - 1);
  return (num & mask) >> i;
}

// i inclusive
inline int32_t getBitsIThrough0(int32_t num,int32_t i)
{
  int32_t mask = (1 << (i+1)) - 1;
  return num & mask;
}

void printBitWise(int32_t num)
{ 
  for (int32_t ctr=(sizeof(int32_t)*8)-1;ctr >=0;ctr--)
    if (getBit(num,ctr))
      cout << "1";
    else 
      cout << "0";
  cout << endl;
  return;
}

void printBitWise(int32_t num, int numBitsPerElement)
{ 
  for (int32_t ctr=(sizeof(int32_t)*8)-1;ctr >=0;ctr--)
  {
    if (ctr % numBitsPerElement == 3)
      cout << " | ";
    if (getBit(num,ctr))
      cout << "1";
    else 
      cout << "0";
  }
  cout << endl;
  return;
}


// Get all bits between bits leftIdx and i (both inclusive)
// Bits 31....0 => Left..... Right (Big Endian Assumption)
int32_t getBits(int32_t num, int8_t leftIdx, int8_t rightIdx)
{
  // cout << (int)leftIdx << "-" << (int)rightIdx << endl;
  // Prepare a mask to get only required bits
  int32_t allOnes = ~0;
  int32_t left;
  if (leftIdx == 31)
    left  = 0;
  else
    left  = allOnes << (leftIdx+1);
  int32_t right = ((1<<rightIdx) -1);
  int32_t mask = ~(left | right);
  return (num & mask);
}

// Merge numToBeMerged into numToMergeInto at position between 
// bits j and i (both inclusive)
// Bits 31....0 => Left..... Right (Big Endian Assumption)
int32_t updateBits(int32_t numToBeMerged,int32_t numToMergeInto, int8_t leftIdx, int8_t rightIdx)
{
  // cout << "UpdateBits called" << endl;
  // cout << "Merge this "; printBitWise(numToBeMerged);
  // cout << "into this "; printBitWise(numToMergeInto);
  // Clear bits i through leftIdx in numToMergeInto
  int32_t allOnes = ~0;
  int32_t left;
  if (leftIdx == 31)
    left = 0;
  else 
    left  = allOnes << (leftIdx+1);
  // cout << "Left: "; printBitWise(left);
  int32_t right = ((1<<rightIdx) -1);
  int32_t mask =left | right;
  // cout << "Right: "; printBitWise(right);
  // cout << "MASK " ; printBitWise(mask);
  int32_t n_cleared = numToMergeInto & mask;
  // cout << "NCLEARED " ; printBitWise(n_cleared);
  int32_t m_shifted = numToBeMerged << rightIdx;
  int32_t res =(n_cleared | m_shifted);
  // cout << "UpdateBits result : "; printBitWise(res);
  return res;
}

int32_t getNumBitsNeeded(int32_t num)
{
  int32_t temp = num;
  int32_t bitsNeeded =1;
  while ((temp/2) >0)
  {
    temp = temp >> 1;
    bitsNeeded = bitsNeeded + 1;
  }    
  return bitsNeeded;
}


