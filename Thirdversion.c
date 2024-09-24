#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<math.h>

//定义书目结构体
typedef struct Book {   
    int id;             //书目编号
    char name[100];    //书籍名称
    char author[100];   //作者名字
    char publisher[100];//出版社名称
    int year;           //出版年份
    struct Book *next;  //指向下一个书目节点的指针
} Book;

//定义链表结构体
typedef struct {
    Book *head;     //链表头节点
    int count;      //链表节点数量
} BookList;

//创建一个书目链表
BookList *InitBookList() {
    BookList *list = (BookList *)malloc(sizeof(BookList));
    if(list == NULL)
        exit(OVERFLOW);
    list->head = NULL;
    list->count = 0;
    return list;
}


//插入书目及其基本信息
bool InsertBook(BookList *list, int id, char *name, char *author, char *publisher, int year, int i) {
    // 检查输入参数是否为 NULL
    if (list == NULL || name == NULL || author == NULL || publisher == NULL) {
        return false;
    }

    // 检查插入位置是否合理
    if (i < 0 || i > list->count) {
        return false;
    }

    Book *newBook = (Book *)malloc(sizeof(Book));
    if (newBook == NULL) {
        return false;
    }

    // 初始化新书目节点
    newBook->id = id;
    strcpy(newBook->name, name);
    strcpy(newBook->author, author);
    strcpy(newBook->publisher, publisher);
    newBook->year = year;

    // 如果插入位置是头节点
    if (i == 0) {
        newBook->next = list->head;
        list->head = newBook;
    } 
    else {
        Book *p = list->head;
        int j = 0;

        // 查找第 i-1 个书目结点
        while (p && j < i - 1) {
            p = p->next;
            ++j;
        }

        // 插入新节点
        newBook->next = p->next;
        p->next = newBook;
    }

    // 更新链表节点计数
    list->count++;
    return true;
}

bool DeleteBook(BookList *list, int id) {
    Book *current = list->head;
    Book *previous = NULL;

    while (current != NULL && current->id != id) { // 遍历链表，找到要删除的节点
        previous = current; // 前一个节点指向当前节点
        current = current->next; // 当前节点指向下一个节点
    }
    if (current == NULL) { // 如果没有找到要删除的节点，打印提示信息并返回
        printf("书目编号 %d 不存在。\n", id);
        return false;
    }

    if (previous == NULL) { // 如果要删除的节点是头节点
        list->head = current->next; // 头节点指向下一个节点
    } 
    else {
        previous->next = current->next; // 前一个节点的next指向当前节点的下一个节点
    }
    free(current); // 释放当前节点的内存
    list->count--; // 链表节点计数减一
    
    return true;
}
