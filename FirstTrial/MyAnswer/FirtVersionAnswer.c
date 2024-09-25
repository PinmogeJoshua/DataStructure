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

//新增书目及其基本信息，1
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

//删除指定书目，2
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

//修改指定书目信息，3
bool ModifyBook(BookList *list, int id, char *name, char *author, char *publisher, int year) {
    //寻找要修改的结点
    Book *p = list->head;
    while( p != NULL && p->id != id ) {
        p = p->next;
    }
    //若修改结点不存在
    if( p == NULL ) {
        printf("书目编号%d不存在\n",id);
        return false;
    }

    //修改结点
    strcpy( p->name, name );
    strcpy( p->author, author );
    strcpy( p->publisher, publisher );
    p->year = year;

    return true;
}

//输出最新出版的书目信息，4
bool GetLatestBook(BookList *list) {
    if(list->head == NULL) {
        printf("列表为空\n");
        return false;
    }

    //找到最新出版年份
    Book *current = list->head;
    int latestYear = current->year;
    while(!current) {
        if(current->year > latestYear) {
            latestYear = current->year;
        }
        current++;
    }

    //重置current指针
    current = list->head;

    //
    printf("最新出版的书目：\n");
    while(current != NULL) {
        if(current->year == latestYear ) {
            printf("编号：%d\n名称：%s\n作者：%s\n出版社：%s\n年份：%d\n", 
                    current->id, current->name, current->author, current->publisher, current->year);
        }
        current++;
    }

    return true;
}

// SaveBooks函数声明
void SaveBooks(BookList *list, const char *filename);

//释放链表程序并退出空间，0
bool FreeBookspace(BookList *list, const char *filename) {
    SaveBooks(list, filename);
    //当前节点指向链表头结点
    Book *current = list->head;

    //遍历链表，释放每个节点的内容
    while(current != NULL) {
        current = list->head;
        list->head = list->head->next;
        free(current);
    }
    free(list);         //释放链表结构体内存
    return true;
}

//从csv文件中读取信息，录入至链表中
bool loadBook(BookList *list, const char *filename) {
    //
    FILE *fp = fopen("booklist.csv","r");
    //
    if(!fp) {
        printf("无法打开文件%s\n", filename);
        return false;
    }
    else {  //写入数据
        char tmpstr[256];
        while(fgets(tmpstr, sizeof(tmpstr), fp)) {
            int id, year;
            char name[100], author[100], publisher[100];
            //解析每行数据至列表中
            sscanf(tmpstr, "%d,%99[^,],%99[^,],%99[^,],%d", &id, name, author, publisher, &year);
            int i = list->count; // 插入位置
            InsertBook(list, id, name, author, publisher, year, i);
        }
    }
    fclose(fp);
    return true;
}

//保存信息
void SaveBooks(BookList *list, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("无法打开文件");
        return;
    }
    Book *current = list->head;
    while (current) {
        fprintf(file, "%d,%s,%s,%s,%d\n", current->id, current->name, current->author, current->publisher, current->year);
        current = current->next;
    }
    fclose(file);
}

//主函数
int main() {
    BookList *list = InitBookList();
    loadBook(list,"book.csv");

    int choice;
    int id, year;
    char name[100], author[100], publisher[100];

    do{
        printf("请问您想进行哪一种操作业务？\n");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                printf("请输入书目信息（编号 名称 作者 出版社 出版年份）：\n");
                scanf("%d %99s %99s %99s %d", &id, name, author, publisher, &year);
                InsertBook(list, id, name, author, publisher, year, list->count);
                break;
            case 2:
                printf("请输入要删除的书目编号：\n");
                scanf("%d", &id);
                DeleteBook(list, id);
                break;
            case 3:
                printf("请输入要修改的书目编号及新的信息（编号 名称 作者 出版社 出版年份）：\n");
                scanf("%d %99s %99s %99s %d", &id, name, author, publisher, &year);
                ModifyBook(list, id, name, author, publisher, year);
                break;
            case 4:
                GetLatestBook(list);
                break;
        }
    } while (choice != 0);  //当用户选择0时，退出循环

    FreeBookspace(list,"book.csv");
    return 0;
}
