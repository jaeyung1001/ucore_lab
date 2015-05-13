#include <stdio.h>
#include <proc.h>
#include <sem.h>
#include <monitor.h>
#include <assert.h>

#define N 5 /* 꿨若뜻빊*/
#define LEFT (i-1+N)%N /* i꾢랩삣뤇*/
#define RIGHT (i+1)%N /* i꾢뤂삣뤇*/
#define THINKING 0 /* 꿨若뜻ⓩ앲*/
#define HUNGRY 1 /* 꿨若뜻꺍뽩풓됧춴 */
#define EATING 2 /* 꿨若뜻ⓨ릡*/
#define TIMES  4 /* 轝↓ */
#define SLEEP_TIME 10

//---------- philosophers problem using semaphore ----------------------
int state_sema[N]; /* 溫겼퐬驪뤶릉雅븀듁곭쉪곁퍍 */
/* 岳▼뤇뤸삸訝訝ょ돶餘딁쉪닷엹섌뇧 */
semaphore_t mutex; /* 訝당븣뷰틨*/
semaphore_t s[N]; /* 驪뤶릉꿨若뜸訝や에룬뇧 */

struct proc_struct *philosopher_proc_sema[N];

void phi_test_sema(i) /* i竊싧벒耶루쟻餓캮-1 */
{ 
    if(state_sema[i]==HUNGRY&&state_sema[LEFT]!=EATING
            &&state_sema[RIGHT]!=EATING)
    {
        state_sema[i]=EATING;
        up(&s[i]);
    }
}

void phi_take_forks_sema(int i) /* i竊싧벒耶루쟻餓캮-1 */
{ 
        down(&mutex); /* 瓦쎾뀯訝당븣*/
        state_sema[i]=HUNGRY; /* 溫겼퐬訝뗥벒耶i耀ι꾡틟若*/
        phi_test_sema(i); /* 瑥뺝쎗孃쀥댆訝ㅵ룵됧춴 */
        up(&mutex); /* 獵삣訝당븣*/
        down(&s[i]); /* 倻귝옖孃쀤툖겼룊耶먨갚삣줊 */
}

void phi_put_forks_sema(int i) /* i竊싧벒耶루쟻餓캮-1 */
{ 
        down(&mutex); /* 瓦쎾뀯訝당븣*/
        state_sema[i]=THINKING; /* 꿨若띈퓵繞먪퍜*/
        phi_test_sema(LEFT); /* 뗤訝뗥랩삣콉겼쑉맔썼퓵繞*/
        phi_test_sema(RIGHT); /* 뗤訝뗥뤂삣콉겼쑉맔썼퓵繞*/
        up(&mutex); /* 獵삣訝당븣*/
}

int philosopher_using_semaphore(void * arg) /* i竊싧벒耶루쟻竊뚥퍗0캮-1 */
{
    int i, iter=0;
    i=(int)arg;
    cprintf("I am No.%d philosopher_sema\n",i);
    while(iter++<TIMES)
    { /* 좈솏孃ょ렞 */
        cprintf("Iter %d, No.%d philosopher_sema is thinking\n",iter,i); /* 꿨若뜻ⓩ앲*/
        do_sleep(SLEEP_TIME);
        phi_take_forks_sema(i); 
        /* 誤곦륵ゅ룊耶먲펽뽬낂샍櫻*/
        cprintf("Iter %d, No.%d philosopher_sema is eating\n",iter,i); /* 瓦쏃쨶 */
        do_sleep(SLEEP_TIME);
        phi_put_forks_sema(i); 
        /* 듾륵듿룊耶먨릪뜻붂욄죱耶*/
    }
    cprintf("No.%d philosopher_sema quit\n",i);
    return 0;    
}

//-----------------philosopher problem using monitor ------------
/*PSEUDO CODE :philosopher problem using monitor
 * monitor dp
 * {
 *  enum {thinking, hungry, eating} state[5];
 *  condition self[5];
 *
 *  void pickup(int i) {
 *      state[i] = hungry;
 *      if ((state[(i+4)%5] != eating) && (state[(i+1)%5] != eating)) {
 *        state[i] = eating;
 *      else
 *         self[i].wait();
 *   }
 *
 *   void putdown(int i) {
 *      state[i] = thinking;
 *      if ((state[(i+4)%5] == hungry) && (state[(i+3)%5] != eating)) {
 *          state[(i+4)%5] = eating;
 *          self[(i+4)%5].signal();
 *      }
 *      if ((state[(i+1)%5] == hungry) && (state[(i+2)%5] != eating)) {
 *          state[(i+1)%5] = eating;
 *          self[(i+1)%5].signal();
 *      }
 *   }
 *
 *   void init() {
 *      for (int i = 0; i < 5; i++)
 *         state[i] = thinking;
 *   }
 * }
 */

