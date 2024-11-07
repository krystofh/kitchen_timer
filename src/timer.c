#include "timer.h"

#define TIMER_STACK 1024
#define TIMER_THREAD_PRIORITY 4

LOG_MODULE_REGISTER(timer, CONFIG_LOG_DEFAULT_LEVEL); // Registers the log level for the module
// Spawn thread
K_THREAD_DEFINE(timer_tid, TIMER_STACK, timer_thread, NULL, NULL, NULL,
                TIMER_THREAD_PRIORITY, 0, 0);

static struct k_work_delayable timer_work;

void run_timer()
{
    k_work_init_delayable(&timer_work, update_timer); // start timer scheduling
    k_work_schedule(&timer_work, K_NO_WAIT);
}

void update_timer(struct k_work *work)
{
    k_work_reschedule(&timer_work, K_MSEC(1000));
    LOG_INF("Tick!");
}

void timer_thread()
{
    run_timer(); // starts the timer work
    while (1)
    {
        k_sleep(K_MSEC(1000));
    }
}