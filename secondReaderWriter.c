#include<stdio.h>
#include<semaphore.h>
#include<pthread.h>

sem_t rd,wrt;
int readCount=0,writeCount=0,cnt=1;
pthread_mutex_t mutex1, mutex2;

void  writer(void *wno){
    pthread_mutex_lock(&mutex2);
    writeCount++;
    if(writeCount==1){
        sem_wait(&rd);
    }
    pthread_mutex_unlock(&mutex2);
    sem_wait(&wrt);
    cnt=cnt*2;
     printf("Writer %d modified cnt to %d\n",(*((int *)wno)),cnt);
     sem_post(&wrt);
     pthread_mutex_lock(&mutex2);
    writeCount--;
    if(writeCount==0){
        sem_post(&rd);
    }
     pthread_mutex_unlock(&mutex2);

}

void *reader(void *rno){
    sem_wait(&rd);
    pthread_mutex_lock(&mutex1);
    readCount++;
    if(readCount==1){
        sem_wait(&wrt);
    }
    pthread_mutex_unlock(&mutex1);
    sem_post(&rd);
    printf("Reader %d: read cnt as %d\n",*((int *)rno),cnt);

    pthread_mutex_lock(&mutex1);
    readCount--;
    if(readCount==0){
        sem_post(&wrt);
    }
pthread_mutex_unlock(&mutex1);
    
}

int main()
{
    sem_init(&rd,0,1);
    sem_init(&wrt,0,1);
    pthread_mutex_init(&mutex1, NULL);
     pthread_mutex_init(&mutex2, NULL);
    pthread_t readerThread[10], writerThread[5];
 
  int a[10]={1,2,3,4,5, 6,7,8,9,10};

     for(int i=0;i<5;i++){
        pthread_create(&writerThread[i],NULL, (void *)writer, (void *)&a[i]);
    }
    for(int i=0;i<10;i++){
        pthread_create(&readerThread[i],NULL, (void *)reader, (void *)&a[i]);
    }
   
   for(int i = 0; i < 5; i++) {
        pthread_join(writerThread[i], NULL);
    }
    
        for(int i = 0; i < 10; i++) {
        pthread_join(readerThread[i], NULL);
    }
    

    pthread_mutex_destroy(&mutex1);
    pthread_mutex_destroy(&mutex2);
    sem_destroy(&rd);
    sem_destroy(&wrt);
 return 0;
}