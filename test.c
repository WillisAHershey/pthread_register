#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>
#include <signal.h>
#include "pthread_store.h"

#define NUM_THREADS 10
#define SIGNAL SIGUSR1

void sigtermHandler(int sig){
  int *array=pthread_discard();
  if(!array)
	printf("Error on thread with tid:%lu. NULL returned from deregister call\n",(unsigned long)pthread_self());
  else
	free(array);
  pthread_exit(NULL);
}

void* threadRun(void *data){
  sem_t *mom=(sem_t*)data;
  int *array=malloc(sizeof(int)*300);
  if(!array)
	return NULL;
  if(pthread_store(array))
	printf("Registration error on thread with tid:%lu. Wrong value returned from register call\n",(unsigned long)pthread_self());
  signal(SIGNAL,sigtermHandler);
  sem_post(mom);
  for(int c=0;c<300;++c)
	array[c]=0;
  while(1){
	//do busy work until signal is recieved
	for(int c=0;c<300;++c)
		if(array[c==0?299:c-1]%2)
			++array[c];
		else
			--array[c];
  }
}

int main(){
  pthread_t tids[NUM_THREADS];
  sem_t mom;
  sem_init(&mom,0,0);
  for(int c=0;c<NUM_THREADS;++c)
	pthread_create(&tids[c],NULL,threadRun,&mom);
  for(int c=0;c<NUM_THREADS;++c)
	sem_wait(&mom);
  sem_destroy(&mom);
  for(int c=0;c<NUM_THREADS;++c)
	pthread_kill(tids[c],SIGNAL);
  int err;
  for(int c=0;c<NUM_THREADS;++c)
	if((err=pthread_join(tids[c],NULL)))
		printf("Failure: %d\n",err);
  printf("Parent thread has joined all threads\n");
  printf("%d\n",pthread_store_close());
  return 0;
}
