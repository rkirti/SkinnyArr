#include "AdaptiveIntArray.hpp"


void AdaptiveIntArray::insert(size_t index, int value)
{
  int64_t bitStart,bitEnd;
  bitStart = index*m_numBitsPerInt;
  bitEnd = bitStart + m_numBitsPerInt -1;
  //cout << "bitStart: " << bitStart 
  //     << "bitEnd:   " << bitEnd  <<  endl;
  bool doesCross; 
  size_t startIdx = bitStart >> 5;
  size_t endIdx = bitEnd >> 5;
  if (startIdx == endIdx)
    doesCross = false;
  else
    doesCross = true;
  if (doesCross)
  {
    cout << "Something went wrong" << endl;
    // FIXME: TBD
  }
  else 
  {
    // FIXME: Remove the magic numbers 5 & 32
    // to allow portability to int64_t or others

    // Single element to be picked up
    int8_t  startBit = 31 - (bitStart % 32);
    int8_t  endBit = startBit - (m_numBitsPerInt -1);
    //cout << "startBit: " <<  (int)startBit << "endBit: " 
    //     << (int)endBit << endl;
    m_storage[startIdx] = updateBits(value, m_storage[startIdx], startBit, endBit);
  }
}


int AdaptiveIntArray::get(size_t index) const
{
  int64_t startBitIdx,endBitIdx;

  // The desired int starts and ends at...
  startBitIdx = index*m_numBitsPerInt;
  endBitIdx = startBitIdx + m_numBitsPerInt -1;
  
  // Does it 'cross a boundary' in the underlying 
  // storage array?
  bool doesCross; 
  size_t startElemIdx = startBitIdx >> POWER_OF_2_USED_IN_STORAGE_ELEMENT;
  size_t endElemIdx = endBitIdx >> POWER_OF_2_USED_IN_STORAGE_ELEMENT;
  if (startElemIdx == endElemIdx)
    doesCross = false;
  else
    doesCross = true;
  
  if (doesCross)
  {
    // FIXME: TBD
  }
  else 
  {
    // Single element to be picked up
    int8_t  leftBitIdx = (NUM_BITS_PER_STORAGE_ELEMENT - 1) 
                    - (startBitIdx % NUM_BITS_PER_STORAGE_ELEMENT);
    int8_t  rightBitIdx = leftBitIdx - (m_numBitsPerInt -1);
    int res = getBits(m_storage[startElemIdx],leftBitIdx, rightBitIdx);
    res = res >> rightBitIdx;
    // FIXME: Right-shifting messes things up when the MSB is 1
    // Correcting for this using a mask now. Get a better solution later?
    int32_t mask = (1 << m_numBitsPerInt) - 1;
    res = res & mask;
    return res;
  }
}

size_t AdaptiveIntArray::find(int value) const
{

}


void AdaptiveIntArray::print(size_t startIndex, 
                             size_t endIndex)
{
  for (size_t ctr=startIndex;ctr<=endIndex;ctr++)
    printBitWise(m_storage[ctr]);
}


int main()
{
  // Toy test case: 8 bits *  16 elements = 128/32 
  // =  4 ints needed!
  AdaptiveIntArray* testArr = new AdaptiveIntArray(230,16);
  testArr->insert(0,12);
  testArr->insert(7,3);
  testArr->insert(5,11);
  testArr->insert(15,10);
  testArr->insert(12,10);
  cout << (int) testArr->get(0) << endl;
  cout << (int) testArr->get(7) << endl;
  cout << (int) testArr->get(5) << endl;
  cout << (int) testArr->get(15) << endl;
  cout << (int) testArr->get(12) << endl;
  // testArr->print(0,3);
  return 0;
}

