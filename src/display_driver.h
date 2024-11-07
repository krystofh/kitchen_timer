#ifndef DISPLAY_DRIVER_H
#define DISPLAY_DRIVER_H

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/shell/shell.h>
#include <zephyr/logging/log.h>
#include <stdlib.h>
#include "timer.h"

#define DIGIT_DISPLAY_TIME_MS 5 // Time to display each digit

extern const struct gpio_dt_spec digit_1;
extern const struct gpio_dt_spec digit_2;
extern const struct gpio_dt_spec digit_3;
extern const struct gpio_dt_spec digit_4;

// Segment GPIO configuration (active low)
extern const struct gpio_dt_spec seg_a;
extern const struct gpio_dt_spec seg_b;
extern const struct gpio_dt_spec seg_c;
extern const struct gpio_dt_spec seg_d;
extern const struct gpio_dt_spec seg_e;
extern const struct gpio_dt_spec seg_f;
extern const struct gpio_dt_spec seg_g;
extern const struct gpio_dt_spec seg_dp;

// Digits and segments
extern const struct gpio_dt_spec *digits[];
extern const struct gpio_dt_spec *segments[];
extern uint8_t display_buffer[];

// Functions
int init_display(void);
void display_refresh_thread(void);
int display_demo(void);
void display_digit(uint8_t number, uint8_t index);
void set_display_value(uint16_t value);
void display_number(uint16_t number);
void display_time(timevar_t *displayed_time);
void reset_segments(void);
void reset_display(void);

#endif // DISPLAY_DRIVER_H