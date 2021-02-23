#include <iostream>
#include <cstdlib>
#include <time.h>
#define MAX_SIZE 10000
#include "Profiler.h"

Profiler profiler("disjoint_sets_krusk");
using namespace std;
int NOms,NOfs,NOl;
struct Set
{
    int key;
    struct Set* p;
    int rk;
};

struct Edgein
{
    int w;
    int u;
    int v;
};

struct Edge
{
    int w;
    Set* u;
    Set* v;
};

struct List
{
    struct List* next;
    int key;
};

void heapify_bu(Edge* a[4*MAX_SIZE], int i, int n)
{
    int largest = i;
    if(2*i+1<n && a[largest]->w<a[2*i+1]->w)
        largest = 2*i+1;
    if(2*i+2<n && a[largest]->w<a[2*i+2]->w)
        largest = 2*i+2;

    if(largest != i)
    {
        swap(a[i],a[largest]);
        heapify_bu(a,largest,n);
    }

}

void build_heap_bu(Edge* a[4*MAX_SIZE], int n)
{
    for(int i = (n/2)-1; i>=0; i--)
        heapify_bu(a,i,n);
}

void heapsort_bottomup(Edge* a[4*MAX_SIZE], int n)
{

    build_heap_bu(a,n);
    int heap_size = n;
    for(int i = n-1; i>0; i--)
    {
        swap(a[0],a[i]);
        heap_size--;
        heapify_bu(a,0,heap_size);
    }

}

Set* makeSet(int item)
{
    NOms+=3;
    Set* x = (Set*) malloc(sizeof(Set));
    x->key = item;
    x->p = x;
    x->rk = 0;
    return x;
}

Set* findSet(Set* x)
{
    NOfs++;
    if(x!=x->p)
    {
        NOfs++;
        x->p = findSet(x->p);
    }
    return x->p;
}

void link(Set* x, Set* y)
{
    NOl+=2;
    if(x->rk>y->rk)
    {
        NOl++;
        y->p = x;
    }

    else
    {
        NOl++;
        x->p = y;
    }
        if(x->rk == y->rk)
        {
            NOl++;
            y->rk = y->rk + 1;
        }
}

void unionSet(Set* x, Set* y)
{
    link(findSet(x),findSet(y));
}

Edge* createEdge(int weight)
{
    Edge* x = (Edge*) malloc(sizeof(Edge));
    x->w = weight;
    x->u = NULL;
    x->v = NULL;
    return x;
}

int createSortEdges(Edgein* ein[4*MAX_SIZE],Edge* E[4*MAX_SIZE],Set* V[MAX_SIZE], int n)
{

    int i,heapsize = 0;
    for(i=0; i<n; i++)
    {
        Edge* x = createEdge(ein[i]->w);
        x->u =V[ein[i]->u];
        x->v=V[ein[i]->v];
        E[heapsize]=x;
        heapsize++;
    }
    cout<<" "<<n;
    heapsort_bottomup(E,heapsize);
    return heapsize;
}

void aloc1(Edge* e[4*MAX_SIZE], int n)
{
    for(int i = 0; i<n; i++)
    {
        Edge* x = (Edge*)malloc(sizeof(Edge));
        e[i] = x;
    }
}
int MSTkruskal(Edgein* ein[4*MAX_SIZE],int n, Edge* a[MAX_SIZE])
{
    int sz = 0;
    Set* V[MAX_SIZE];
    Edge* E[4*MAX_SIZE];
    aloc1(E,4*n);
    for(int i=0; i<n; i++)
    {
        V[i] = makeSet(i);
    }

    int heapsize = createSortEdges(ein,E,V,n);

    for(int i=0; i<heapsize; i++)
        if(findSet(E[i]->u) != findSet(E[i]->v))
        {
            a[sz] = E[i];
            sz++;
            unionSet(E[i]->u,E[i]->v);
        }
    //free(E);
    //free(V);
    return sz;
}

