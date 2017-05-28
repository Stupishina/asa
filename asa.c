#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define N 256

int qsi=0, tsi=0, ts=0, trsi=0;
double cltsh=0;

typedef struct pack {
    double num;
    unsigned int date;
    } PACK;

void reset(PACK *a, PACK *b, long K) {
int i;
for (i=0; i<K; i++)
    a[i]=b[i];
}

unsigned int ForSearching (PACK *a, int K) {
int i;
PACK p=a[0];
unsigned int x;
for (i=1; i<K; i++)
    if (a[i].num<p.num) x=a[i].date;
return (x);
}

int main(int argc, char *argv[]) {
clock_t start, finish;
char ch, name[N];
PACK *arr, *copy, temp;
int i=0, j=0, k=0, p=0;
long K=0;
unsigned int x;
long int tt;
FILE *File;

if (argc!=2) {
    printf("Enter file name.\n");
    scanf("%s", name);
}
else
    strcpy(name, argv[1]);
File=fopen(name, "rb");
while (File == NULL) {
    printf("File not found.\nTry again.\n");
    scanf("%s", name);
    File = fopen(name, "rb");
}

while (fscanf(File,"%c",&ch) != EOF)
if (ch=='\n') K++;
rewind (File);
arr=(PACK*)malloc(K*sizeof(PACK));
copy=(PACK*)malloc(K*sizeof(PACK));
while (fscanf(File, "%lf,%u", &arr[i].num, &arr[i].date)!=EOF) {
i++;
}
fclose(File);
x=ForSearching(arr, K);



reset (copy, arr, K);
start=clock();
BubbleSort (copy, K);
finish=clock();
printf("BubbleSort Time: %f\n", ((double)(finish - start)) / CLOCKS_PER_SEC);
reset (copy, arr, K);

start=clock();
QuickSort(copy, 0, K-1);
finish=clock();
printf ("Iterations in QuickSort: %u\n", qsi);
printf("QuickSort Time: %f\n", ((double)(finish - start)) / CLOCKS_PER_SEC);
reset (copy, arr, K);

start=clock();
InsertionSort(copy, K);
finish=clock();
printf("InsertionSort Time: %f\n", ((double)(finish - start)) / CLOCKS_PER_SEC);
reset (copy, arr, K);

start=clock();
TreeSort (copy, K, x);
finish=clock();
printf ("Iterations in TreeSort: %u\n", tsi);
printf("TreeSort Time: %f\n", (((double)(finish - start)) / CLOCKS_PER_SEC)-cltsh);

start=clock();
int ls=LinearSearch (arr, K, x);
finish=clock();
printf("LinearSearch Time: %f\n", ((double)(finish - start)) / CLOCKS_PER_SEC);

start=clock();
int fls_ns=QuickLinearSearch_NotSorted (arr, K, x);
finish=clock();
printf("QuickLinearSearch_NotSorted Time: %f\n", ((double)(finish - start)) / CLOCKS_PER_SEC);

start=clock();
int fls_s=QuickLinearSearch_Sorted (copy, K, x);
finish=clock();
printf("QuickLinearSearch_Sorted Time: %f\n", ((double)(finish - start)) / CLOCKS_PER_SEC);

start=clock();
int bs=BinarySearch (copy, K, x);
finish=clock();
printf("BinarySearch Time: %f\n", ((double)(finish - start)) / CLOCKS_PER_SEC);

start=clock();
int bls=BlockSearch(copy, K, x);
finish=clock();
printf("BlockSearch Time: %f\n", ((double)(finish - start)) / CLOCKS_PER_SEC);

printf ("Iterations in TreeSearch: %u\n", trsi);
printf("TreeSearch Time: %f\n", cltsh);
}



void BubbleSort (PACK *a, long K) {
int i, j, k;
unsigned long long int iter=0;
PACK temp;
for (i=0; i<K; i++){
    iter++;
    for (j=0; j<K-i-1; j++){
        iter++;
        if (a[j].date>a[j+1].date) {
            iter+=3;
            temp=a[j];
            a[j]=a[j+1];
            a[j+1]=temp;
            }
        }
    }
printf ("Iterations in BubbleSort: %llu\n", iter);
}

void QuickSort(PACK *a, long l, long r) {
int j;
if(l<r) {
    j = partition(a, l, r);
    QuickSort(a, l, j-1);
    QuickSort(a, j+1, r);
   }
}

