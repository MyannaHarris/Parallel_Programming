/*Myanna Harris
 * seq.c
 * sort
 */

/*
 * This function 
 * sorts a list of numbers using algorithm
 * @param array: (starting address of) the list that needs to be sorted
 * @param lo: the lowest index. E.g., {}
 * @param hi: the highest index. E.g., nlocal-1
 * @ param flag: selects increasing or decrease order of the
 *  sorted list. 1: increasing; 0: decreasing
 */
void quicksort(int *array, int lo, int hi, int flag)  
{
  //flag should be 1 for increasing 
  int i = lo-1;
  int j = hi;
  int pivot= array[hi];
  int temp;
  if (hi>lo)
  {
    do
    {
      if(flag == 1)  /* a increase sort */
      {
        do i++; while (array[i]<pivot);
        do j--; while (array[j]>pivot);
      }
      else  /* a decrease sort */
      {
        do i++; while (array[i]>pivot);
        do j--; while (array[j]<pivot);
      }
      temp = array[i];        /* swap values */
      array[i] = array[j];
      array[j] = temp;
    } while (j>i);
    array[j] = array[i];  /* swap values */
    array[i] = pivot;
    array[hi] = temp;
    quicksort(array,lo,i-1,flag); /* recursive until hi == lo */
    quicksort(array,i+1,hi,flag);
  }
}

void bubbleSort(int *array, int n)
{
  int i, j, temp;
  for (i=0; i < (n-1); i++)
  {
    for (j=0; j < (n-1); j++)
    {
      //use < for decreasing order
      if (array[j] > array[j+1])
      {
        temp = array[j];
        array[j] = array[j+1];
        array[j+1] = temp;
      }
    }
  }
} 
