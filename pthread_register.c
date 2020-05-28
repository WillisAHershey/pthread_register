#include <stdlib.h>
#include <pthread.h>
#include "pthread_register.h"

void* pthread_register(int which,void *reg){
  static pthread_mutex_t mux=PTHREAD_MUTEX_INITIALIZER;
  static reglist *head=NULL;
  pthread_t self=pthread_self();
  pthread_mutex_lock(&mux);
  void *out=NULL;
  switch(which){
    case REGISTER:
      reglist *pt=malloc(sizeof(reglist));
      if(!pt)
        break;
      *pt=(reglist){.next=head,.reg=reg,.tid=self};
      head=pt;
      out=reg;
      break;
    case RECALL:
      for(reglist *pt=head;pt;pt=pt->next)
        if(pthread_equal(pt->tid,self)){
          out=pt->reg;
          break;
        }
      break;
    case DEREGISTER:
      if(!head)
        break;
      if(pthread_equal(head->tid,self)){
        reglist *pt=head;
        head=pt->next;
        out=pt->reg;
        free(pt);
        break;
      }
      for(reglist *b=head,*r=head->next;r;b=r,r=r->next)
        if(pthread_equal(r->tid,self)){
          b->next=r->next;
          out=r->reg;
          free(r);
          break;
        }
      break;
  }
  pthread_mutex_unlock(&mux);
  return out;
}
