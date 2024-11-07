#include "sound_player.h"

// Sounds
const int click_notes[] = {880};
const int click_durations[] = {100};
const int confirm_notes[] = {550, 900};
const int confirm_durations[] = {200, 400};
const int mode_notes[] = {440};
const int mode_durations[] = {100};
const int alarm_notes[] = {770, 0, 770, 0, 770, 0};
const int alarm_durations[] = {200, 200, 200, 200, 200, 200};
const int reset_notes[] = {880, 330};
const int reset_durations[] = {150, 300};

// Define Sound structs for each sound
// SOUND_CLICK,
// SOUND_CONFIRM
// SOUND_MODE,
// SOUND_ALARM,
// SOUND_RESET
static const Sound sounds[] = {
    {click_notes, click_durations, sizeof(click_notes) / sizeof(click_notes[0])}, // SOUND_CLICK
    {confirm_notes, confirm_durations, sizeof(confirm_notes) / sizeof(confirm_durations[0])},
    {mode_notes, mode_durations, sizeof(mode_notes) / sizeof(mode_notes[0])},
    {alarm_notes, alarm_durations, sizeof(alarm_notes) / sizeof(alarm_notes[0])}, // SOUND_ALARM
    {reset_notes, reset_durations, sizeof(reset_notes) / sizeof(reset_notes[0])}  // SOUND_RESET
};

void play_note(const struct pwm_dt_spec *device, int frequency, int duration_ms)
{
    if (!frequency)
    {
        pwm_set_pulse_dt(device, 0); // turn off note
        k_sleep(K_MSEC(duration_ms));
    }
    else
    {
        pwm_set_dt(device, PWM_HZ(frequency), PWM_HZ(frequency) / 2U);
        k_sleep(K_MSEC(duration_ms));
        pwm_set_pulse_dt(device, 0); // turn off note
    }
}

// Play function
void play_tune(const struct pwm_dt_spec *device, int *notes, int *durations, int length)
{
    for (int i = 0; i < length; i++)
    {
        play_note(device, notes[i], durations[i]);
    }
}

// Play sound
void play_sound(const struct pwm_dt_spec *device, SoundID sound_id, uint16_t repetitions)
{
    Sound *sound = &sounds[sound_id]; // init a pointer to the sound
    uint16_t counter = 0;
    while (counter < repetitions)
    {
        play_tune(device, sound->notes, sound->note_durations, sound->length);
        ++counter;
    }
}