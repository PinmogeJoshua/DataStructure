#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <unistd.h> // 添加这个头文件以使用 getcwd
#include <limits.h> // 添加这个头文件以使用 PATH_MAX
#include <locale.h> // 添加这个头文件以设置区域
#include<stdbool.h>
#include<math.h>

//定义书目结构体
typedef struct Book {
    char id[20];             // 书目编号，修改为字符串类型
    char name[100];          // 书籍名称
    char author[100];        // 作者名字
    char publisher[100];     // 出版社名称
    int year;                // 出版年份
    struct Book *next;       // 指向下一个书目节点的指针
} Book;

//定义链表结构体
typedef struct {
    Book *head;     //链表头节点
    int count;      //链表节点数量
} BookList;

void LoadBook(BookList *list, const char *filename) {
    // 设置区域设置为支持多字节字符
    setlocale(LC_ALL, "");
    printf("正在加载书目数据，文件路径：%s\n", filename);
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("无法打开文件");
        exit(EXIT_FAILURE); // 直接退出程序
    }
    printf("文件读取成功，开始加载书目...\n");
    char line[256];
    // 读取并处理每一行数据
    while (fgets(line, sizeof(line), file)) {
        Book *newBook = (Book *)malloc(sizeof(Book));
        if (newBook == NULL) {
            perror("内存分配失败");
            fclose(file);
            return;
        }
        int year;
        if (sscanf(line, "%19[^,],%99[^,],%99[^,],%99[^,],%d",
                   newBook->id, newBook->name, newBook->author, newBook->publisher, &year) == 5) {
            newBook->year = year; // 正确读取年份
            newBook->next = list->head; // 将新书目插入链表头部
            list->head = newBook;
            list->count++;
            printf("已加载书目：编号=%s, 名称=%s, 作者=%s, 出版社=%s, 年份=%d\n",
                   newBook->id, newBook->name, newBook->author, newBook->publisher, newBook->year);
        } else {
            fprintf(stderr, "读取格式错误：%s\n", line);
            free(newBook); // 释放未使用的内存
        }
    }
    fclose(file);
    printf("所有书目加载完成。\n");
}

//创建一个书目链表
BookList *InitBookList() {
    BookList *list = (BookList *)malloc(sizeof(BookList));
    if (list == NULL) {
        perror("内存分配失败");
        exit(EXIT_FAILURE);
    }
    list->head = NULL;
    list->count = 0;
    return list;
}

//新增书目及其基本信息，1
bool InsertBook(BookList *list, char *id, char *name, char *author, char *publisher, int year, int i) {
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
    strcpy(newBook->id, id);
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

        if (newBook == NULL || p == NULL) {
            fprintf(stderr, "指针为 null，无法插入新节点。\n");
            exit(EXIT_FAILURE);
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
bool DeleteBook(BookList *list, char *id) {
    Book *current = list->head;
    Book *previous = NULL;

    while (current != NULL && strcmp(current->id, id) != 0) { // 遍历链表，找到要删除的节点
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
bool ModifyBook(BookList *list, char *id, char *name, char *author, char *publisher, int year) {
    //寻找要修改的结点
    Book *p = list->head;
    while( p != NULL && strcmp(p->id, id) != 0 ) {
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

    Book *current = list->head;
    int latestYear = current->year;
    while (current != NULL) {
        if(current->year > latestYear) {
            latestYear = current->year;
        }
        current = current->next; // 修正这里
    }

    // 重置 current 指针
    current = list->head;

    printf("最新出版的书目：\n");
    while(current != NULL) {
        if(current->year == latestYear) {
            printf("编号：%s\n名称：%s\n作者：%s\n出版社：%s\n年份：%d\n",
                   current->id, current->name, current->author, current->publisher, current->year);
        }
        current = current->next;
    }

    return true;
}

// SaveBooks函数声明
void SaveBooks(BookList *list, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("无法打开文件%s\n", filename);
        return;
    }

    Book *current = list->head;
    while (current != NULL) {
        fprintf(fp, "%s,%s,%s,%s,%d\n", current->id, current->name, current->author, current->publisher, current->year);
        current = current->next;
    }

    fclose(fp);
}

//释放链表程序并退出空间，0
void FreeBookspace(BookList *list, const char *filename) {
    SaveBooks(list, filename);
    Book *current = list->head;
    while(current != NULL) {
        Book *temp = current; // 临时指针
        current = current->next;
        free(temp);
    }
    free(list);
}

//主函数
int main() {
    printf("程序开始运行...\n");
    BookList *list = InitBookList();
    LoadBook(list, "C:\\VSCode\\CcodeProjects\\booklist.csv");

    int choice;
    int year;
    char id[20];
    char name[100], author[100], publisher[100];

    do {
        printf("请问您想进行哪一种操作业务？\n");
        if (scanf("%d", &choice) != 1) {
            printf("输入无效，请重试。\n");
            while (getchar() != '\n'); // 清空输入缓冲区
            continue;
        }
        printf("您选择的操作是：%d\n", choice);
        
        switch (choice) {
            case 1:
                printf("请输入书目信息（编号 名称 作者 出版社 出版年份）：\n");
                scanf("%s %99s %99s %99s %d", id, name, author, publisher, &year);
                InsertBook(list, id, name, author, publisher, year, list->count);
                break;
            case 2:
                printf("请输入要删除的书目编号：\n");
                scanf("%s", id);
                DeleteBook(list, id);
                break;
            case 3:
                printf("请输入要修改的书目编号及新的信息（编号 名称 作者 出版社 出版年份）：\n");
                scanf("%s %99s %99s %99s %d", id, name, author, publisher, &year);
                ModifyBook(list, id, name, author, publisher, year);
                break;
            case 4:
                GetLatestBook(list);
                break;
        }
    } while (choice != 0);

    FreeBookspace(list, "book.csv");
    printf("程序结束。\n");
    return 0;
}
