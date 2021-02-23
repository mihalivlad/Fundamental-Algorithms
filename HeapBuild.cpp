#include <iostream>
#include <climits>
#include "Profiler.h"

#define MAX_SIZE 100000
#define m 5
using namespace std;
int sz,oTD,oBU;
Profiler profiler("avgCase");
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

void heapify_td(int a[], int i, int n)
{
    oTD++;
    int largest = i;
    if(2*i+1<n && a[largest]<a[2*i+1])
        largest = 2*i+1;
    oTD++;
    if(2*i+2<n && a[largest]<a[2*i+2])
        largest = 2*i+2;

    if(largest != i)
    {
        oTD+=3;
        swap(a[i],a[largest]);
        heapify_td(a,largest,n);
    }

}


void heap_increase_key(int a[], int i, int key)
{
    oTD++;
    if (key<a[i])
        return;
    a[i] = key;
    while(i>0 && a[(i-1)/2]<a[i])
    {
        oTD+=4;
        swap(a[i],a[(i-1)/2]);
        i = (i-1)/2;
    }
    if(i!=0)
        oTD++;
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


void pushHeap(int key,int a[],int &n)
{
    n++;
    a[n-1]=INT_MIN;
    heap_increase_key(a,n-1, key);
}

int popHeap(int a[],int &n)
{

    oTD+=3;
    n--;
    swap(a[0],a[n]);
    heapify_td(a,0,n);
    return a[n];
}

void build_heap_topdown(int a[], int n)
{
    int heap_size = 0;
    for(int i = 0; i<n; i++)
        pushHeap(a[i],a,heap_size);
}

void heapsort_topdown(int a[], int n)
{
    build_heap_topdown(a,n);
    int heap_size = n;
    for(int i = n-1; i>0; i--)
        popHeap(a,heap_size);

}
void print_array(int a[],int n)
{
    for(int i=0; i<n; i++)
        cout<<a[i]<<" ";
}

int main()
{
    int test[]= {150,71,0,1,99,-31,-1},arr[7];
    int n_test = 7;
    cop(arr,test,n_test);
    heapsort_topdown(test,n_test);
    print_array(test,n_test);
    cout<<endl;
    heapsort_bottomup(arr,n_test);
    print_array(arr,n_test);

    int v[MAX_SIZE],a[MAX_SIZE];
     for(int i=0; i<m; i++)
     {
         FillRandomArray(v, MAX_SIZE);
         for(sz=100; sz<MAX_SIZE; sz += 100)
         {
             oTD=0;
             oBU=0;
             cop(a,v,sz);
             //heapsort_bottomup(a,sz);
             build_heap_bu(a,sz);

             cop(a,v,sz);
             //heapsort_topdown(a,sz);
             build_heap_topdown(a,sz);
             profiler.countOperation("heapbuild_bottomup",sz,oBU);
             profiler.countOperation("heapbuild_topdown",sz,oTD);
         }
     }
     profiler.divideValues("heapbuild_bottomup",5);
     profiler.divideValues("heapbuild_topdown",5);
     profiler.createGroup("heapbuild","heapbuild_bottomup","heapbuild_topdown");
     profiler.reset("worstCase");

         FillRandomArray(v, MAX_SIZE,10,50000,false,1);
         for(sz=100; sz<MAX_SIZE; sz += 100)
         {
             cout<<sz<<endl;
             oTD=0;
             oBU=0;
             cop(a,v,sz);
             //heapsort_bottomup(a,sz);
             build_heap_bu(a,sz);

             cop(a,v,sz);
             //heapsort_topdown(a,sz);
             build_heap_topdown(a,sz);
             profiler.countOperation("heapbuild_bottomup",sz,oBU);
             profiler.countOperation("heapbuild_topdown",sz,oTD);
         }
     profiler.createGroup("heapbuild","heapbuild_bottomup","heapbuild_topdown");
     profiler.showReport();

    return 0;
}
