#ifndef PTHREAD_REGISTER
#define PTHREAD_REGISTER

#define REGISTER 0
#define RECALL 1
#define DEREGISTER 2

typedef struct reglistStruct{
  struct reglistStruct *next;
  void *reg;
  pthread_t tid;
}reglist;

void* pthread_register(int,void*);

#endif
