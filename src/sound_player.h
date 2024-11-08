#ifndef SOUND_PLAYER_H
#define SOUND_PLAYER_H

#include <stdlib.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

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
    RESET_SOUND,
    STOP_SOUND
} SoundID;

typedef struct
{
    SoundID sound;
} SoundEvent;

void sound_processing_thread(void);
void init_sound(const struct pwm_dt_spec *pwm_device);
// Play functions
void play_note(int frequency, int duration_ms);
void play_tune(int *notes, int *durations, int length);
void play_sound(SoundID sound_id, uint16_t repetitions);

#endif