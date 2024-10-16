#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*构建Huffman Tree*/
typedef struct {
    int weight;
    int parent, lchild, rchild;
} HTNode, *HuffmanTree;

/*动态分配数组存放哈夫曼编码*/
typedef char **HuffmanCode; 

/*创建Huffman Tree*/
void CreateHT(HuffmanTree *ht, int *w, int n) {
    if(n <= 1)
        return;
    
    int m;  
    m = 2*n-1;  /*n个叶子节点，度为2的节点个数为n-1，所以结点总数2*(n-1)+1=2n-1*/
    ht=(HuffmanTree)malloc((m+1)*sizeof(HTNode)); /*0号单元未使用*/
    
    /*叶子结点初始化*/
    for(int i=1; i<=m; i++) {
        (*ht)[i].weight = w[i-1];  /*注意索引*/ 
        (*ht)[i].parent = 0;
        (*ht)[i].lchild = 0;
        (*ht)[i].rchild = 0;
    }
    /*非叶子结点初始化*/
    for(int i=n+1; i<=m; i++) {
        (*ht)[i].weight = 0;
        (*ht)[i].parent = 0;
        (*ht)[i].lchild = 0;
        (*ht)[i].rchild = 0;
    }

    /*创建非叶子结点，构建Huffman Tree*/
    for(int i=n+1; i<=m; i++)   {
        int min1 = -1, min2;    /*初始化两个最小权重结点的索引*/
        /*在ht［1］~ht［i-1］的范围内选择两个parent为0且weight最小的结点，其序号分别赋值给s1、 s2返回*/
        for(int j=1; j<=i-1; j++) {
            if((*ht)[j].parent == 0) {    /*考虑叶子结点*/
                /*如果当前节点比min1更小，更新min2为min1，min1为当前节点*/
                if( min1 == -1 || (*ht)[i].weight < (*ht)[min1].weight) {   
                    min2 = min1;
                    min1 = j;
                }
                /*如果当前节点比min2更小，更新min2为当前节点*/
                else if( min2 == -1 || (*ht)[i].weight < (*ht)[min2].weight){
                    min2 = j;
                }
            }
        }
        /*将min1和min2合并为新结点，并更新它们的父结点*/
        (*ht)[min1].parent = i;
        (*ht)[min2].parent = i;
        (*ht)[i].lchild = min1;
        (*ht)[i].rchild = min2;
    }
}

/*遍历哈夫曼树，并将其输出*/
void PreOrderTraverse(HuffmanTree ht, int root) {
    if( root == 0)
        return;
    printf("Node %d: weight = %d, parent = %d, lchild = %d, rchild = %d\n", 
           root, ht[root].weight, ht[root].parent, ht[root].lchild, ht[root].rchild);
    PreOrderTraverse(ht, ht[root].lchild);
    PreOrderTraverse(ht, ht[root].rchild);
}

/*编写分数段判定程序，并将其与某同学的“判定树”程序进行比较*/


int main() {
    int weights[] = {5, 15, 40, 30, 10};
    int n = sizeof(weights) / sizeof(weights[0]);
    HuffmanTree ht;

    CreateHT(&ht, weights, n);
    PrintHuffmanTree(ht, n);

    free(ht);
    return 0;
}
