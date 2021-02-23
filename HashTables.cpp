#include <iostream>
#include <climits>
#include "Profiler.h"
#include <cstring>

#define MAX_SIZE 9973
#define c1 6029
#define c2 929
#define r 5
#define ERR -1

using namespace std;
int oBU;
Profiler profiler("mergeKStable");

typedef struct
{
    int id;
    char name[30];
} Entry;

void hash_empty(Entry a[],int n)
{
    for(int i=0; i<n; i++)
    {
        a[i].id=ERR;
        strcpy(a[i].name, "");
    }
}
int h(int k, int i,int m)
{
    return (k + c1*i + c2*i*i) % m;
}

int equal_data(Entry a, Entry b)
{
    if(a.id == b.id && strcmp(a.name, b.name)==0)
        return a.id;
    return ERR;
}

int empty_pos(Entry k)
{
    if(k.id == 0 && strcmp(k.name,"")==0)
        return ERR;
    return k.id;
}

int hash_insert(Entry T[], Entry k, int m)
{
   for(int i=0; i<=m; i++)
   {
    int j=h(k.id,i,m);
    if(empty_pos(T[j]) == ERR)
    {
        T[j].id=k.id;
        strcpy(T[j].name, k.name);
        return j;
    }
   }
   return ERR;
}

int hash_search(Entry T[], Entry k, int m)
{
    for(int i=0; i<=m; i++)
    {
        int j=h(k.id,i,m);
        oBU++;
        if(T[j].id == k.id)
            return j;
        if(empty_pos(T[j]) == ERR)
            return ERR;
    }
    return ERR;
}
float table[5][4];
int main()
{
    //Entry hasht[13];
    int m = MAX_SIZE;
    /*Entry temp1,temp2,temp3;
    temp1.id=100;
    strcpy(temp1.name , "Vlad");
    hash_insert(hasht,temp1,m);
    temp2.id=10073;
    strcpy(temp2.name,"Mihai");
    hash_insert(hasht,temp2,m);
    temp3.id=10072;
    strcpy(temp3.name , "Mihai");
    for(int j=0;j<m;j++){
    for(int i = 0; i < m; i++)
        cout<<h(j,i,m)<<" ";
    cout<<endl;
    }
    cout << hash_search(hasht,temp1,m) << endl;
    cout << hash_search(hasht,temp2,m) << endl;
    cout << hash_search(hasht,temp3,m) << endl;*/


    for(int runs = 0; runs<r; runs++)
        for(float ff=0.8; ff<=1;ff+=0.05)
        {
            int v[2*MAX_SIZE], ff_no=(int)(((float)m)*ff),maxFound=0,maxNotFound=0;
            Entry temp;
            //temp.name = (char*)malloc(30*sizeof(char));
            char str[] ="name";
            strcpy(temp.name,str);
            //temp.name[0]='n';

            FillRandomArray(v, 2*MAX_SIZE,0,INT_MAX-1,true);
            Entry hasht[MAX_SIZE];
            hash_empty(hasht,MAX_SIZE);
            if(ff == 1)
                ff_no=(int)(((float)m)*0.99);
            for(int i=0;i< ff_no;i++)
            {
                temp.id = v[i];
                //strcpy(temp.name,"name");
                hash_insert(hasht,temp,m);
            }

            maxFound=0;
            maxNotFound=0;

            for(int j=0;j<1500;j++)
            {
                oBU=0;
                int index = rand()%ff_no;
                temp.id = v[index];
                //strcpy(temp.name,"name");
                if(hash_search(hasht,temp,m)==ERR)
                    return ERR;
                table[(int)((ff-0.79)/0.05)][0]+=oBU;
                if(oBU>maxFound)
                    maxFound=oBU;

            }
            for(int j=0;j<1500;j++)
            {
                oBU=0;
                int index = rand()%ff_no;
                temp.id = v[MAX_SIZE+index];
                //strcpy(temp.name,"name");
                if(hash_search(hasht,temp,m)!=ERR)
                    return ERR;
                table[(int)((ff-0.79)/0.05)][2]+=oBU;
                if(oBU>maxNotFound)
                    maxNotFound=oBU;
            }
            table[(int)((ff-0.79)/0.05)][1]+=maxFound;
            table[(int)((ff-0.79)/0.05)][3]+=maxNotFound;

        }
        for(int i=0; i<6;i++)
        {
        table[i][0]/=(1500*r);
        table[i][2]/=(1500*r);
        table[i][1]/=5;
        table[i][3]/=5;
        }
        for(int i=0;i<5;i++)
        {
            for(int j=0;j<4;j++)
                cout<<table[i][j]<<" ";
            cout<<endl;
        }
    return 0;
}
