#include <stdlib.h>
#include <string.h>
#include <climits>
#include <queue>
#include <iostream>
#include <math.h>
#include "bfs.h"

NodeM* newNode(int i)
{
    NodeM* x = new NodeM();
    x->key = i;
    x->sz = 0;
    return x;
}
NodeBT* createNode(int i)
{
    NodeBT* x = new NodeBT();
    x->key = i;
    x->left = NULL;
    x->right = NULL;
    return x;
}
void createArr(NodeM* arr[],int n)
{
    for(int i = 0 ; i < n; i++ )
    {
        arr[i] = newNode(i);
    }
}
NodeM* transform1(int r[], int n)
{
    NodeM* arr[n];
    createArr(arr,n);
    NodeM* root;
    for(int i = 0 ; i < n; i++)
    {
        if(r[i] == -1)
        {
            root = arr[i];
        }
        else
        {
            arr[r[i]]->children[arr[r[i]]->sz] = arr[i];
            arr[r[i]]->sz++;
        }
    }
    return root;
}

NodeBT* transform2(NodeM* root, NodeM* proot, int pos)
{
    if(root == NULL)
        return NULL;
        NodeBT* broot = createNode(root->key);
    if(root->children[0] !=NULL)
        broot->left = transform2(root->children[0],root,0);
    if(proot!=NULL && proot->children[pos] == root && proot->children[pos+1]!= NULL)
        broot->right = transform2(proot->children[pos+1],proot,pos+1);
    return broot;
}

int get_neighbors(const Grid *grid, Point p, Point neighb[])
{
    // TODO: fill the array neighb with the neighbors of the point p and return the number of neighbors
    // the point p will have at most 4 neighbors (up, down, left, right)
    // avoid the neighbors that are outside the grid limits or fall into a wall
    // note: the size of the array neighb is guaranteed to be at least 4
    int noOfNeigh = 0;
    if(grid->mat[p.row][p.col+1]==0)
    {
        neighb[noOfNeigh].col=p.col+1;
        neighb[noOfNeigh].row=p.row;
        noOfNeigh++;
    }
    if(grid->mat[p.row][p.col-1]==0)
    {
        neighb[noOfNeigh].col=p.col-1;
        neighb[noOfNeigh].row=p.row;
        noOfNeigh++;
    }
    if(grid->mat[p.row+1][p.col]==0)
    {
        neighb[noOfNeigh].col=p.col;
        neighb[noOfNeigh].row=p.row+1;
        noOfNeigh++;
    }
    if(grid->mat[p.row-1][p.col]==0)
    {
        neighb[noOfNeigh].col=p.col;
        neighb[noOfNeigh].row=p.row-1;
        noOfNeigh++;
    }
    return noOfNeigh;
}

void grid_to_graph(const Grid *grid, Graph *graph)
{
    //we need to keep the nodes in a matrix, so we can easily refer to a position in the grid
    Node *nodes[MAX_ROWS][MAX_COLS];
    int i, j, k;
    Point neighb[4];

    //compute how many nodes we have and allocate each node
    graph->nrNodes = 0;
    for(i=0; i<grid->rows; ++i){
        for(j=0; j<grid->cols; ++j){
            if(grid->mat[i][j] == 0){
                nodes[i][j] = (Node*)malloc(sizeof(Node));
                memset(nodes[i][j], 0, sizeof(Node)); //initialize all fields with 0/NULL
                nodes[i][j]->position.row = i;
                nodes[i][j]->position.col = j;
                ++graph->nrNodes;
            }else{
                nodes[i][j] = NULL;
            }
        }
    }
    graph->v = (Node**)malloc(graph->nrNodes * sizeof(Node*));
    k = 0;
    for(i=0; i<grid->rows; ++i){
        for(j=0; j<grid->cols; ++j){
            if(nodes[i][j] != NULL){
                graph->v[k++] = nodes[i][j];
            }
        }
    }

    //compute the adjacency list for each node
    for(i=0; i<graph->nrNodes; ++i){
        graph->v[i]->adjSize = get_neighbors(grid, graph->v[i]->position, neighb);
        if(graph->v[i]->adjSize != 0){
            graph->v[i]->adj = (Node**)malloc(graph->v[i]->adjSize * sizeof(Node*));
            k = 0;
            for(j=0; j<graph->v[i]->adjSize; ++j){
                if( neighb[j].row >= 0 && neighb[j].row < grid->rows &&
                    neighb[j].col >= 0 && neighb[j].col < grid->cols &&
                    grid->mat[neighb[j].row][neighb[j].col] == 0){
                        graph->v[i]->adj[k++] = nodes[neighb[j].row][neighb[j].col];
                }
            }
            if(k < graph->v[i]->adjSize){
                //get_neighbors returned some invalid neighbors
                graph->v[i]->adjSize = k;
                graph->v[i]->adj = (Node**)realloc(graph->v[i]->adj, k * sizeof(Node*));
            }
        }
    }
}