struct proc_struct *philosopher_proc_condvar[N]; // N philosopher
int state_condvar[N];                            // the philosopher's state: EATING, HUNGARY, THINKING  
monitor_t mt, *mtp=&mt;                                    // mp is mutex semaphore for monitor's procedures

void phi_test_condvar (i) { 
    if(state_condvar[i]==HUNGRY&&state_condvar[LEFT]!=EATING
            &&state_condvar[RIGHT]!=EATING) {
        cprintf("phi_test_condvar: state_condvar[%d] will eating\n",i);
        state_condvar[i] = EATING ;
        cprintf("phi_test_condvar: signal self_cv[%d] \n",i);
        cond_signal(&mtp->cv[i]) ;
    }
}


void phi_take_forks_condvar(int i) {
     down(&(mtp->mutex));
//--------into routine in monitor--------------
     // LAB7 EXERCISE1: 2012080061 李载隆
     // I am hungry
     // try to get fork
      // I am hungry
      state_condvar[i]=HUNGRY; 
      // try to get fork
      phi_test_condvar(i); 
      while (state_condvar[i] != EATING) {
          cprintf("phi_take_forks_condvar: %d didn't get fork and will wait\n",i);
          cond_wait(&mtp->cv[i]);
      }
//--------leave routine in monitor--------------
      if(mtp->next_count>0)
         up(&(mtp->next));
      else
         up(&(mtp->mutex));
}

void phi_put_forks_condvar(int i) {
     down(&(mtp->mutex));

//--------into routine in monitor--------------
     // LAB7 EXERCISE1: 2012080061 李载隆
     // I ate over
     // test left and right neighbors
      // I ate over 
      state_condvar[i]=THINKING;
      // test left and right neighbors
      phi_test_condvar(LEFT);
      phi_test_condvar(RIGHT);
//--------leave routine in monitor--------------
     if(mtp->next_count>0)
        up(&(mtp->next));
     else
        up(&(mtp->mutex));
}

//---------- philosophers using monitor (condition variable) ----------------------
int philosopher_using_condvar(void * arg) { /* arg is the No. of philosopher 0~N-1*/
  
    int i, iter=0;
    i=(int)arg;
    cprintf("I am No.%d philosopher_condvar\n",i);
    while(iter++<TIMES)
    { /* iterate*/
        cprintf("Iter %d, No.%d philosopher_condvar is thinking\n",iter,i); /* thinking*/
        do_sleep(SLEEP_TIME);
        phi_take_forks_condvar(i); 
        /* need two forks, maybe blocked */
        cprintf("Iter %d, No.%d philosopher_condvar is eating\n",iter,i); /* eating*/
        do_sleep(SLEEP_TIME);
        phi_put_forks_condvar(i); 
        /* return two forks back*/
    }
    cprintf("No.%d philosopher_condvar quit\n",i);
    return 0;    
}

void check_sync(void){

    int i;

    //check semaphore
    sem_init(&mutex, 1);
    for(i=0;i<N;i++){
        sem_init(&s[i], 0);
        int pid = kernel_thread(philosopher_using_semaphore, (void *)i, 0);
        if (pid <= 0) {
            panic("create No.%d philosopher_using_semaphore failed.\n");
        }
        philosopher_proc_sema[i] = find_proc(pid);
        set_proc_name(philosopher_proc_sema[i], "philosopher_sema_proc");
    }

    //check condition variable
    monitor_init(&mt, N);
    for(i=0;i<N;i++){
        state_condvar[i]=THINKING;
        int pid = kernel_thread(philosopher_using_condvar, (void *)i, 0);
        if (pid <= 0) {
            panic("create No.%d philosopher_using_condvar failed.\n");
        }
        philosopher_proc_condvar[i] = find_proc(pid);
        set_proc_name(philosopher_proc_condvar[i], "philosopher_condvar_proc");
    }
}
