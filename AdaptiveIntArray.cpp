#include "AdaptiveIntArray.hpp"
#include "TestAIA.hpp"

void AdaptiveIntArray::insert(size_t index, int value)
{
  // cout << "Inserting into index " << index << " "; printBitWise(value);
  int64_t startBitIdx,endBitIdx;
  bool doesCross; 

  // Determine where this integer starts and ends
  startBitIdx = index*m_numBitsPerInt;
  endBitIdx = startBitIdx + m_numBitsPerIntMinusOne;
  
  // Determine its position in the underlying int
  // storage array and if it crosses an int 
  // boundary there
  size_t startElemIdx = startBitIdx >> POWER_OF_2_USED_IN_STORAGE_ELEMENT;
  size_t endElemIdx = endBitIdx >> POWER_OF_2_USED_IN_STORAGE_ELEMENT;

  doesCross = (startElemIdx == endElemIdx)? false:true;
  
  if (doesCross)
  {
    int8_t numBitsLeft, numBitsRight;
    int8_t leftBitIdx, rightBitIdx;
    int value_actual,value_left, value_right;
    

    leftBitIdx = startBitIdx - (startElemIdx << POWER_OF_2_USED_IN_STORAGE_ELEMENT);
    leftBitIdx = (NUM_BITS_PER_STORAGE_ELEMENT_MINUS_ONE) - leftBitIdx;

    // Since we know this int crosses a boundary,
    // number of bits in the left element == leftBitIdx+1
    // (else the full int would fit into 1 element)
    numBitsLeft = leftBitIdx+1; 
    numBitsRight = m_numBitsPerInt - numBitsLeft;
    
    rightBitIdx = NUM_BITS_PER_STORAGE_ELEMENT - numBitsRight;

    value_right = getBits(value,numBitsRight-1,0);
    value_left = 
      getBits(value,m_numBitsPerIntMinusOne, m_numBitsPerInt -numBitsLeft); 
    value_left = value_left >> (m_numBitsPerInt - numBitsLeft);
    m_storage[startElemIdx] = updateBits(value_left, m_storage[startElemIdx],leftBitIdx,0);
    m_storage[endElemIdx] = updateBits(value_right, m_storage[endElemIdx],NUM_BITS_PER_STORAGE_ELEMENT_MINUS_ONE, rightBitIdx);
  }
  else 
  {
    // Single element to be picked up
    int8_t  startBit = (NUM_BITS_PER_STORAGE_ELEMENT_MINUS_ONE) - (startBitIdx % NUM_BITS_PER_STORAGE_ELEMENT);
    int8_t  endBitIdx = startBit - (m_numBitsPerIntMinusOne);
    m_storage[startElemIdx] = updateBits(value, m_storage[startElemIdx], startBit, endBitIdx);
  }
}


int AdaptiveIntArray::get(size_t index) const
{
  int64_t startBitIdx,endBitIdx;

  // The desired int starts and ends at...
  // FIXME: This data dependency on result 
  // of multiply costs 14% of total time. RETHINK!
  startBitIdx = index*m_numBitsPerInt;
  endBitIdx = startBitIdx + m_numBitsPerIntMinusOne;
  
  // Does it 'cross a boundary' in the underlying 
  // storage array?
  bool doesCross; 
  size_t startElemIdx = startBitIdx >> POWER_OF_2_USED_IN_STORAGE_ELEMENT;
  size_t endElemIdx = endBitIdx >> POWER_OF_2_USED_IN_STORAGE_ELEMENT;

  doesCross = (startElemIdx == endElemIdx)? false:true;

  if (doesCross)
  {
    int32_t value_left, value_right;
    int32_t mask;
    int8_t leftBitIdx, rightBitIdx;
    int8_t numBitsLeft, numBitsRight;
 
 
    leftBitIdx = startBitIdx - (startElemIdx << POWER_OF_2_USED_IN_STORAGE_ELEMENT);
    leftBitIdx = (NUM_BITS_PER_STORAGE_ELEMENT_MINUS_ONE) - leftBitIdx;

    // Since we know this int crosses a boundary,
    // number of bits in the left element == leftBitIdx+1
    // (else the full int would fit into 1 element)
    numBitsLeft = leftBitIdx+1; 
    numBitsRight = m_numBitsPerInt - numBitsLeft;
    
    rightBitIdx = NUM_BITS_PER_STORAGE_ELEMENT - numBitsRight;

    // Get the first part
    value_left = getBits(m_storage[startElemIdx],leftBitIdx,0);
    
    // Get the second part
    value_right = getBits(m_storage[endElemIdx], NUM_BITS_PER_STORAGE_ELEMENT_MINUS_ONE, rightBitIdx);
    
    // value_right needs shifting
    // FIXME: Right-shifting messes things up when the MSB is 1
    // Correcting for this using a mask now. Get a better solution later?
    value_right = value_right >> rightBitIdx;
    mask = (1 << numBitsRight) - 1;
    value_right = value_right & mask;

    // First part needs left shifting
    value_left = value_left << (NUM_BITS_PER_STORAGE_ELEMENT - rightBitIdx);
    int32_t res = value_left | value_right;
    return res;
  }
  else 
  {
    // Single element to be picked up
    int8_t leftBitIdx,rightBitIdx;
    leftBitIdx = startBitIdx - (startElemIdx << POWER_OF_2_USED_IN_STORAGE_ELEMENT);
    leftBitIdx = (NUM_BITS_PER_STORAGE_ELEMENT_MINUS_ONE) - leftBitIdx;
    rightBitIdx = leftBitIdx - (m_numBitsPerIntMinusOne);
    
    int32_t res = getBits(m_storage[startElemIdx],leftBitIdx, rightBitIdx);
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
  // FIXME: TBD
}


void AdaptiveIntArray::print(size_t startIndex, 
                             size_t endIndex)
{
  for (size_t ctr=startIndex;ctr<=endIndex;ctr++)
    printBitWise(m_storage[ctr]);
}

int main()
{
  testAIA(10,900000);
  testRegularIntArray(10,900000);
  return 0;
}

