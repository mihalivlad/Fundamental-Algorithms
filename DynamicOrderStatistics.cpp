/**
The management operations of an order statistics tree are:
- BUILD_TREE(n)
    - builds a balanced BST containing the keys 1,2,...n (hint: use a divide and conquer
    approach)
    - make sure you initialize the size field in each tree node
- OS-SELECT(tree, i)
    - selects the element with the ith smallest key
    - the pseudo-code is available in chapter 14.1 from the book1
- OS-DELETE(tree, i)
    - you may use the deletion from a BST, without increasing the height of the tree
    - keep the size information consistent after subsequent deletes
    - there are several alternatives to update the size field without increasing the
    complexity of the algorithm (it is up to you to figure this out).

    BUILD_TREE(n) complexity is O(n).
    OS-SELECT(tree, i) and OS-DELETE(tree, i) are O(log n)
    That two functions repeats n times
    Total complexity is O(n log n), n + n log n

**/
#include <iostream>
#include<stdio.h>
#include<stdlib.h>
#include <climits>
#include "Profiler.h"

#define MAX_SIZE 10000
#define m 5

using namespace std;
int oBU;
Profiler profiler("DOS");
struct Node
{
    int key;
    int sz;
    struct Node *left;
    struct Node *right;
    struct Node *p;
};

Node *createNode(int item)
{
    oBU+=3;
    Node *temp =  (Node *)malloc(sizeof(Node));
    temp->key = item;
    //cout<<item<<endl;
    temp->sz = 1;
    temp->left = temp->right = NULL;
    return temp;
}

Node* tree_sz(Node* root)
{
    if(root->left == NULL && root->right == NULL)
    {
        oBU++;
        root->sz = 1;
    }
    else if(root->left == NULL)
    {
        oBU+=2;
        root->sz = root->right->sz + 1;
        root->right->p = root;
    }
    else if(root->right == NULL)
    {
        oBU+=2;
        root->sz = root->left->sz + 1;
        root->left->p = root;
    }
    else
    {
        oBU+=3;
        root->sz = root->left->sz + root->right->sz + 1;
        root->right->p = root;
        root->left->p = root;
    }
    return root;
}

Node* build(int first, int last)
{
    if(first<=last)
    {
        oBU+=2;
        int mid = (first + last)/2;
        Node* root = createNode(mid);
        root->left = build(first, mid-1);
        root->right = build(mid+1,last);
        root = tree_sz(root);
        //cout<<root->sz<<endl;
        return root;
    }
    else
        return NULL;

}
Node* tree_min(Node* root, int del)
{
    oBU++;
    while(root->left != NULL)
    {
        if(del ==1)
        {
            oBU++;
            root->sz--;
        }
        oBU+=2;
        root = root->left;
    }
    return root;
}

Node* os_select(Node* x, int i, int del)
{
    int r=1;
    oBU++;
    if(x->left != NULL)
    {
        oBU++;
        r = x->left->sz + 1;
    }
    if(del == 1)
    {

        oBU++;
        x->sz = x->sz - 1;
        //cout<<r<<" sau "<<i<<endl;
    }
    if(r == i)
        return x;
    else if(i<r && x->left != NULL)
        return os_select(x->left,i,del);
    else return os_select(x->right,i-r,del);
}

Node* transplant(Node* root, Node* u, Node* v)
{

    oBU++;
    if(u->p == NULL)
    {
        oBU++;
        return v;
    }
    else
    {
        oBU+=2;
        if(u == u->p->left)
            u->p->left = v;
        else
            u->p->right = v;
    }
    oBU++;
    if(v != NULL)
    {
        oBU++;
        v->p = u->p;
    }
    return root;
}

Node* tree_delete(Node* root, Node* z)
{
    oBU+=2;
    if(z->left == NULL && z->right ==NULL)
    {
        oBU+=3;
        if(z->p == NULL)
            return NULL;
        if(z->p->left == z)
            z->p->left = NULL;
        else
           z->p->right = NULL;
    }
    else if(z->left == NULL)
    {
        oBU+=2;
        root = transplant(root,z,z->right);
        //z->sz = z->right->sz;
    }

    else if(z->right == NULL)
    {
        oBU+=3;
        root = transplant(root,z,z->left);
        //z->sz = z->left->sz;
    }
    else
    {
        oBU+=10;
        Node* y = tree_min(z->right,1);
        if(y->p != z)
        {
            oBU+=3;
            root = transplant(root,y,y->right);
            y->right = z->right;
            y->right->p = y;
            //swap(y->right->sz,y->sz);
        }
        root = transplant(root,z,y);
        y->left = z->left;
        y->left->p = y;
        swap(z->sz,y->sz);

    }
    //free(z);
    root->p = NULL;
    return root;

}

void printBST(Node *root, int space)
{
    if (root == NULL)
        return;
    space += 5;
    printBST(root->right, space);
    cout<<endl;
    for (int i = 5; i < space; i++)
        cout<<" ";
    cout<<root->key<<"-"<<root->sz<<"\n";
    printBST(root->left, space);
}


int main()
{
    /*8for(int a=3;a<MAX_SIZE ; a++)
    {
    Node* root = build(1,a);
    //printBST(root,0);
    //root = tree_delete(root,os_select(root,11,1));
    //printBST(root,0);
    //root = tree_delete(root,os_select(root,6,1));
    //printBST(root,0);
    for(int n=a; n>=1; n--)
         {
            int random=1;
            if(n !=1)
            random = 1+(rand()%(n-1));
            //cout<<random<<"-"<<n<<endl;
            Node* x = os_select(root,random,1);
            cout<<x->key<<" si "<<a<<endl;
            root = tree_delete(root,x);
            printBST(root,0);
         }
    }*/
    //Node* z=os_select(root,11,1);
    //cout<<z->key<<endl;
    for(int i=0; i<m; i++)
         for(int sz=100; sz<MAX_SIZE; sz += 100)
         {
             oBU = 0;
             Node* root = build(1,sz);
             profiler.countOperation("OStree-Build",sz,oBU);
             for(int n=sz; n>=1; n--)
         {
            oBU = 0;
            int random=1;
            if(n !=1)
            random = 1+(rand()%(n-1));
            //cout<<n<<"-"<<random<<endl;
            Node * x = os_select(root,random,1);
            profiler.countOperation("OStree-Select",sz,oBU);
            oBU = 0;
            root = tree_delete(root, x);
            profiler.countOperation("OStree-Delete",sz,oBU);
         }
         free(root);
         //cout<<sz <<endl;
     }
    profiler.divideValues("OStree-Build", 5);
    profiler.divideValues("OStree-Select", 5);
    profiler.divideValues("OStree-Delete", 5);
    profiler.addSeries("OS","OStree-Build","OStree-Select");
    profiler.addSeries("OStree","OS","OStree-Delete");
    profiler.createGroup("all", "OStree","OStree-Build","OStree-Select","OStree-Delete");
    profiler.showReport();

    return 0;
}
