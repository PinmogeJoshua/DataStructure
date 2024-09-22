#include <malloc.h>
#include <stdbool.h>
#include <math.h>

//定义DataType为int型
typedef int ElemType;

//定义线性表的最大长度
#define LIST_INT_SIZE 100
#define LISTINCREMENT 10

//定义线性表的结构体
typedef struct {
    ElemType *elem;
    int length;
    int size;
} SeqList;


bool InitList(SeqList *L){
    //构造一个空的线性表L
    L->elem = (ElemType *)malloc(LIST_INT_SIZE *sizeof(ElemType));
    if(! L->elem)
        exit(OVERFLOW);
    L->length = 0;
    L->size = LIST_INT_SIZE ;
    return 1;
}

bool ListInsert(SeqList *L, ElemType *newlist, int i, ElemType e, int *p, int *q){
    //在线性表第i个位置之前插入新的元素e
    //i的范围在1≤i小于等于List_Length_Sq(L)+1
    if(i<1||i>L->length+1)
        exit(OVERFLOW);

    if(L->length>=LIST_INT_SIZE){
        newlist = (ElemType *) realloc(L->elem, (LIST_INT_SIZE+LISTINCREMENT)*sizeof(ElemType));
        if(!newlist)
            exit(OVERFLOW);
        L->elem = newlist;          //新基址
        L->size += LISTINCREMENT;   //增容量
    }
    q = &L->elem[i-1];
    for(p = &(L->elem[L->length-1]); p>=q; --p)
        *(p+1) = *p;
    *q = e;
    ++L->length;
    return 1;
}
