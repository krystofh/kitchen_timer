#ifndef DISPLAY_DRIVER_H
#define DISPLAY_DRIVER_H

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/shell/shell.h>
#include <zephyr/logging/log.h>
#include <stdlib.h>

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

// Buffers
extern const struct gpio_dt_spec *digit_buffer[];
extern const struct gpio_dt_spec *segment_buffer[];

// Functions
int init_display(void);
int display_demo(void);
void display_digit(uint8_t number, uint8_t index);

void reset_digit(uint8_t index);
void reset_display(void);

#endif // DISPLAY_DRIVER_H