#include <iostream>
#include <stack>
#include <stdlib.h>
#include <string.h>
#include "Profiler.h"
using namespace std;

int odfs;
struct Set
{
    int key;
    struct Set* p;
    int rk;
};

enum
{
    LABEL_TREE = 0,
    LABEL_BWD,
    LABEL_FWD,
    LABEL_CROSS
};
enum
{
    COLOR_WHITE = 0,
    COLOR_GRAY,
    COLOR_BLACK
};

typedef struct _Node
{
    int adjSize;
    struct _Node **adj;
    int info;
    int color;
    int discoverTime;
    int finalTime;
    struct _Node *parent;

    int index;
    int lowlink;
    bool onStack;
} Node;

typedef struct
{
    int time;
    int nrNodes;
    Node **v;
} Graph;

struct Edge
{
    Node* u;
    Node* v;
    int label;
};
typedef struct
{
    int nrEdges;
    Edge **v;
} EdgeArr;

stack<Node*> mystack;


/// Sets ////////////////////////////////////
Set* makeSet(int item)
{
    Set* x = (Set*) malloc(sizeof(Set));
    x->key = item;
    x->p = x;
    x->rk = 0;
    return x;
}

Set* findSet(Set* x)
{
    if(x!=x->p)
    {
        x->p = findSet(x->p);
    }
    return x->p;
}

void link(Set* x, Set* y)
{
    if(x->rk>y->rk)
    {
        y->p = x;
    }

    else
    {
        x->p = y;
    }
    if(x->rk == y->rk)
    {
        y->rk = y->rk + 1;
    }
}

void unionSet(Set* x, Set* y)
{
    link(findSet(x),findSet(y));
}

/// End Sets ////////////////////////////////////

Edge* newEdge(Node* u, Node* v,int label)
{
    Edge* x =(Edge*) malloc(sizeof(Edge));
    x->u = u;
    x->v = v;
    x->label = label;
    return x;
}

void DFS_visit(Graph* graph, Node* u, EdgeArr* edgearr)
{
    odfs+=8;
    graph->time++;
    u->discoverTime = graph->time;
    u->color = COLOR_GRAY;
    for(int i = 0; i<u->adjSize; i++)
    {
        odfs+=2;
        if(u->adj[i]->color==COLOR_BLACK)
        {
            if(u->discoverTime<u->adj[i]->discoverTime)
            {
                edgearr->v[edgearr->nrEdges]= newEdge(u,u->adj[i],LABEL_FWD);
                edgearr->nrEdges++;
            }
            else
            {
                edgearr->v[edgearr->nrEdges]= newEdge(u,u->adj[i],LABEL_CROSS);
                edgearr->nrEdges++;
            }
        }
        else if(u->adj[i]->color==COLOR_GRAY)
        {
            edgearr->v[edgearr->nrEdges]= newEdge(u,u->adj[i],LABEL_BWD);
            edgearr->nrEdges++;
        }
        else if(u->adj[i]->color==COLOR_WHITE)
        {
            odfs+=3;
            u->adj[i]->parent = u;
            edgearr->v[edgearr->nrEdges]= newEdge(u,u->adj[i],LABEL_TREE);
            edgearr->nrEdges++;
            DFS_visit(graph,u->adj[i], edgearr);

        }
    }
    u->color = COLOR_BLACK;
    graph->time++;
    u->finalTime = graph->time;
    mystack.push(u);

}

int Exist(Graph* graph,int x, int a, int n )
{
    for(int j=0; j<graph->v[x]->adjSize; j++)
        if(graph->v[x]->adj[j] == graph->v[a])
            return 1;
    return 0;
}

void generateRandomGraph(Graph* graph,int e, int n)
{
    Set* V[n*n];
    int i,x,a;
    for(i=0; i<n; i++)
    {
        V[i] = makeSet(i);
        graph->v[i]->adjSize = 0;
        graph->v[i]->adj=(Node**)malloc(graph->nrNodes * sizeof(Node*));
    }
    i=0;
    while(i<n-1)
    {
        x = rand()%n;
        graph->v[i]->adj[graph->v[i]->adjSize]=graph->v[x];
        if(findSet(V[i]) != findSet(V[x]))
        {
            unionSet(V[i],V[x]);
            graph->v[i]->adjSize++;
            graph->v[x]->adj[graph->v[x]->adjSize]=graph->v[i];
            graph->v[x]->adjSize++;
            i++;
        }
    }
    i = n-1;
    int ct=0;
    while(i<e)
    {
        x = rand()%n, a=rand()%n;
        if(!Exist(graph,x,a,n))
        {
            graph->v[a]->adj[graph->v[a]->adjSize]=graph->v[x];
            graph->v[a]->adjSize++;
            graph->v[x]->adj[graph->v[x]->adjSize]=graph->v[a];
            graph->v[x]->adjSize++;
            i++;
        }

    }

}

