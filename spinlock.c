#include <stdbool.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#define UNLOCKED 0
#define LOCKED 1

struct tas{
  int lock;

}
tas1;
int counter = 0;

/**
* @pram taslock, initialises the handle for the taslock
*/

void tas_init(struct tas* tas){
  (tas->lock)=0;

};
/**
* Locks, operation should be successful if the lock is valid.
* If the lock is invalid, the lock operation returns a non-zero integer
* tas lock that is currently in a locked state will keep threads waiting.
* @param taslock
* @return success
*/
int tas_lock(struct tas* t){
int expected=UNLOCKED;
if(t->lock==1 ||t->lock==0 ){
while(!__atomic_compare_exchange_n (&(t->lock),&expected,LOCKED,true,__ATOMIC_SEQ_CST,__ATOMIC_SEQ_CST)){
expected=UNLOCKED;
return 0;
}
}
else{
  return 1;
}
return 1;
};
/**
* Unlocks, operation should be successful is the lock is valid.
* If the lock is invalid, unlock operation returns a non-zero integer.
* @param taslock
* @return success
*/


int tas_unlock(struct tas* t){
  if(t->lock==1 ||t->lock==0 ){
  __atomic_store_n(&(t->lock),UNLOCKED, __ATOMIC_SEQ_CST);
  return 0;
}
else{
  return 1;
}
return 1;
};
/**
* Destroys the tas lock, puts it in an invalid state
* @param taslock
*/
int *print(int *num) {
tas_lock(&tas1);
    printf("thread:%d ", *num);
tas_unlock(&tas1);
    return NULL;
}
void tas_destroy(struct tas* t){
  t->lock=-1;

};
int main(int argc, char* argv[]){
struct tas* tas1 = (struct tas*)malloc(sizeof(struct tas));
  tas_init(tas1);

  int i, *val;
      pthread_t pt[10];
      for(i = 0; i < 10; i++) {
        tas_lock(tas1);
          counter++;
        tas_unlock(tas1);
          pthread_create(&pt[i], NULL, (void *(*)(void *))print, &counter);
          pthread_join(pt[i], (void **)&val);

      }


};
