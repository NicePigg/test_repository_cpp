// 互斥锁+顺序表
// 线程从a.txt中读取一个字符写入顺序表
// 主线程从键盘接收字符写入顺序表
// 顺序表满后不允许继续写入操作
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#define  SQL_SIZE 100

// 顺序表：内存连续，结构体里是一个固定长度的数组
struct sql{
    char array[SQL_SIZE];
    int last;
};

// 结构体嵌套顺序表结构体+mutex
struct data{
    struct sql seq_list;
    pthread_mutex_t mutex;
};

struct data* request_data_and_init(void){
    struct data *glob;
    glob = malloc(sizeof(struct data));  // 申请一个结构体
    if(glob == NULL){
        perror("malloc error:");
        return NULL;
    }
    glob->seq_list.last = -1;
    pthread_mutex_init(&(glob->mutex), NULL);
    return glob;
}

void destroy_data(struct data *glob){
    free(glob);
}

bool is_full(struct sql *seq_list){   // 传顺序表的地址比较高效
    return seq_list->last == SQL_SIZE-1;
}

void thread_cancel_hand(void *arg){

    pthread_mutex_unlock((pthread_mutex_t *)arg);
}


int insert_char_to_seqlist(struct data *glob, char ch){
    pthread_cleanup_push(thread_cancel_hand,&(glob->mutex)); // 压栈 
    pthread_mutex_lock(&(glob->mutex));
    if(is_full(&(glob->seq_list))) {
        fprintf(stderr, "LIST FULL!!!\n");
        pthread_mutex_unlock(&(glob->mutex));
        return -1;
    }
    glob->seq_list.last++;
    glob->seq_list.array[glob->seq_list.last] = ch;
    pthread_mutex_unlock(&(glob->mutex));
    pthread_cleanup_pop(0);

    return 0;
}

void *thread(void *arg){
    char ch;
    FILE *fp;
    struct data *glob = arg;
    fp = fopen("a.txt", "r"); 
    if(fp == NULL){
        perror("打开文件失败\n");
        goto thread_exit; 
    }
    while(1){
        //读取文件的顺序写入顺序表
        sleep(1);
        ch = fgetc(fp);
        if(ch == EOF){
            if(feof(fp)){
                printf("文件读取完毕\n");
                break;
            }
            else if(ferror(fp)){
                printf("文件读取出错\n");
                goto opt_file_err;
            }
        }
        insert_char_to_seqlist(glob, ch);
    }    
    fclose(fp);
    return NULL;
opt_file_err:
    fclose(fp);
thread_exit:
    pthread_exit(NULL);

}

void display_seqlist_data(struct sql *seq_list){
    int i;
    printf("seq list has data:\n");
    for(i = 0; i < seq_list->last; i++){
        printf("array[%d]=%c\n",i,seq_list->array[i]);
    }
    printf("----------------------------\n");
}   


int main(void)
{
    char ch;  // 变量从小到大摆放
    pthread_t tid;
    struct data *glob;

    glob = request_data_and_init();  // 申请结构体内存， 并初始化里面的数据

    pthread_create(&tid, NULL, thread, glob);//创建子线程区读取文件内容


    while(1){
        ch = getchar();//从键盘获取字符
        if(ch == '\n')//如果\n继续获取
            continue;
        if(ch == 'p'){
            display_seqlist_data(&(glob->seq_list));
        }
        
        //写入顺序表
       insert_char_to_seqlist(glob, ch);
    }
    pthread_join(tid, NULL);// 等待子线程退出
    destroy_data(glob);
    return 0;
}