void free_graph(Graph *graph)
{
    if(graph->v != NULL)
    {
        for(int i=0; i<graph->nrNodes; ++i)
        {
            if(graph->v[i] != NULL)
            {
                if(graph->v[i]->adj != NULL)
                {
                    free(graph->v[i]->adj);
                    graph->v[i]->adj = NULL;
                }
                graph->v[i]->adjSize = 0;
                free(graph->v[i]);
                graph->v[i] = NULL;
            }
        }
        free(graph->v);
        graph->v = NULL;
    }
    graph->nrNodes = 0;
}

void free_edgearr(EdgeArr *edgearr)
{
    if(edgearr->v != NULL)
    {
        for(int i=0; i<edgearr->nrEdges; ++i)
        {
            if(edgearr->v[i] != NULL)
            {
                if(edgearr->v[i]->u != NULL)
                {
                    free(edgearr->v[i]->u);
                    edgearr->v[i]->u = NULL;
                }
                if(edgearr->v[i]->v != NULL)
                {
                    free(edgearr->v[i]->v);
                    edgearr->v[i]->v = NULL;
                }
                edgearr->v[i]->label = 0;
                free(edgearr->v[i]);
                edgearr->v[i] = NULL;
            }
        }
        free(edgearr->v);
        edgearr->v = NULL;
    }
    edgearr->nrEdges = 0;
}

EdgeArr allocEdgeArr()
{
    EdgeArr edgearr;
    edgearr.nrEdges = 0;
    edgearr.v = (Edge**)malloc(9000 * sizeof(Edge*));
    for(int i=0; i<9000; ++i)
    {
        edgearr.v[i] = (Edge*)malloc(sizeof(Edge));
        memset(edgearr.v[i], 0, sizeof(Edge));
    }
    return edgearr;
}

Graph allocGraph(int no)
{
    Graph graph;
    graph.nrNodes = no;
    graph.v = (Node**)malloc(graph.nrNodes * sizeof(Node*));
    for(int i=0; i<graph.nrNodes; ++i)
    {
        graph.v[i] = (Node*)malloc(sizeof(Node));
        memset(graph.v[i], 0, sizeof(Node));
        graph.v[i]->info = i;
    }
    return graph;
}

void performance()
{
    int n, i;
    Profiler p("dfs");

    for(n=1000; n<=5000; n+=100)
    {
        Operation op = p.createOperation("dfs-edges", n);
        EdgeArr edgearr = allocEdgeArr();
        Graph graph= allocGraph(100);
        generateRandomGraph(&graph,n,graph.nrNodes);
        odfs=0;
        DFS_visit(&graph, graph.v[0], &edgearr);
        op.count(odfs);
        free_graph(&graph);
        //free_edgearr(&edgearr);
        //cout<<n<<endl;
    }
    for(n=100; n<=200; n+=10)
    {
        Operation op = p.createOperation("dfs-nrNodes", n);
        EdgeArr edgearr = allocEdgeArr();
        Graph graph= allocGraph(n);
        generateRandomGraph(&graph,4500,graph.nrNodes);
        odfs = 0;
        DFS_visit(&graph, graph.v[0], &edgearr);
        op.count(odfs);
        free_graph(&graph);
        //free_edgearr(&edgearr);
    }

    p.showReport();
}

void addEdge(Graph graph, int head, int tail)
{
    if (graph.v[head]->adjSize == 0)
    {
        graph.v[head]->adj = (Node **) malloc(sizeof(Node *));
    }
    else
    {
        graph.v[head]->adj = (Node **) realloc(graph.v[head]->adj,
                                               (graph.v[head]->adjSize + 1) * sizeof(Node *));
    }

    if (graph.v[tail]->adjSize == 0)
    {
        graph.v[tail]->adj = (Node **) malloc(sizeof(Node *));
    }
    else
    {
        graph.v[tail]->adj = (Node **) realloc(graph.v[tail]->adj,
                                               (graph.v[tail]->adjSize + 1) * sizeof(Node *));
    }

    graph.v[head]->adj[graph.v[head]->adjSize++] = graph.v[tail];
}

