#include "sound_player.h"

#define SOUND_STACK 1024
#define SOUND_THREAD_PRIORITY 7 // low priority

LOG_MODULE_REGISTER(sound, CONFIG_LOG_DEFAULT_LEVEL); // Registers the log level for the module

// Speaker (PWM device) used globally
static const struct pwm_dt_spec *speaker;

// Sounds
const int click_notes[] = {880, 0};
const int click_durations[] = {100, 50};
const int confirm_notes[] = {440, 880};
const int confirm_durations[] = {100, 100};
const int mode_notes[] = {588, 0};
const int mode_durations[] = {100, 50};
const int alarm_notes[] = {880, 0, 880, 0, 880, 0};
const int alarm_durations[] = {200, 200, 200, 200, 200, 200};
const int reset_notes[] = {880, 440};
const int reset_durations[] = {100, 100};
const int stop_notes[] = {330, 0};
const int stop_durations[] = {100, 50};

// Define message queue for handling sound events
K_MSGQ_DEFINE(sound_queue, 1, 1, 1);

// Spawn thread for sound output
K_THREAD_DEFINE(sound_player_tid, SOUND_STACK, sound_processing_thread,
                NULL, NULL, NULL, SOUND_THREAD_PRIORITY, 0, 0);

// Define Sound structs for each sound
static const Sound sounds[] = {
    {click_notes, click_durations, sizeof(click_notes) / sizeof(click_notes[0])}, // SOUND_CLICK
    {confirm_notes, confirm_durations, sizeof(confirm_notes) / sizeof(confirm_durations[0])},
    {mode_notes, mode_durations, sizeof(mode_notes) / sizeof(mode_notes[0])},
    {alarm_notes, alarm_durations, sizeof(alarm_notes) / sizeof(alarm_notes[0])}, // SOUND_ALARM
    {reset_notes, reset_durations, sizeof(reset_notes) / sizeof(reset_notes[0])}, // SOUND_RESET
    {stop_notes, stop_durations, sizeof(stop_notes) / sizeof(stop_notes[0])},
};

void sound_processing_thread()
{
    SoundEvent sound_event;
    int ret_code;
    while (1)
    {
        ret_code = k_msgq_get(&sound_queue, &sound_event, K_FOREVER); // receive the sound message
        if (!ret_code)
        {
            Sound *sound = &sounds[sound_event.sound]; // init a pointer to the sound
            // Play sound
            play_tune(sound->notes, sound->note_durations, sound->length);
        }
        else
        {
            LOG_ERR("Error playing sound");
        }
        k_sleep(K_MSEC(100));
    }
}

void init_sound(const struct pwm_dt_spec *pwm_device)
{
    speaker = pwm_device;
    play_sound(CONFIRM_SOUND, 1);
}

void play_note(int frequency, int duration_ms)
{
    if (!frequency)
    {
        pwm_set_pulse_dt(speaker, 0); // turn off note
        k_sleep(K_MSEC(duration_ms));
    }
    else
    {
        pwm_set_dt(speaker, PWM_HZ(frequency), PWM_HZ(frequency) / 2U);
        k_sleep(K_MSEC(duration_ms));
        pwm_set_pulse_dt(speaker, 0); // turn off note
    }
}

// Play function
void play_tune(int *notes, int *durations, int length)
{
    for (int i = 0; i < length; i++)
    {
        play_note(notes[i], durations[i]);
    }
}

// Play sound
void play_sound(SoundID sound_id, uint16_t repetitions)
{
    SoundEvent new_sound_event = {sound_id};
    uint16_t counter = 0;
    int return_code;
    while (counter < repetitions)
    {
        // put the sound to queue
        return_code = k_msgq_put(&sound_queue, &new_sound_event, K_NO_WAIT);
        if (return_code != 0)
        {
            LOG_ERR("Sound events MSGQ full or some other error!");
        }
        ++counter;
    }
}