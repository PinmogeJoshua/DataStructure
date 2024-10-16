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

/*构建HuffmanTree*/
void SelectMinNodes(HuffmanTree *ht, int range, int *min1, int *min2) {
    *min1 = -1;
    *min2 = -1;
    for (int j = 1; j <= range; j++) {
        if ((*ht)[j].parent == 0) {    /*考虑叶子结点*/
            /*如果当前节点比min1更小，更新min2为min1，min1为当前节点*/
            if (*min1 == -1 || (*ht)[j].weight < (*ht)[*min1].weight) {   
                *min2 = *min1;
                *min1 = j;
            }
            /*如果当前节点比min2更小，更新min2为当前节点*/
            else if (*min2 == -1 || (*ht)[j].weight < (*ht)[*min2].weight) {
                *min2 = j;
            }
        }
    }
}

/*初始化HuffmanTree*/
void CreateHT(HuffmanTree *ht, int *weights, int n) {
    int m = 2 * n - 1;
    for (int i = 1; i <= n; i++) {
        (*ht)[i].weight = weights[i - 1];
        (*ht)[i].parent = 0;
        (*ht)[i].lchild = 0;
        (*ht)[i].rchild = 0;
    }
    for (int i = n + 1; i <= m; i++) {
        (*ht)[i].weight = 0;
        (*ht)[i].parent = 0;
        (*ht)[i].lchild = 0;
        (*ht)[i].rchild = 0;
    }

    /*创建非叶子结点，构建Huffman Tree*/
    for (int i = n + 1; i <= m; i++) {
        int min1, min2;    /*初始化两个最小权重结点的索引*/
        /*在ht［1］~ht［i-1］的范围内选择两个parent为0且weight最小的结点，其序号分别赋值给min1、 min2返回*/
        SelectMinNodes(ht, i - 1, &min1, &min2);

        /* 构建新的非叶子结点 */
        (*ht)[min1].parent = i;
        (*ht)[min2].parent = i;
        (*ht)[i].lchild = min1;
        (*ht)[i].rchild = min2;
        (*ht)[i].weight = (*ht)[min1].weight + (*ht)[min2].weight;
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

/*分数段判定*/
int ScoreSegment(HuffmanTree ht, int score,int root) {
    if(root == 0) {
        return -1;  /*无效结点*/
    }
    if(ht[root].lchild == 0 && ht[root].rchild == 0) {
        return root;    /*叶子结点*/
    }
    if(score <= ht[ht[root].lchild].weight) {
        return ScoreSegment(ht, score, ht[root].lchild);
    }
    else {
        return ScoreSegment(ht, score - ht[ht[root].lchild].weight, ht[root].rchild);
    }
}

/*与他人“判定树”程序进行比较*/
int CompareTrees(HuffmanTree ht1, int root1, HuffmanTree ht2, int root2) {
    if (root1 == 0 && root2 == 0) {
        return 1; // 两个节点都为空，视为相同
    }
    if (root1 == 0 || root2 == 0) {
        return 0; // 一个节点为空，另一个不为空，视为不同
    }
    if (ht1[root1].weight != ht2[root2].weight) {
        return 0; // 权重不同，视为不同
    }
    return CompareTrees(ht1, ht1[root1].lchild, ht2, ht2[root2].lchild) &&
           CompareTrees(ht1, ht1[root1].rchild, ht2, ht2[root2].rchild);
}

int main() {
    int weights[] = {5, 15, 40, 30, 10};
    int n = sizeof(weights) / sizeof(weights[0]);
    HuffmanTree ht1 = (HuffmanTree)malloc((2 * n) * sizeof(HTNode));

    CreateHT(&ht1, weights, n);
    PreOrderTraverse(ht1, 2 * n - 1);

    int score = 25;
    int segment = ScoreSegment(ht1, 2 * n - 1, score);
    if (segment != -1) {
        printf("Score %d falls into segment with weight %d\n", score, ht1[segment].weight);
    } else {
        printf("Score %d does not fall into any segment\n", score);
    }

    int weights2[] = {10, 15, 35, 30, 10};
    int m = sizeof(weights2) / sizeof(weights2[0]);
    HuffmanTree ht2 = (HuffmanTree)malloc((2 * m) * sizeof(HTNode));
    
    CreateHT(&ht2, weights2, m);
    PreOrderTraverse(ht2, 2 * m - 1);

    int areTreesEqual = CompareTrees(ht1, 2 * n - 1, ht2, 2 * m - 1);
    if (areTreesEqual) {
        printf("The two trees are identical.\n");
    } else {
        printf("The two trees are different.\n");
    }

    free(ht1);
    free(ht2);
    return 0;
}
