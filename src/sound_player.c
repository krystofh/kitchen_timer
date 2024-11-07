#include "sound_player.h"

// #define SOUND_STACK 1024
// #define SOUND_THREAD_PRIORITY 7 // low priority

// Speaker (PWM device) used globally
static const struct pwm_dt_spec *speaker;

// Sounds
const int click_notes[] = {880, 0};
const int click_durations[] = {100, 50};
const int confirm_notes[] = {440, 880};
const int confirm_durations[] = {100, 100};
const int mode_notes[] = {440, 0};
const int mode_durations[] = {100, 50};
const int alarm_notes[] = {880, 0, 880, 0, 880, 0};
const int alarm_durations[] = {200, 200, 200, 200, 200, 200};
const int reset_notes[] = {880, 440};
const int reset_durations[] = {100, 100};

// Spawn thread for sound output
// K_THREAD_DEFINE(sound_player_tid, SOUND_STACK, sound_output_th,
//                 NULL, NULL, NULL, SOUND_THREAD_PRIORITY, 0, 0);

// Define Sound structs for each sound
static const Sound sounds[] = {
    {click_notes, click_durations, sizeof(click_notes) / sizeof(click_notes[0])}, // SOUND_CLICK
    {confirm_notes, confirm_durations, sizeof(confirm_notes) / sizeof(confirm_durations[0])},
    {mode_notes, mode_durations, sizeof(mode_notes) / sizeof(mode_notes[0])},
    {alarm_notes, alarm_durations, sizeof(alarm_notes) / sizeof(alarm_notes[0])}, // SOUND_ALARM
    {reset_notes, reset_durations, sizeof(reset_notes) / sizeof(reset_notes[0])}  // SOUND_RESET
};

// void sound_output_th()
// {
//     while (1)
//     {
//     }
// }

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
    Sound *sound = &sounds[sound_id]; // init a pointer to the sound
    uint16_t counter = 0;
    while (counter < repetitions)
    {
        play_tune(sound->notes, sound->note_durations, sound->length);
        ++counter;
    }
}