void free_graph(Graph *graph)
{
    if(graph->v != NULL){
        for(int i=0; i<graph->nrNodes; ++i){
            if(graph->v[i] != NULL){
                if(graph->v[i]->adj != NULL){
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

void bfs(Graph *graph, Node *s, Operation *op)
{
    // TOOD: implement the BFS algorithm on the graph, starting from the node s
    // at the end of the algorithm, every node reachable from s should have the color BLACK
    // for all the visited nodes, the minimum distance from s (dist) and the parent in the BFS tree should be set
    // for counting the number of operations, the optional op parameter is received
    // since op can be NULL (when we are calling the bfs for display purposes), you should check it before counting:
    // if(op != NULL) op->count();
    for(int i = 0; i < graph->nrNodes; i++)
    {
        if(op != NULL) op->count(3);
        graph->v[i]->color = COLOR_WHITE;
        graph->v[i]->dist = INT_MAX;
        graph->v[i]->parent = NULL;
    }
    if(op != NULL) op->count(5);
    s->color = COLOR_GRAY;
    s->dist = 0;
    s->parent = NULL;
    std::queue <Node*> myqueue;
    myqueue.push(s);

    while(!myqueue.empty())
    {
        if(op != NULL) op->count(4);
        Node* u = myqueue.front();
        myqueue.pop();
        for(int i = 0; i<u->adjSize; i++)
        {
            if(op != NULL) op->count(2);
            if(u->adj[i]->color == COLOR_WHITE)
        {
            if(op != NULL) op->count(4);
           u->adj[i]->color = COLOR_GRAY;
           u->adj[i]->dist = u->dist + 1;
           u->adj[i]->parent = u;
            myqueue.push(u->adj[i]);
        }
        }
        u->color = COLOR_BLACK;
    }
}

void printBT(NodeBT *root, int space, Point* p)
{
	if (root != NULL)
    {
		for (int i = 0; i < space; i++)
			std::cout << "   ";
		std::cout << p[root->key].row<<"-"<<p[root->key].col<< "\n";
		printBT(root->left, space + 1,p);
		printBT(root->right, space,p);
	}
}

void print_bfs_tree(Graph *graph)
{
    //first, we will represent the BFS tree as a parent array
    int n = 0; //the number of nodes
    int *p = NULL; //the parent array
    Point *repr = NULL; //the representation for each element in p

    //some of the nodes in graph->v may not have been reached by BFS
    //p and repr will contain only the reachable nodes
    int *transf = (int*)malloc(graph->nrNodes * sizeof(int));
    for(int i=0; i<graph->nrNodes; ++i){
        if(graph->v[i]->color == COLOR_BLACK){
            transf[i] = n;
            ++n;
        }else{
            transf[i] = -1;
        }
    }
    if(n == 0){
        //no BFS tree
        free(transf);
        return;
    }

    int err = 0;
    p = (int*)malloc(n * sizeof(int));
    repr = (Point*)malloc(n * sizeof(Node));
    for(int i=0; i<graph->nrNodes && !err; ++i){
        if(graph->v[i]->color == COLOR_BLACK){
            if(transf[i] < 0 || transf[i] >= n){
                err = 1;
            }else{
                repr[transf[i]] = graph->v[i]->position;
                if(graph->v[i]->parent == NULL){
                    p[transf[i]] = -1;
                }else{
                    err = 1;
                    for(int j=0; j<graph->nrNodes; ++j){
                        if(graph->v[i]->parent == graph->v[j]){
                            if(transf[j] >= 0 && transf[j] < n){
                                p[transf[i]] = transf[j];
                                err = 0;
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
    free(transf);
    transf = NULL;

    if(!err){
        // TODO: pretty print the BFS tree
        // the parrent array is p (p[k] is the parent for node k or -1 if k is the root)
        // when printing the node k, print repr[k] (it contains the row and column for that point)
        // you can adapt the code for transforming and printing multi-way trees from the previous labs
        printBT(transform2(transform1(p,n),NULL,0), 0,repr);

    }

    if(p != NULL){
        free(p);
        p = NULL;
    }
    if(repr != NULL){
        free(repr);
        repr = NULL;
    }
}

int shortest_path(Graph *graph, Node *start, Node *end, Node *path[])
{
    // TODO: compute the shortest path between the nodes start and end in the given graph
    // the nodes from the path, should be filled, in order, in the array path
    // the number of nodes filled in the path array should be returned
    // if end is not reachable from start, return -1
    // note: the size of the array path is guaranteed to be at least 1000
    int sp;
    bfs(graph,start);
    if(end->dist == INT_MAX)
        return -1;

    sp = end->dist;
    for(int i = sp; i>=0; i--)
    {
        path[i]= end;
        end = end->parent;
    }
    return sp;

}

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
    for(i=0; i<n; i++){
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

void performance()
{
    int n, i;
    Profiler p("bfs");

    // vary the number of edges
    for(n=1000; n<=4500; n+=100){
        Operation op = p.createOperation("bfs-edges", n);
        Graph graph;
        graph.nrNodes = 100;
        //initialize the nodes of the graph
        graph.v = (Node**)malloc(graph.nrNodes * sizeof(Node*));
        for(i=0; i<graph.nrNodes; ++i){
            graph.v[i] = (Node*)malloc(sizeof(Node));
            memset(graph.v[i], 0, sizeof(Node));
        }
        // TODO: generate n random edges
        // make sure the generated graph is connected
        generateRandomGraph(&graph,n,graph.nrNodes);

        bfs(&graph, graph.v[0], &op);
        free_graph(&graph);
    }

    // vary the number of vertices
    for(n=100; n<=200; n+=10){
        Operation op = p.createOperation("bfs-vertices", n);
        Graph graph;
        graph.nrNodes = n;
        //initialize the nodes of the graph
        graph.v = (Node**)malloc(graph.nrNodes * sizeof(Node*));
        for(i=0; i<graph.nrNodes; ++i){
            graph.v[i] = (Node*)malloc(sizeof(Node));
            memset(graph.v[i], 0, sizeof(Node));
        }
        // TODO: generate 4500 random edges
        // make sure the generated graph is connected
        generateRandomGraph(&graph,4500,graph.nrNodes);

        bfs(&graph, graph.v[0], &op);
        free_graph(&graph);
    }

    p.showReport();
}
