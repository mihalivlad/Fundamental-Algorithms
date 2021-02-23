/**
You are required to implement correctly and efficiently linear time transformations between
three different representations for a multi-way tree:
    R1: parent representation: for each key you are given the parent key, in a vector.
    R2: multi-way tree representation: for each node you have the key and a vector of
        children nodes
    R3: binary tree representation: for each node, you have the key, and two pointers: one to
        the first child node, and one to the brother on the right (i.e. the next brother node)

T1 - from R1 to R2
T1 complexity is O(n) but with O(n) additional memory.
    The strategy is to have an array of nodes and then we link.

T2 - from R2 to R3
T2 complexity is O(n).
**/

#include <iostream>
#define M 5
using namespace std;
struct NodeM
{
    int key;
    int sz;
    struct NodeM *children[M];
};

struct Node
{
    int key;
    struct Node *left;
    struct Node *right;
};

NodeM* newNode(int i)
{
    NodeM* x = new NodeM();
    x->key = i+1;
    x->sz = 0;
    return x;
}

Node* createNode(int i)
{
    Node* x = new Node();
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
            arr[r[i]-1]->children[arr[r[i]-1]->sz] = arr[i];
            arr[r[i]-1]->sz++;
        }
    }
    return root;
}

Node* transform2(NodeM* root, NodeM* proot, int pos)
{
    if(root == NULL)
        return NULL;
        Node* broot = createNode(root->key);
    if(root->children[0] !=NULL)
        broot->left = transform2(root->children[0],root,0);
    if(proot!=NULL && proot->children[pos] == root && proot->children[pos+1]!= NULL)
        broot->right = transform2(proot->children[pos+1],proot,pos+1);
    return broot;
}

void printBT(Node *root, int space)
{
	if (root != NULL)
    {
		for (int i = 0; i < space; i++)
			cout << "   ";
		cout << root->key << "\n";
		printBT(root->left, space + 1);
		printBT(root->right, space);
	}
}
int main()
{
    int r1[]={7,7,6,3,6,-1,6,7,3};
    NodeM* rootM = transform1(r1,9);
    //cout<<rootM->sz;
    Node* root = transform2(rootM,NULL,0);
    //cout<<root->left->right->right->key;
    printBT(root, 0);

    return 0;
}
