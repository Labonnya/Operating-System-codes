#include<stdio.h>
#include<semaphore.h>
#include<pthread.h>

sem_t wrt;
int readcount=0,cnt=1;
pthread_mutex_t mutex;

void *writer(void *wno){
     sem_wait(&wrt);
     cnt=cnt*2;
      printf("Writer %d modified cnt to %d\n",(*((int *)wno)),cnt);
     sem_post(&wrt);
}

void *reader(void *rno){
    pthread_mutex_lock(&mutex);
    readcount++;
    if(readcount==1){
        sem_wait(&wrt);
    }
        pthread_mutex_unlock(&mutex);
        printf("readerThread %d: read cnt as %d\n",*((int *)rno),cnt);
    
    sem_post(&wrt);

    pthread_mutex_lock(&mutex);
    readcount--;

    if(readcount==0)
    {
        sem_post(&wrt);
    }
    pthread_mutex_unlock(&mutex);

}

int main()
{
    sem_init(&wrt,0,1);
    pthread_mutex_init(&mutex, NULL);

    pthread_t readerThread[10], writerThread[5];
 
  int a[10]={1,2,3,4,5, 6,7,8,9,10};

    for(int i=0;i<10;i++){
        pthread_create(&readerThread[i],NULL, (void *)reader, (void *)&a[i]);
    }
    for(int i=0;i<5;i++){
        pthread_create(&writerThread[i],NULL, (void *)writer, (void *)&a[i]);
    }
    
        for(int i = 0; i < 10; i++) {
        pthread_join(readerThread[i], NULL);
    }
    for(int i = 0; i < 5; i++) {
        pthread_join(writerThread[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&wrt);
 return 0;
}