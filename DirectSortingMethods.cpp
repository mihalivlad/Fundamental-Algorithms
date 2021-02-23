/*
Best Case: Sorted array as input
Worst Case: Reversely sorted array as input, except for selection case
Average Case: Random order as input repeated m times

I use correctly and efficiently 3 direct sorting methods (Bubble Sort,
linear-Insertion Sort and Selection Sort)

For Best Case:
Bubble Sort: Total: n    Comparison: n  Assignment: 0
Insertion Sort: Total: n     Comparison: n   Assignment: n
Selection Sort: Total: n^2   Comparison: n^2   Assignment: 0

Number of operations: bubble sort complexity is linear and better then insertion
which is also linear but 3 times slower. Selection sort complexity is quadratic and is worst then other two.

Number of assignments: bubble sort complexity and selection sort complexity is 0 and for linear
is 2*n complexity

Number of comparisons: bubble sort complexity is linear and equal with insertion.
Selection sort complexity is quadratic and is worst then other two.



For Average Case:
Bubble Sort: Total: n^2    Comparison: n^2  Assignment: n^2
Insertion Sort: Total: n^2     Comparison: n^2   Assignment: n^2
Selection Sort: Total: n^2   Comparison: n^2   Assignment: n

Number of operations: insertion sort complexity is quadratic and little better then selection.
Bubble sort complexity is quadratic and is worst then other two (3 times slower).

Number of assignments: selection sort complexity is linear and insertion sort complexity is quadratic.
3 times slower then insertion is bubble sort, also quadratic.

Number of comparisons: selection sort complexity is quadratic and little better then insertion.
2 times slower then insertion is bubble sort, also quadratic.



For Worst Case:
Bubble Sort: Total: n^2    Comparison: n^2  Assignment: 0
Insertion Sort: Total: n^2     Comparison: n^2   Assignment: 0
Selection Sort: Total: n^2   Comparison: n^2   Assignment: n

Number of operations: selection sort complexity is quadratic and 2 times better then insertion.
Bubble sort complexity is quadratic and is worst then other two (2 times slower then insertion).

Number of assignments: selection sort complexity is linear and insertion sort complexity is quadratic.
3 times slower then insertion is bubble sort, also quadratic.

Number of comparisons: selection sort complexity is quadratic and little better then insertion.
2 times slower then insertion is bubble sort, also quadratic.



Stability:
Bubble Sort: It is stable because of this comparison: arr[j] > arr[j+1]
Insertion Sort: It is stable because of this comparison: temp < arr[j-1]
Selection Sort: You can not tell if it is stable

*/

#include <iostream>
#include "Profiler.h"

#define MAX_SIZE 10000
#define m 5
using namespace std;

Profiler profiler("avgSort");

void cop(int arr1[], int arr2[], int n)
{
    for (int i = 0; i < n; i++)
        arr1[i]=arr2[i];
}

void bubbleSort(int arr[], int n)
{
    int ok=1,j;
    Operation oAss = profiler.createOperation("bubbleSortAss", n);
    Operation oCom = profiler.createOperation("bubbleSortCom", n);
    //oCom.count();
    //oAss.count();
    while(ok)
    {
        ok=0;
        //oAss.count(2);
        //oCom.count(2);
        for (j = 0; j < n-1; j++)
        {
            //oAss.count();
            oCom.count();//oCom.count(2);
            if (arr[j] > arr[j+1])
            {
                oAss.count(3);
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1]= temp;
                ok=1;
            }
        }
    }
}

void insertionSort(int arr[], int n)
{
    int i, j;
    Operation oAss = profiler.createOperation("insertionSortAss", n);
    Operation oCom = profiler.createOperation("insertionSortCom", n);
    //oCom.count();
    //oAss.count();
    for( int i = 1 ; i < n ; i++ )
    {
        oAss.count(2);//oAss.count(4);
        oCom.count();//oCom.count(3);
        int temp = arr[i];
        int j = i;
        while(  j > 0  && temp < arr[ j -1])
        {
            oAss.count();//oAss.count(2);
            oCom.count();//oCom.count(2);
            arr[j] = arr[j-1];
            j= j - 1;

        }
        arr[j] = temp;
    }
}

void selectionSort(int arr[], int n)
{
    Operation oAss = profiler.createOperation("selectionSortAss", n);
    Operation oCom = profiler.createOperation("selectionSortCom", n);
    //oCom.count();
    //oAss.count();
    int i, j, min_idx;
    for (i = 0; i < n-1; i++)
    {
        //oCom.count(2);
        min_idx = i;
        for (j = i+1; j < n; j++)
        {
            //oAss.count();
            oCom.count();//oCom.count(2);
            if (arr[j] < arr[min_idx])
            {
                //oAss.count();
                min_idx = j;
            }

        }
        if(min_idx != i)
        {
        oAss.count(3);//oAss.count(6);
        int temp = arr[i];
        arr[i] = arr[min_idx];
        arr[min_idx]= temp;
        }
    }
}
void print_array(int a[],int n)
{
    for(int i=0; i<n; i++)
        cout<<a[i]<<" ";
    cout<<endl;
}

