#include <iostream>
#include <climits>
#include "Profiler.h"

#define MAX_SIZE 100000
#define m 5
using namespace std;
int sz,oQS,oBU;
Profiler profiler("HeapSortVsQuickSort");
void cop(int arr1[], int arr2[], int n)
{
    for(int i = 0; i < n; i++)
        arr1[i]=arr2[i];
}

/*int maximum(int a, int b, int c, int index)
{
    if(a>b)
        return ((a>c)?index:(2*index+2));
   return ((b>c)?(2*index+1):(2*index+2));
}*/

void heapify_bu(int a[], int i, int n)
{
    oBU++;
    int largest = i;
    if(2*i+1<n && a[largest]<a[2*i+1])
        largest = 2*i+1;
    oBU++;
    if(2*i+2<n && a[largest]<a[2*i+2])
        largest = 2*i+2;

    if(largest != i)
    {
        oBU+=3;
        swap(a[i],a[largest]);
        heapify_bu(a,largest,n);
    }

}

void build_heap_bu(int a[], int n)
{
    for(int i = (n/2)-1; i>=0; i--)
        heapify_bu(a,i,n);
}



void heapsort_bottomup(int a[], int n)
{

    build_heap_bu(a,n);
    int heap_size = n;
    for(int i = n-1; i>0; i--)
    {
        oBU+=3;
        swap(a[0],a[i]);
        heap_size--;
        heapify_bu(a,0,heap_size);
    }

}


/// quick sort

int part(int a[], int p, int r)
{
    oQS+=4;
    int x = a[r];
    int i = p-1;
    for(int j = p; j<=r-1; j++)
    {
        oQS++;
        if(a[j]<= x)
        {
            oQS+=3;
            i++;
            swap(a[i],a[j]);
        }
    }
    swap(a[i+1],a[r]);
    return (i+1);
}

int rand_part(int a[], int p, int r)
{
    int x = p+rand()%(r-p+1);
    swap(a[r],a[x]);
    return part(a,p,r);
}

void quickSort(int a[], int p, int r)
{
    if(p<r)
    {
        int q = rand_part(a,p,r);
        quickSort(a,p,q-1);
        quickSort(a,q+1,r);
    }
}

/// QuickSelect

int QuickSelect(int a[], int p, int r, int i)
{
    if(p==r)
        return a[p];
    int q = rand_part(a,p,r);
    int k = q-p+1;
    if(i+1==k)
        return a[q];
    else if(i+1<k)
        return QuickSelect(a,p,q-1,i);
    else
        return QuickSelect(a,q+1,r,i-k);
}

void print_array(int a[],int n)
{
    for(int i=0; i<n; i++)
        cout<<a[i]<<" ";
}

int main()
{
    int test[]= {150,71,0,1,99,-31,-1};
    int n_test = 7;
    cout<<QuickSelect(test,0,n_test-1,6)<<endl;
    quickSort(test,0,n_test-1);
    print_array(test,n_test);

    int v[MAX_SIZE],a[MAX_SIZE];
     for(int i=0; i<m; i++)
     {
         FillRandomArray(v, MAX_SIZE);
         for(sz=100; sz<MAX_SIZE; sz += 100)
         {
             oQS=0;
             oBU=0;
             cop(a,v,sz);
             heapsort_bottomup(a,sz);

             cop(a,v,sz);
             quickSort(a,0,sz-1);
             profiler.countOperation("heapsort_bottomup",sz,oBU);
             profiler.countOperation("quickSort",sz,oQS);
         }
     }
     profiler.divideValues("heapsort_bottomup",5);
     profiler.divideValues("quickSort",5);
     profiler.createGroup("HSvsQS","heapsort_bottomup","quickSort");
     profiler.reset("worstCase");

         FillRandomArray(v, MAX_SIZE,10,50000,false,2);
         for(sz=100; sz<MAX_SIZE; sz += 100)
         {
             //cout<<sz<<endl;
             oQS=0;
             //oBU=0;
             //cop(a,v,sz);
             //heapsort_bottomup(a,sz);

             cop(a,v,sz);
             quickSort(a,0,sz-1);
             //profiler.countOperation("heapsort_bottomup",sz,oBU);
             profiler.countOperation("quickSort",sz,oQS);
         }
     profiler.createGroup("worstQuickSort","quickSort");
     profiler.reset("QuickSelect");

     for(int i=0; i<m; i++)
     {
         FillRandomArray(v, MAX_SIZE);
         for(sz=100; sz<MAX_SIZE; sz += 100)
         {
             oQS=0;
             cop(a,v,sz);
             QuickSelect(a,0,sz-1,(sz-1)/2);
             profiler.countOperation("quickSelect",sz,oQS);
         }
     }
     profiler.divideValues("quickSelect",m);
     profiler.showReport();

    return 0;
}
