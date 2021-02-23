/*
Given k sorted arrays of size unknown size each, merge them and print the sorted output.
All k arrays combine have size n.

Following is detailed algorithm.
1. Create an output array of size n.

2. Create a min heap of size k and insert 1st element in all the arrays into the heap

3. Repeat following steps n*k times.
     a) Get minimum element from heap (minimum is always at root) and store it in output array.
     b) Replace heap root with next element from the array from which the element is extracted.
      If the array does not have any more elements, then replace root with infinite.
       After replacing the root, heapify the tree.

Time complexity is O(n log k).

We can see from the charts that values for k = 100 are double with k = 10 because is logarithmic.
Because k is constant the chart is linear.

The next chart is logarithmic because n is constant.
We can observe bumps on the graph.

*/


#include <iostream>
#include <climits>
#include "Profiler.h"

#define MAX_SIZE 10000
#define m 5

using namespace std;
int oBU;
Profiler profiler("mergeKStable");

struct HeapS
{
    int key;
    int index;
};

struct Node
{
    int data;
    struct Node *next;
};

void cop(int arr1[], int arr2[], int n)
{
    for(int i = 0; i < n; i++)
        arr1[i]=arr2[i];
}

void heapify_bu(HeapS a[], int i, int n)
{
    oBU++;
    int largest = i;
    if(2*i+1<n && a[largest].key>a[2*i+1].key)
        largest = 2*i+1;
    oBU++;
    if(2*i+2<n && a[largest].key>a[2*i+2].key)
        largest = 2*i+2;

    if(largest != i)
    {
        oBU+=3;
        swap(a[i],a[largest]);
        heapify_bu(a,largest,n);
    }

}

void build_heap_bu(HeapS a[], int n)
{
    for(int i = (n/2)-1; i>=0; i--)
        heapify_bu(a,i,n);
}


Node* newNode(int data)
{
    Node* new_node = new Node();
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}


void push(Node** root, int item)
{
    Node* temp = newNode(item);
    Node* ptr;
    temp->next = NULL;

    if (*root == NULL)
        *root = temp;
    else
    {
        ptr = *root;
        while (ptr->next != NULL)
            ptr = ptr->next;
        ptr->next = temp;
    }
}


void printList(Node *head)
{
    Node *temp = head;
    while (temp != NULL)
    {
        cout<<temp->data<<" ";
        temp = temp->next;
    }
    cout<<endl;
}

void insertLast(Node** last, int key)
{

    oBU++;
    Node* myNode=newNode(key);
    if(*last == NULL)
    {
        *last = myNode;
        oBU++;
        return;
    }
    oBU+=2;
    (*last)->next = myNode;
    (*last) = (*last)->next;
}

Node* mergeK(Node* arr[], int k)
{
    Node *head = NULL;
    Node *tail = newNode(0);
    int first = 0;
    HeapS heap[k];
    int heap_size = k;
    for(int i=0; i<heap_size; i++)
    {
        oBU++;
        heap[i].key = arr[i]->data;
        heap[i].index = i;
    }
    build_heap_bu(heap, heap_size);
    while(heap_size>1)
    {

        oBU+=3;
        first++;
        if(first == 1)
        {
            push(&head,heap[0].key);
            tail->next = head->next;
            tail->data = head->data;


        }
        else
            insertLast(&tail,heap[0].key);

        if(first == 2)
            head->next=tail;

        if(arr[heap[0].index]->next == NULL)
        {
            oBU++;
            heap[0]=heap[heap_size-1];
            heap_size--;
            heapify_bu(heap,0,heap_size);
        }
        else
        {
            oBU+=2;
            arr[heap[0].index] = arr[heap[0].index]->next;
            heap[0].key = arr[heap[0].index]->data;
            heapify_bu(heap,0,heap_size);
        }
    }

    while(arr[heap[0].index]!=NULL)
    {
        oBU+=4;
        push(&head,arr[heap[0].index]->data);
        arr[heap[0].index] = arr[heap[0].index]->next;
    }

    free(heap);
    free(arr);
    return head;
}

Node *arrayToList(int arr[], int n)
{
    Node *root = NULL;
    for (int i = 0; i < n; i++)
        push(&root, arr[i]);
    return root;
}
int main()
{
    int l = 3;
    int n = 4;
    Node* arr1[l];


    arr1[0] = newNode(0);
    arr1[0]->next = newNode(1);
    arr1[0]->next->next = newNode(2);
    arr1[0]->next->next->next = newNode(9);

    arr1[1] = newNode(3);
    arr1[1]->next = newNode(4);
    arr1[1]->next->next = newNode(5);
    arr1[1]->next->next->next = newNode(10);

    arr1[2] = newNode(6);
    arr1[2]->next = newNode(7);
    arr1[2]->next->next = newNode(8);
    arr1[2]->next->next->next = newNode(11);


    Node* head = mergeK(arr1, l);

    printList(head);
    free(arr1);
    free(head);
    int v[MAX_SIZE],a[MAX_SIZE],sz,k[]= {5,10,100};
    Node * arr[5000];
    for(sz=100; sz<MAX_SIZE; sz += 100)
    {

        for(int index = 0; index < 3; index++)
        {
            oBU=0;
            for(int i=0; i<k[index]; i++)
            {
                FillRandomArray(v, sz/k[index],10,50000,false,1);
                arr[i] = arrayToList(v,sz/k[index]);
            }
            Node* head = mergeK(arr, k[index]);
            free(head);
            if(k[index] == 5)
                profiler.countOperation("mergeK5",sz,oBU);
            else if(k[index] == 10)
                profiler.countOperation("mergeK10",sz,oBU);
            else if(k[index] == 100)
                profiler.countOperation("mergeK100",sz,oBU);
        }
    }
    profiler.createGroup("mergeK","mergeK5","mergeK10","mergeK100");
    free(arr);
    profiler.reset("mergeKRandom");
    sz=MAX_SIZE;
    for(int ka=10; ka<=500; ka += 10)
    {
        oBU=0;
        for(int i=0; i<ka-1; i++)
        {
            FillRandomArray(v, sz/ka,10,50000,false,1);
            arr[i] = arrayToList(v,sz/ka);
        }
        FillRandomArray(v, sz/ka+sz%ka,10,50000,false,1);
        arr[ka-1] = arrayToList(v,sz/ka+sz%ka);
        Node* head = mergeK(arr, ka);
        free(head);
        profiler.countOperation("mergeK",ka,oBU);
    }
    profiler.showReport();
    free(arr);

    return 0;
}
