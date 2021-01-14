// 利用读写锁 + 信号量
// 主线程：每隔1s从a.txt读取一行放到buffer中
// 线程1： 负责打印buffer中的数据
// 线程2： 负责从buffer中读数据到b.txt文件中

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

// 信号量
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>


pthread_rwlock_t rwlock;
sem_t *sem;


// 防止死锁：线程退出之前，先做压栈

void thread_cancel_hand(void *arg){
    pthread_rwlock_unlock(&rwlock);
}


void *show_buffer_thread(void *arg){
    char *buffer = arg;
    while(1){
        sleep(1);
        pthread_cleanup_push(thread_cancel_hand, NULL); // 压栈
        pthread_rwlock_rdlock(&rwlock);
        printf("buffer = %s\n", buffer);
        pthread_rwlock_unlock(&rwlock);
        pthread_cleanup_pop(0);  // 出栈
    }
    return NULL;
}

void *copy_thread(void *arg){
    // 写入数据到b.txt
    FILE *fp;
    char *retstr;
    char *buffer = arg;
    
    fp = fopen("b.txt","w");
    
    if(fp==NULL){
        perror("open a.txt error\n" );
        return  NULL;
    }
    
    while(1){
        // 信号量
        sem_wait(sem);// 信号量-1
        
        pthread_cleanup_push(thread_cancel_hand, NULL); // 压栈
        pthread_rwlock_rdlock(&rwlock);
        //if(fputs(buffer, fp)<0){
         //   printf("write to b.txt error\n");
       // }
        int ret = fputs(buffer, fp);
        printf("run here ret = %d\n", ret);
        pthread_rwlock_unlock(&rwlock);
        pthread_cleanup_pop(0);
    }
    fclose(fp);
    return NULL;
}

int main(void)
{
    FILE *fp;
    char *buffer;
    char *retstr;
    pthread_t tid1, tid2;
    
    sem = sem_open("mysem", O_CREAT, 0644, 0); // 打开信号量
    
    fp = fopen("a.txt", "r");
    if(fp == NULL){
        perror("open a.txt error\n");
        return -1;
    }

    pthread_rwlock_init(&rwlock,NULL);
        
    buffer = malloc(256);

    pthread_create(&tid1, NULL, show_buffer_thread, buffer);
    pthread_create(&tid1, NULL, copy_thread, buffer);

    while(1){
        sleep(1);
        
        pthread_rwlock_wrlock(&rwlock);

        // 读取a.txt文件内容到buffer中
        retstr =  fgets(buffer, 256, fp);
        if(retstr == NULL){
            if(feof(fp)){
                printf("read to file end\n");   // 读到文件末尾
                pthread_rwlock_unlock(&rwlock); // 解锁
                break;
            }
            else if(ferror(fp)){
                perror("read file error\n");
                break;
            }
        }
        pthread_rwlock_unlock(&rwlock);
        sem_post(sem);  // 信号量+1
    }

    pthread_cancel(tid1);
    pthread_cancel(tid2);
    
    free(buffer);
    
    pthread_rwlock_destroy(&rwlock);
    sem_close(sem);

    fclose(fp);
    return 0;
}

