/* Verifies that lowering a thread's priority so that it is no
   longer the highest-priority thread in the system causes it to
   yield immediately. */

#include <stdio.h>
#include "tests/threads/tests.h"
#include "threads/init.h"
#include "threads/thread.h"

static thread_func changing_thread;

void
test_priority_change (void) 
{
  /* This test does not work with the MLFQS. */
  ASSERT (!thread_mlfqs);
  msg ("Creating a high-priority thread 2.");
  //（1）thread 2 优先级比 main 高，抢占式调度。
  thread_create ("thread 2", PRI_DEFAULT + 1, changing_thread, NULL);
  msg ("Thread 2 should have just lowered its priority.");
  //（3）main 线程优先级比 thread 2 低，让出时间片
  thread_set_priority (PRI_DEFAULT - 2);
  msg ("Thread 2 should have just exited.");
  //（6）main 线程执行完毕。
}

static void
changing_thread (void *aux UNUSED) 
{
  msg ("Thread 2 now lowering priority.");
  //（2）Thread 2 此时优先级比main线程低，让出时间片。
  thread_set_priority (PRI_DEFAULT - 1);
  //（4）Thread 2 执行完毕 ，轮到main 执行
  msg ("Thread 2 exiting.");
}