void aloc(Edgein* e[4*MAX_SIZE], int n)
{
    for(int i = 0; i<n; i++)
    {
        Edgein* x = (Edgein*)malloc(sizeof(Edgein));
        e[i] = x;
    }
}
void init(List* v[], int n)
{
    for(int i = 0; i < n; i++)
    {
        List* temp =(List*)malloc(sizeof(List));
        temp->key = -1;
        temp->next = NULL;
        v[i]=temp;
    }
}

List* findList(List* node, int item)
{
    while(node->next != NULL)
    {
        if(node->key == item)
            return NULL;
        node = node->next;
    }
    if(node->key == item)
        return NULL;
    return node;
}

int Exist(Edgein * e[], int i )
{
    for(int j=0; j<i; j++)
        if((e[i]->v == e[j]->v && e[i]->u == e[j]->u) ||
                (e[i]->u == e[j]->v && e[i]->v == e[j]->u))
            return 1;
    return 0;
}

void generateRandomGraph(Edgein* e[4*MAX_SIZE], int n)
{
    Set* V[n];
    aloc(e,4*n);
    int i;
    for(i=0; i<n; i++)
    {
        V[i] = makeSet(i);
    }
    i=0;
    while(i<n-1)
    {
        //cout<<i<<endl;
            e[i]->w = rand();
            e[i]->v = i;
            e[i]->u = rand()%n;
            if(findSet(V[e[i]->u]) != findSet(V[i]))
            {
                unionSet(V[i],V[e[i]->u]);
                i++;
            }
    }
    i = n-1;
    int ct=0;
    while(i<4*n)
    {
        e[i]->w = rand();
        e[i]->v = rand()%n;
        e[i]->u = rand()%n;
        if(!Exist(e,i))
            i++;
    }

}

int main()
{
    Edge* a[MAX_SIZE];
    Edgein* e[4*MAX_SIZE];
    cout<<"ceau";

    int sz,n=10,i,j;
    aloc1(a,MAX_SIZE);
    aloc(e,4*MAX_SIZE);
    /*e[0]->w=3;
    e[0]->u=0;
    e[0]->v=1;

    e[1]->w=1;
    e[1]->u=0;
    e[1]->v=2;

    e[2]->w=5;
    e[2]->u=1;
    e[2]->v=2;

    e[3]->w=6;
    e[3]->u=0;
    e[3]->v=3;

    e[4]->w=5;
    e[4]->u=2;
    e[4]->v=3;

    e[5]->w=3;
    e[5]->u=1;
    e[5]->v=4;

    e[6]->w=6;
    e[6]->u=2;
    e[6]->v=4;

    e[7]->w=4;
    e[7]->u=2;
    e[7]->v=5;

    e[8]->w=2;
    e[8]->u=3;
    e[8]->v=5;

    e[9]->w=6;
    e[9]->u=4;
    e[9]->v=5;

    cout<<"\nEnter number of vertices:";

    //cin>>n;

    cout<<"\nEnter the adjacency matrix:\n";

    //for(i=0;i<n;i++)
    	//for(j=0;j<n;j++)
    		//cin>>G[i][j];
    sz = MSTkruskal(e,n,a);
    int cost=0;

    for(i=0;i<sz;i++)
    {
    	cout<<a[i]->u->key<<"     "<<a[i]->v->key;
    	cout<<"     "<<a[i]->w<<endl;
    	cost=cost+a[i]->w;
    }

    cout<<endl<<"Cost of the spanning tree="<<cost;*/
    for (n = 100; n <= 10000; n += 100)
    {
        generateRandomGraph(e,n);
        cout<<n;
        NOms = 0;
        NOfs = 0;
        NOl = 0;
        sz = MSTkruskal(e,n,a);
        cout<<" "<<n<<endl;
        profiler.countOperation("NOms", n, NOms);
        profiler.countOperation("NOfs",n, NOfs);
        profiler.countOperation("NOl",n, NOl);
        }
        profiler.createGroup("All","NOms","NOfs","NOl");
        profiler.showReport();
        return 0;
}
