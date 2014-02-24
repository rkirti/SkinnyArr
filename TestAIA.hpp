/* Test to see comparative times taken by regular int 
 * array and adaptive int array
 */
#include <ctime>
#include "AdaptiveIntArray.hpp"

timespec start,end,timediff;

timespec diff(timespec start, timespec end)
{
	timespec temp;
	if ((end.tv_nsec-start.tv_nsec)<0) {
		temp.tv_sec = end.tv_sec-start.tv_sec-1;
		temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
	} else {
		temp.tv_sec = end.tv_sec-start.tv_sec;
		temp.tv_nsec = end.tv_nsec-start.tv_nsec;
	}
	return temp;
}

void testAIA(int8_t maxPowerOf2, int maxNumElements)
{
  int curPowerOf2 = 2;
  int curNumElements = 2;
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
  for (;curPowerOf2 <= maxPowerOf2;curPowerOf2++)
  {
    for (curNumElements=2;curNumElements<=maxNumElements;curNumElements+=1000)
    {
      int rangeMax = (1<< curPowerOf2) - 1;
      int testValue = rand() % (1<<curPowerOf2);
      // cout << "Testing: Range: " << rangeMax << " numElements: " << curNumElements << endl; 
      AdaptiveIntArray* testArr = new AdaptiveIntArray(rangeMax,curNumElements);
      for (int ctr=0;ctr<curNumElements;ctr++)
      {
        testArr->insert(ctr,testValue);
        assert(testArr->get(ctr) == testValue);
      }
      delete testArr;
    }
  }
 clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
 timediff = diff(start,end);
 cout << "AIA time: "<< timediff.tv_sec<<" s: "<<timediff.tv_nsec << " ns" << endl;;
 return;
}


void testRegularIntArray(int8_t maxPowerOf2, int maxNumElements)
{
  int curPowerOf2 = 2;
  int curNumElements = 2;
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
  for (;curPowerOf2 <= maxPowerOf2;curPowerOf2++)
  {
    for (curNumElements=2;curNumElements<=maxNumElements;curNumElements+=1000)
    {
      // rangeMax included here just to keep computation similar
      int rangeMax = (1<< curPowerOf2) - 1;
      int testValue = rand() % (1<<curPowerOf2);
      // cout << "Testing: Range: " << rangeMax << " numElements: " << curNumElements << endl; 
      int* testArr = (int*) malloc(sizeof(int)*curNumElements);
      for (int ctr=0;ctr<curNumElements;ctr++)
      {
        testArr[ctr] = testValue;
        assert(testArr[ctr] == testValue);
      }
      free(testArr);
    }
  }
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
  timediff = diff(start,end);
  cout << "Regular Int Array time: "<< timediff.tv_sec<<" s: "<<timediff.tv_nsec << " ns" << endl;;
  return;
}