int partition(PACK *a, long l, long r) {
int i, j;
PACK pivot, temp;
pivot=a[l];
i = l; j = r+1;
while(1) {
    qsi++;
    do {++i; qsi++;} while(a[i].date<=pivot.date && i<=r);
   	do {--j; qsi++;} while(a[j].date>pivot.date);
   	if(i>=j) {qsi++; break;}
   	temp=a[i]; a[i]=a[j]; a[j]=temp;
   	qsi+=3;
    }
temp=a[l]; a[l]=a[j]; a[j]=temp;
qsi+=3;
return j;
}

void InsertionSort (PACK *a, long K) {
int loc, i;
unsigned int iter=0;
PACK elem;
for (i=1; i<K; i++) {
    iter++;
    elem=a[i];
    loc=i-1;
    while(loc>=0 && a[loc].date>elem.date) {
        iter++;
        a[loc+1]=a[loc];
        loc--;
        }
    a[loc+1]=elem;
    }
printf ("Iterations in InsertionSort: %u\n", iter);
}

typedef struct tree {
    PACK a;
    struct tree *left;
    struct tree *right;
  } TREE;

TREE *add_to_tree (TREE *root, PACK new_value) {
    tsi++;
    if (root==NULL)
        {
        root=(TREE*)malloc(sizeof(TREE));
        root->a = new_value;
        root->left = root->right = 0;
        return root;
        }
    if (root->a.date<new_value.date) {
        root->right=add_to_tree(root->right, new_value);
        tsi++;
    }
    else {
        root->left=add_to_tree(root->left, new_value);
        tsi++;
    }
    return root;
}

void tree_to_array (TREE *root, PACK *a) {
    tsi++;
    static max2=0;
    if (root==NULL) return;
    tree_to_array(root->left, a);
    a[max2++]=root->a;
    tree_to_array(root->right, a);
    free(root);
  }

void TreeSort (PACK *a, int max, unsigned int x) {
int i;
TREE *root = 0;
for (i=0; i<max; i++) {
    tsi++;
    root=add_to_tree(root, a[i]);
    }
TreeSearch(root, x);
tree_to_array(root, a);
}

void TreeSearch(TREE *root, unsigned int x) {
clock_t begin, end;
trsi++;
begin=clock();
while ((root!=NULL) && root->a.date!=x) {
    trsi++;
    if (x<root->a.date)
        return TreeSearch(root->left, x);
    else if (x>root->a.date)
        return TreeSearch(root->right, x);
    }
end=clock();
cltsh=(double)(begin - end) / CLOCKS_PER_SEC;
}

int LinearSearch (PACK *a, int K, unsigned int x) {
int i;
unsigned int iter=0;
for (i=0; i<K; i++) {
    iter++;
    if (a[i].date==x) {
        iter++;
        printf ("Iterations in LinearSearch: %u\n", iter);
        return i;
        }
    }
return -1;
}

int QuickLinearSearch_NotSorted (PACK *a, int K, unsigned int x) {
int i=0;
unsigned int iter=0;
PACK New;
New.date=x;
a[K+1]=New;
while (a[i].date!=x) {++i; ++iter;}
printf ("Iterations in FastLinearSearch_NotSorted: %u\n", iter);
if (i!=K+1) return i;
else return -1;
}

int QuickLinearSearch_Sorted (PACK *a, int K, unsigned int x) {
int i=0;
unsigned int iter=0;
PACK New;
New.date=x;
a[K+1]=New;
do {i++; iter++;}
while (a[i].date<x);
printf ("Iterations in FastLinearSearch_Sorted:%u\n", iter);
if (i!=K+1) return i;
else return -1;
}

int BinarySearch(PACK *a, int K, unsigned int x) {
int low, high, middle;
unsigned int iter=0;
low = 0;
high = K-1;
while (low<=high) {
    iter++;
    middle=(low + high)/2;
    if (x<a[middle].date) {
        high=middle-1;
        iter++;
    }
    else if (x>a[middle].date) {
        low=middle+1;
        iter++;
    }
    else {
        printf ("Iterations in BinarySearch: %u\n", iter);
        if (x==a[middle].date) return middle;
            else return -1;
        }
    }
}

int BlockSearch(PACK *a, int K, unsigned int x) {
int f=1, i=0, l, in=(int)pow(K,(1/2));
unsigned int iter=0;
while ((i<=K)&&(f)) {
    iter++;
    i=i+in;
    if (i>K) {i=K; iter++;}
    int r=i;
    l=i-3;
    if (x<a[i].date) {
        while ((l<=r)&&(x!=a[l].date)) {iter++; l++;}
        f=0;
        printf ("Iterations in BlockSearch: %u\n", iter);
        if (x==a[l].date) return l;
        else return -1;
        }
    }
}

