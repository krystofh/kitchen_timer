#ifndef SOUND_PLAYER_H
#define SOUND_PLAYER_H

#include <stdlib.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/kernel.h>

typedef struct
{
    const int *notes;
    const int *note_durations;
    const int length; // number of notes
} Sound;

// Enum to identify each sound by name
typedef enum
{
    CLICK_SOUND,
    CONFIRM_SOUND,
    MODE_SOUND,
    ALARM_SOUND,
    RESET_SOUND
} SoundID;

// Play functions
void play_note(const struct pwm_dt_spec *device, int frequency, int duration_ms);
void play_tune(const struct pwm_dt_spec *device, int *notes, int *durations, int length);
void play_sound(const struct pwm_dt_spec *device, SoundID sound_id, uint16_t repetitions);

#endif