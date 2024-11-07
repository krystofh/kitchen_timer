#ifndef TIMER_H
#define TIMER_H

#include <zephyr/kernel.h>
#include <zephyr/shell/shell.h>
#include <zephyr/logging/log.h>
#include <zephyr/shell/shell.h>

#include <stdlib.h>

#include "sound_player.h"

// Time
typedef struct
{
    uint8_t minutes;
    uint8_t seconds;
} timevar_t;

typedef enum
{
    SLEEPING,
    SET_SECONDS,
    SET_MINUTES,
    COUNTDOWN,
    ALARM
} timer_state;

extern timer_state current_state;

// State functions
void set_state(timer_state state);
timer_state get_state(void);
// Time functions
void inc_minutes(void);
void dec_minutes(void);
void inc_seconds(void);
void dec_seconds(void);
void reset_time(void);

// Timer (countdown) control
void run_timer(void);
void stop_timer(void);
void update_timer(struct k_work *work);

// Shell commands
void cmd_reset_time(const struct shell *sh, size_t argc, char **argv);
void cmd_start_countdown(const struct shell *sh, size_t argc, char **argv);
#endif // TIMER_H