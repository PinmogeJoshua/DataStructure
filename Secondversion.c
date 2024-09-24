#include <malloc.h>
#include <stdbool.h>
#include <math.h>

//定义DataType为int型
typedef int ElemType;

//定义线性表的最大长度
#define LIST_INT_SIZE 100
#define LISTINCREMENT 10

//定义线性表的结构体
typedef struct LNode{
    ElemType data;
    struct LNode *next;
} LNode, *LinkList;


bool InitList(LinkList *L) {
    *L = (LinkList)malloc(sizeof(LNode));
    if (*L == NULL) {
        return false; // 内存分配失败
    }
    (*L)->next = NULL; // 初始化头节点的next指针
    return true; // 初始化成功
}

bool ListInsert(LinkList *L, int i, int e) {
    LinkList p = *L; // p指向链表的头节点
    int j = 0;

    // 找到第i-1个节点
    while (p && j < i - 1) {
        p = p->next;
        ++j;
    }

    // 如果p为空或者j大于i-1，说明插入位置不合法
    if (!p || j > i-1) {
        return false;
    }

    // 创建新节点
    LinkList s = (LinkList)malloc(sizeof(LNode));
    if (!s) {
        return false; // 内存分配失败
    }
    s->data = e;

    // 插入新节点
    s->next = p->next;
    p->next = s;

    return true;
}

bool ListDelete(LinkList *L, int i, int *e) {
    LinkList p = *L;    //p指向链表的头节点
    int j = 0;

    //找到第i个节点
    while(p && j < i-1) {
        p = p->next;
        j++;
    }

    // 如果p为空或者j大于i-1，说明删除位置不合法
    if(!p || j > i-1) {
        return false;
    }


    LinkList q = p->next; // q指向要删除的节点
    *e = q->data;         // 将要删除节点的值赋给e
    p->next = q->next;    // 将p的next指针指向q的下一个节点
    free(q);      // 释放q节点的内存

    return true;
}
