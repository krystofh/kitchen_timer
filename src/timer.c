#include "timer.h"

#define TIMER_STACK 1024
#define TIMER_THREAD_PRIORITY 4

LOG_MODULE_REGISTER(timer, CONFIG_LOG_DEFAULT_LEVEL); // Registers the log level for the module

static struct k_work_delayable timer_work;
static struct k_work_delayable alarm_work;

// Time logic
timevar_t set_time = {0, 0};
timevar_t current_time = {0, 0};
timer_state current_state = SLEEPING;

// State getters setter
void set_state(timer_state state)
{
    current_state = state;
}

timer_state get_state()
{
    return current_state;
}

// Time functions
void inc_minutes()
{
    LOG_INF("INC minutes called: %d:%d -> +1", set_time.minutes, set_time.seconds);
    if (set_time.minutes == 99)
    {
        LOG_ERR("Timer can't increase minutes count more than 99!");
    }
    else
    {
        set_time.minutes += 1;
    }
    display_time(&set_time);
}

void dec_minutes()
{
    LOG_INF("DEC minutes called: %d:%d -> -1", set_time.minutes, set_time.seconds);
    if (set_time.minutes == 0)
    {
        LOG_ERR("Timer reached 0 minutes. Can't decrease further!");
    }
    else
    {
        set_time.minutes -= 1;
    }
    display_time(&set_time);
}

void inc_seconds()
{
    LOG_INF("INC seconds called: %d:%d -> +1", set_time.minutes, set_time.seconds);
    if (set_time.seconds == 59)
    {
        inc_minutes();
        set_time.seconds = 0;
    }
    else
    {
        set_time.seconds += 1;
    }
    display_time(&set_time);
}

void dec_seconds()
{
    LOG_INF("DEC seconds called: %d:%d -> -1", set_time.minutes, set_time.seconds);
    if (set_time.seconds > 0)
    {
        set_time.seconds -= 1;
    }
    else // XX:00 set
    {
        if (set_time.minutes > 0)
        { // 01:00 -> 00:59
            dec_minutes();
            set_time.seconds = 59;
        }
        else
        { // 00:00
            LOG_ERR("Timer reached 00:00, can't decrease!");
            if (current_state == COUNTDOWN)
            {
                current_state = ALARM;
                stop_timer();
                sound_alarm(true);
            }
        }
    }
    display_time(&set_time);
}

void sound_alarm(bool forever)
{
    if (forever)
    {
        if (current_state == ALARM) // sound alarm until a button is pressed
        {
            k_work_init_delayable(&alarm_work, alarm_work_handler); // start alarm scheduling
            k_work_schedule(&alarm_work, K_NO_WAIT);
        }
    }
    else
    {
        play_sound(ALARM_SOUND, 3); // play alarm few times and go to sleep
        current_state = SLEEPING;
    }
}

void stop_alarm()
{
    current_state = SLEEPING;
}

void reset_time()
{
    set_time.minutes = 0;
    set_time.seconds = 0;
    display_time(&set_time);
    current_state = SLEEPING; // TODO check if SET_SECONDS is not better
    LOG_INF("Reset performed");
    LOG_INF("Current state: %d", current_state);
    play_sound(RESET_SOUND, 1);
}

// Timer countdown control
void run_timer()
{
    current_state = COUNTDOWN;
    LOG_INF("Countdown started");
    k_work_init_delayable(&timer_work, update_timer); // start timer scheduling
    k_work_schedule(&timer_work, K_MSEC(1000));
    play_sound(CONFIRM_SOUND, 1);
}

void stop_timer()
{
    if (current_state != ALARM)
    {
        play_sound(STOP_SOUND, 1); // do not play when alarm shall play now
        current_state = SLEEPING;  // put to sleep only if not alarm sounding
    }
    k_work_cancel_delayable(&timer_work);
    LOG_INF("Countdown stopped");
}

// Work function that is rescheduled periodically
void update_timer(struct k_work *work)
{
    if (current_state == COUNTDOWN)
    {
        dec_seconds();
        k_work_reschedule(&timer_work, K_MSEC(1000));
    }
}

// work handler to play alarm sound at regular intervals
void alarm_work_handler(struct k_work *work)
{
    if (current_state == ALARM)
    {
        play_sound(ALARM_SOUND, 1);                   // Play the alarm sound
        k_work_reschedule(&alarm_work, K_MSEC(1000)); // Reschedule for 1 second later, must be larger than 1 alarm sound
    }
}

// Shell commands (mainly for development)
void cmd_reset_time(const struct shell *sh, size_t argc, char **argv)
{
    reset_time();
}

void cmd_start_countdown(const struct shell *sh, size_t argc, char **argv)
{
    run_timer();
}

void cmd_stop_countdown(const struct shell *sh, size_t argc, char **argv)
{
    stop_timer();
}

SHELL_CMD_REGISTER(reset, NULL, "Reset timer", cmd_reset_time);
SHELL_CMD_REGISTER(start, NULL, "Starts the timer (countdown)", cmd_start_countdown);
SHELL_CMD_REGISTER(stop, NULL, "Stops the timer (countdown)", cmd_stop_countdown);
