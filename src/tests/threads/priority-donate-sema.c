/* Low priority thread L acquires a lock, then blocks downing a
   semaphore.  Medium priority thread M then blocks waiting on
   the same semaphore.  Next, high priority thread H attempts to
   acquire the lock, donating its priority to L.

   Next, the main thread ups the semaphore, waking up L.  L
   releases the lock, which wakes up H.  H "up"s the semaphore,
   waking up M.  H terminates, then M, then L, and finally the
   main thread.

   Written by Godmar Back <gback@cs.vt.edu>. */

#include <stdio.h>
#include "tests/threads/tests.h"
#include "threads/init.h"
#include "threads/synch.h"
#include "threads/thread.h"

struct lock_and_sema 
  {
    struct lock lock;
    struct semaphore sema;
  };

static thread_func l_thread_func;
static thread_func m_thread_func;
static thread_func h_thread_func;

void
test_priority_donate_sema (void) 
{
  struct lock_and_sema ls;

  /* This test does not work with the MLFQS. */
  ASSERT (!thread_mlfqs);

  /* Make sure our priority is the default. */
  ASSERT (thread_get_priority () == PRI_DEFAULT);

  lock_init (&ls.lock);
  sema_init (&ls.sema, 0);
  //（1）low 线程优先级比 main（31) 高,优先执行
  thread_create ("low", PRI_DEFAULT + 1, l_thread_func, &ls);
  //（4）med 线程优先级比 main（32) 高,优先执行
  thread_create ("med", PRI_DEFAULT + 3, m_thread_func, &ls);
  //（6）high 线程优先级比 main（34) 高,优先执行
  thread_create ("high", PRI_DEFAULT + 5, h_thread_func, &ls);
  //（8）释放ls.sema，此时 main的优先级改变为31。等待队列中的Thread L（36）优先被调度。
  sema_up (&ls.sema);
  //（12）main 最后结束
  msg ("Main thread finished.");
}

static void
l_thread_func (void *ls_)
{
  struct lock_and_sema *ls = ls_;
  //（2）Thread L 成功获取到 ls->lock ,继续执行。
  lock_acquire (&ls->lock);
  msg ("Thread L acquired lock.");
  //（3）Thread L 被ls->sema 阻塞，添加到 ls->sema 的等待队列中,同时将优先级捐赠给Main
  sema_down (&ls->sema);
  msg ("Thread L downed semaphore.");
  //（9）释放了ls-lock，Thread L的优先级变为32，被 Thread H 抢占调度。
  lock_release (&ls->lock);
  //（12）Thread L 比 main高，优先调度。
  msg ("Thread L finished.");
}

static void
m_thread_func (void *ls_) 
{
  struct lock_and_sema *ls = ls_;
  //（5）Thread M 被ls->sema 阻塞，添加到 ls->sema 的等待队列中,同时将优先级捐赠给Main
  sema_down (&ls->sema);
  //（11）Thread M 比 L 和 main 高，优先调度。
  msg ("Thread M finished.");
}

static void
h_thread_func (void *ls_) 
{
  struct lock_and_sema *ls = ls_;
  //（7）Thread H 获取到 ls->lock 失败，添加到ls->lock的等待队列，同时将优先级捐赠给 Thread L。
  lock_acquire (&ls->lock);
  msg ("Thread H acquired lock.");
  //（10）释放ls->sema，Thread M 被唤醒。由于 Thread H 的优先级比 main、L、M 都高，所以继续执行。
  sema_up (&ls->sema);
  lock_release (&ls->lock);
  msg ("Thread H finished.");
}
