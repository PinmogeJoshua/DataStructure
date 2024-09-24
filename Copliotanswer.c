#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 定义书目结构体
typedef struct Book {
    int id; // 书目编号
    char name[100]; // 书目名称
    char author[100]; // 作者
    char publisher[100]; // 出版社
    int year; // 出版年份
    struct Book *next; // 指向下一个书目节点的指针
} Book;

// 定义链表结构体
typedef struct {
    Book *head; // 链表的头节点
    int count; // 链表中节点的数量
} BookList;

// 创建一个新的书目链表
BookList* createBookList() {
    BookList *list = (BookList *)malloc(sizeof(BookList)); // 分配内存
    list->head = NULL; // 初始化头节点为NULL
    list->count = 0; // 初始化节点数量为0
    return list; // 返回创建的链表
}

void addBook(BookList *list, int id, char *name, char *author, char *publisher, int year) {
    Book *newBook = (Book *)malloc(sizeof(Book)); // 分配新书目节点的内存
    newBook->id = id; // 设置书目编号
    strcpy(newBook->name, name); // 设置书目名称
    strcpy(newBook->author, author); // 设置作者
    strcpy(newBook->publisher, publisher); // 设置出版社
    newBook->year = year; // 设置出版年份
    newBook->next = list->head; // 新书目节点的next指向当前头节点
    list->head = newBook; // 将新书目节点设置为链表的头节点
    list->count++; // 链表节点计数加一
}

void deleteBook(BookList *list, int id) {
    Book *current = list->head; // 当前节点指向链表的头节点
    Book *previous = NULL; // 前一个节点初始化为NULL
    while (current != NULL && current->id != id) { // 遍历链表，找到要删除的节点
        previous = current; // 前一个节点指向当前节点
        current = current->next; // 当前节点指向下一个节点
    }
    if (current == NULL) { // 如果没有找到要删除的节点，打印提示信息并返回
        printf("书目编号 %d 不存在。\n", id);
        return;
    }
    if (previous == NULL) { // 如果要删除的节点是头节点
        list->head = current->next; // 头节点指向下一个节点
    } else {
        previous->next = current->next; // 前一个节点的next指向当前节点的下一个节点
    }
    free(current); // 释放当前节点的内存
    list->count--; // 链表节点计数减一
}

void modifyBook(BookList *list, int id, char *name, char *author, char *publisher, int year) {
    Book *current = list->head; // 当前节点指向链表的头节点
    while (current != NULL && current->id != id) { // 遍历链表，找到要修改的节点
        current = current->next; // 当前节点指向下一个节点
    }
    if (current == NULL) { // 如果没有找到要修改的节点，打印提示信息并返回
        printf("书目编号 %d 不存在。\n", id);
        return;
    }
    strcpy(current->name, name); // 修改书目名称
    strcpy(current->author, author); // 修改作者
    strcpy(current->publisher, publisher); // 修改出版社
    current->year = year; // 修改出版年份
}

void printLatestBook(BookList *list) {
    if (list->head == NULL) { // 如果链表为空，打印提示信息并返回
        printf("书目列表为空。\n");
        return;
    }
    Book *current = list->head; // 当前节点指向链表的头节点
    Book *latest = current; // 最新出版的书目节点初始化为头节点
    while (current != NULL) { // 遍历链表，找到最新出版的书目
        if (current->year > latest->year) { // 如果当前节点的出版年份大于最新出版的书目节点的出版年份
            latest = current; // 更新最新出版的书目节点
        }
        current = current->next; // 当前节点指向下一个节点
    }
    // 打印最新出版的书目信息
    printf("最新出版的书目：\n编号：%d\n名称：%s\n作者：%s\n出版社：%s\n年份：%d\n",
           latest->id, latest->name, latest->author, latest->publisher, latest->year);
}

void freeBookList(BookList *list) {
    Book *current = list->head; // 当前节点指向链表的头节点
    while (current != NULL) { // 遍历链表，释放每个节点的内存
        Book *next = current->next; // 保存当前节点的下一个节点
        free(current); // 释放当前节点的内存
        current = next; // 当前节点指向下一个节点
    }
    free(list); // 释放链表结构体的内存
}

void loadBooksFromCSV(BookList *list, const char *filename) {
    FILE *file = fopen(filename, "r"); // 打开CSV文件
    if (!file) { // 如果文件打开失败，打印提示信息并返回
        printf("无法打开文件 %s\n", filename);
        return;
    }
    char line[256]; // 用于存储每行数据的缓冲区
    while (fgets(line, sizeof(line), file)) { // 逐行读取文件内容
        int id, year;
        char name[100], author[100], publisher[100];
        // 解析每行数据并添加到链表中
        sscanf(line, "%d,%99[^,],%99[^,],%99[^,],%d", &id, name, author, publisher, &year);
        addBook(list, id, name, author, publisher, year);
    }
    fclose(file); // 关闭文件
}

int main() {
    BookList *list = createBookList(); // 创建书目链表
    loadBooksFromCSV(list, "books.csv"); // 从CSV文件加载书目

    int choice;
    do {
        // 打印操作选择菜单
        printf("请输入操作选择：\n1. 新增书目\n2. 删除书目\n3. 修改书目\n4. 输出最新出版的书目\n0. 退出程序\n");
        scanf("%d", &choice); // 获取用户选择
        if (choice == 1) {
            int id, year;
            char name[100], author[100], publisher[100];
            // 获取新增书目的信息
            printf("请输入书目编号、名称、作者、出版社和年份：\n");
            scanf("%d %99s %99s %99s %d", &id, name, author, publisher, &year);
            addBook(list, id, name, author, publisher, year); // 新增书目
        } else if (choice == 2) {
            int id;
            // 获取要删除的书目编号
            printf("请输入要删除的书目编号：\n");
            scanf("%d", &id);
            deleteBook(list, id); // 删除书目
        } else if (choice == 3) {
            int id, year;
            char name[100], author[100], publisher[100];
            // 获取要修改的书目的信息
            printf("请输入要修改的书目编号、名称、作者、出版社和年份：\n");
            scanf("%d %99s %99s %99s %d", &id, name, author, publisher, &year);
            modifyBook(list, id, name, author, publisher, year); // 修改书目
        } else if (choice == 4) {
            printLatestBook(list); // 输出最新出版的书目
        }
    } while (choice != 0); // 当用户选择0时退出循环

    freeBookList(list); // 释放书目链表的内存
    return 0; // 程序结束
}
