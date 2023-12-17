#include<stdio.h>
#include<stdlib.h>
struct node* createheader();
void inlist(struct node* header,int x);
void inlast(struct node* header,int x);
struct node* find(struct node* header,int x);
struct node* findxx(struct node* header,int n);
void delete(struct node* header,int x);
void plist(struct node* header);   
void reorder(struct node* header);
void renode(struct node* header);
struct node{
    int date;
    struct node *next;
};
int main(){
    struct node* header;
    struct node* ppp;
    //创建头节点
    header=createheader();
    //头插
    puts("--------inlist--1-5--------");
    int i = 1;
    while(i<=500){
        inlist(header,i);
        i++;
    }
    plist(header);
    //尾插
    puts("---------inlast--1-5------");
    while(i<=1000){
        inlast(header,i);
        i++;
    }
    plist(header);
    //查date
    puts("---------fin--2--------");
    ppp = find(header,2);
    if(ppp != NULL)
        printf("%d\n",ppp->date);
    else
    printf("no");
    //查位置
    puts("---------findxx---3-------");
    ppp = findxx(header,3);
    if(ppp != NULL)
        printf("%d\n",ppp->date);
    else
    printf("no");
    //删
    puts("---------delete---2-------");
    delete(header,2);
    plist(header);
    puts("---------reorder----------");
    reorder(header);
    puts("---------free----------");
    renode(header);
    return 0;
}

struct node* createheader(){
    struct node* p;
    p=(struct node*)malloc(sizeof(struct node));
    if(p == NULL)return NULL;
    p->next = NULL;
    return p;
}
void inlist(struct node* header,int x){
    struct node* p;
    p = (struct node*)malloc(sizeof(struct node));
    p->date = x;
    p->next = header->next;
    header->next = p;
}
void inlast(struct node* header,int x){
    struct node* p;
    p = (struct node*)malloc(sizeof(struct node));
    p->date = x;
    p->next = NULL;
    while(header->next != NULL)
        header = header->next;
    header->next = p;
}
struct node* find(struct node* header,int x){
    struct node* p;
    p  = header->next;
    while(p != NULL && p->date != x){
        p = p->next;
    }
    return p;
}
struct node* findxx(struct node* header,int n){
    struct node* p = header->next;
    int i = 1;
    while(p != NULL && i != n){
        p = p->next;
        i++;
    }
    return  p;
}
void delete(struct node* header,int x){
    struct node* p = header->next;
    struct node* pir = header;
    while(p != NULL){
        if(p->date == x){
            pir->next = p->next;
            free(p);
            break;
        }
        else{
            pir = p;
            p = p->next;
        }
    }
}
void plist(struct node* header){
    struct node* p;
    if(header == NULL){
        printf("NULL\n");
        exit(1);
    }
    p = header->next;
    while(p != NULL && header != NULL){
        printf("%d ",p->date);
        p = p->next;
    }
    printf("\n");
}
void reorder(struct node* header){
    struct node* p;
    struct node* temp = header->next;
    struct node* new_last = NULL;
    while(temp != NULL){
        p = temp;
        temp = temp->next;//header右移
        p->next = new_last;//断出的节点指向新建的尾节点，依次...
        new_last = p;//头节点置为p
    }
    header->next = new_last;
    plist(header);
}
void renode(struct node* header){
    struct node* p = header;
    struct node* nextnode;
    while(p != NULL){
        nextnode = p->next;
        free(p);
        p = nextnode;
    }
    header = NULL;
}