void dfs(Graph graph)
{
    EdgeArr edgearr = allocEdgeArr();
    for (int i = 0; i < graph.nrNodes; i++)
    {
        graph.v[i]->color = COLOR_WHITE;
        graph.v[i]->parent = nullptr;
    }
    graph.time = 0;
    for (int j = 0; j < graph.nrNodes; j++)
    {
        if (graph.v[j]->color == COLOR_WHITE)
        {
            DFS_visit(&graph, graph.v[j],&edgearr);
        }
    }

}
stack<Node *> stackTar;
void strongConnect(Graph graph, Node *node, int& index) {
    node->index = index;
    node->lowlink = index;
    index++;
    stackTar.push(node);
    node->onStack = true;

    for (int i = 0; i < node->adjSize; i++) {
        if (node->adj[i]->index == -1) {
            strongConnect(graph, node->adj[i], index);
            node->lowlink = min(node->lowlink, node->adj[i]->lowlink);
        } else if (node->adj[i]->onStack) {
            node->lowlink = min(node->lowlink, node->adj[i]->lowlink);
        }
    }

    if (node->lowlink == node->index) {
        Node *w = new Node;
        do {
            w = stackTar.top();
            stackTar.pop();
            w->onStack = false;

            cout << w->info << ' ';
        } while (node->info != w->info);
        cout << endl;
    }
}

void tarjan(Graph graph) {
    int index = 0;
    for (int i = 0; i < graph.nrNodes; i++) {
        graph.v[i]->index = -1;
        graph.v[i]->lowlink = -1;
    }

    for (int i = 0; i < graph.nrNodes; i++) {
        if (graph.v[i]->index == -1) {
            strongConnect(graph, graph.v[i], index);
        }
    }
}

void trj()
{
    Graph graph = allocGraph(11);
    addEdge(graph, 0,1);
    addEdge(graph, 0,3);
    addEdge(graph, 1,2);
    addEdge(graph, 1,4);
    addEdge(graph, 2,0);
    addEdge(graph, 2,6);
    addEdge(graph, 3,2);
    addEdge(graph, 4,5);
    addEdge(graph, 4,6);
    addEdge(graph, 5,6);
    addEdge(graph, 5,7);
    addEdge(graph, 5,8);
    addEdge(graph, 5,9);
    addEdge(graph, 6,4);
    addEdge(graph, 7,9);
    addEdge(graph, 8,9);
    addEdge(graph, 9,8);

    cout << "Graph: " << endl;
    for (int i = 0; i < graph.nrNodes; i++)
    {
        cout << graph.v[i]->info << ": ";
        for (int j = 0; j < graph.v[i]->adjSize; j++)
        {
            cout << graph.v[i]->adj[j]->info << ' ';
        }
        cout << endl;
    }

    cout << "Strongly connected components : " << endl;
    tarjan(graph);
}

bool isCyclicUtil(int v ,Graph* graph,bool *visited, bool *recStack)
{
    if(visited[v] == false)
    {
        visited[v] = true;
        recStack[v] = true;

        for(int i = 0; i != graph->v[v]->adjSize; ++i)
        {
            if ( !visited[i] && isCyclicUtil(i,graph, visited, recStack) )
                return true;
            else if (recStack[i])
                return true;
        }

    }
    recStack[v] = false;
    return false;
}

bool isCyclic(Graph* graph)
{
    bool *visited = new bool[graph->nrNodes];
    bool *recStack = new bool[graph->nrNodes];
    for(int i = 0; i < graph->nrNodes; i++)
    {
        visited[i] = false;
        recStack[i] = false;
    }
    for(int i = 0; i < graph->nrNodes; i++)
        if (isCyclicUtil(i,graph, visited, recStack))
            return true;

    return false;
}

int main()
{
    Graph graph= allocGraph(6);
    addEdge(graph, 5, 2);
    addEdge(graph, 5, 0);
    addEdge(graph, 4, 0);
    addEdge(graph, 4, 1);
    addEdge(graph, 2, 3);
    addEdge(graph, 3, 1);
    if(!isCyclic(&graph))
    {
    dfs(graph);

    cout << "Topological Sort: ";
    for (int j = 0; j < graph.nrNodes; j++)
    {
        cout << mystack.top()->info << ' ';
        mystack.pop();
    }
    cout<<endl;
    }
    else
    {
        cout<<"Has cycle(s)"<<endl;
    }
    trj();
    performance();
    return 0;
}
