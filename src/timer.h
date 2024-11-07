#ifndef TIMER_H
#define TIMER_H

#include <zephyr/kernel.h>
#include <zephyr/shell/shell.h>
#include <zephyr/logging/log.h>
#include <stdlib.h>

void update_timer(struct k_work *work);
void timer_thread(void);
#endif // TIMER_H