int main()
{
    //test
    int test[]= {150,71,0,1,99,-31,-1},arr[7];
    int n_test = 7;
    cop(arr,test,n_test);
    bubbleSort(arr,n_test);
    print_array(arr,n_test);

    cop(arr,test,n_test);
    insertionSort(arr,n_test);
    print_array(arr,n_test);

    cop(arr,test,n_test);
    selectionSort(arr,n_test);
    print_array(arr,n_test);
    //end test
    int v[MAX_SIZE],a[MAX_SIZE],n;
    for(int i=0; i<m; i++)
    {
        FillRandomArray(v, MAX_SIZE);
        for(n=100; n<MAX_SIZE; n += 500)
        {
            cop(a,v,MAX_SIZE);
            bubbleSort(a, n);

            cop(a,v,MAX_SIZE);
            insertionSort(a,n);

            cop(a,v,MAX_SIZE);
            selectionSort(a,n);
        }
    }
    profiler.divideValues("bubbleSortAss",5);
    profiler.divideValues("insertionSortAss",5);
    profiler.divideValues("selectionSortAss",5);
    profiler.divideValues("bubbleSortCom",5);
    profiler.divideValues("insertionSortCom",5);
    profiler.divideValues("selectionSortCom",5);
    profiler.addSeries("bubbleSort", "bubbleSortAss", "bubbleSortCom");
    profiler.addSeries("insertionSort", "insertionSortAss", "insertionSortCom");
    profiler.addSeries("selectionSort", "selectionSortAss", "selectionSortCom");
    profiler.createGroup("avg", "bubbleSort", "insertionSort", "selectionSort");
    profiler.createGroup("avgAss", "bubbleSortAss", "insertionSortAss", "selectionSortAss");
    profiler.createGroup("avgCom", "bubbleSortCom", "insertionSortCom", "selectionSortCom");
    profiler.createGroup("avgAssSelection", "selectionSortAss");
    profiler.showReport();

    profiler.reset("bestSort");
    FillRandomArray(v, MAX_SIZE,10,50000,false,1);
    for(n=100; n<MAX_SIZE; n += 500)
    {
        cop(a,v,MAX_SIZE);
        bubbleSort(a, n);

        cop(a,v,MAX_SIZE);
        insertionSort(a,n);

        cop(a,v,MAX_SIZE);
        selectionSort(a,n);
    }
    profiler.addSeries("bubbleSort", "bubbleSortAss", "bubbleSortCom");
    profiler.addSeries("insertionSort", "insertionSortAss", "insertionSortCom");
    profiler.addSeries("selectionSort", "selectionSortAss", "selectionSortCom");
    profiler.createGroup("best", "bubbleSort", "insertionSort", "selectionSort");
    profiler.createGroup("bestAss", "bubbleSortAss", "insertionSortAss", "selectionSortAss");
    profiler.createGroup("bestCom", "bubbleSortCom", "insertionSortCom", "selectionSortCom");
    profiler.createGroup("bestBubble&Insertion", "bubbleSort", "insertionSort");
    profiler.createGroup("bestBubble&InsertionCom", "bubbleSortCom", "insertionSortCom");
    profiler.showReport();

    profiler.reset("worstSort");

    FillRandomArray(v, MAX_SIZE,10,50000,false,2);
    for(n=100; n<MAX_SIZE; n += 500)
    {
        cop(a,v,MAX_SIZE);
        bubbleSort(a, n);

        cop(a,v,MAX_SIZE);
        insertionSort(a,n);
    }
    FillRandomArray(v, MAX_SIZE,10,50000,false,2);
    for(n=100; n<MAX_SIZE; n += 500)
    {
        a[0]=9;
        cop(a,v,MAX_SIZE);
        selectionSort(a,n);
    }
    profiler.addSeries("bubbleSort", "bubbleSortAss", "bubbleSortCom");
    profiler.addSeries("insertionSort", "insertionSortAss", "insertionSortCom");
    profiler.addSeries("selectionSort", "selectionSortAss", "selectionSortCom");
    profiler.createGroup("worst", "bubbleSort", "insertionSort", "selectionSort");
    profiler.createGroup("worstAss", "bubbleSortAss", "insertionSortAss", "selectionSortAss");
    profiler.createGroup("worstCom", "bubbleSortCom", "insertionSortCom", "selectionSortCom");
    profiler.createGroup("worstAssSelection", "selectionSortAss");
    profiler.showReport();
    return 0;
}
