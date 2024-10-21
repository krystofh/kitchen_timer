#include "display_driver.h"

LOG_MODULE_REGISTER(display, CONFIG_DISPLAY_LOG_LEVEL);

// Digits GPIO configuration (active high)
const struct gpio_dt_spec digit_1 = GPIO_DT_SPEC_GET(DT_NODELABEL(d1), gpios);
const struct gpio_dt_spec digit_2 = GPIO_DT_SPEC_GET(DT_NODELABEL(d2), gpios);
const struct gpio_dt_spec digit_3 = GPIO_DT_SPEC_GET(DT_NODELABEL(d3), gpios);
const struct gpio_dt_spec digit_4 = GPIO_DT_SPEC_GET(DT_NODELABEL(d4), gpios);

// Segment GPIO configuration (active low)
const struct gpio_dt_spec seg_a = GPIO_DT_SPEC_GET(DT_NODELABEL(sega), gpios);
const struct gpio_dt_spec seg_b = GPIO_DT_SPEC_GET(DT_NODELABEL(segb), gpios);
const struct gpio_dt_spec seg_c = GPIO_DT_SPEC_GET(DT_NODELABEL(segc), gpios);
const struct gpio_dt_spec seg_d = GPIO_DT_SPEC_GET(DT_NODELABEL(segd), gpios);
const struct gpio_dt_spec seg_e = GPIO_DT_SPEC_GET(DT_NODELABEL(sege), gpios);
const struct gpio_dt_spec seg_f = GPIO_DT_SPEC_GET(DT_NODELABEL(segf), gpios);
const struct gpio_dt_spec seg_g = GPIO_DT_SPEC_GET(DT_NODELABEL(segg), gpios);
const struct gpio_dt_spec seg_dp = GPIO_DT_SPEC_GET(DT_NODELABEL(segdp), gpios);

// Buffers
const struct gpio_dt_spec *digit_buffer[] = {&digit_1, &digit_2, &digit_3, &digit_4};
const struct gpio_dt_spec *segment_buffer[] = {&seg_a, &seg_b, &seg_c, &seg_d, &seg_e, &seg_f, &seg_g, &seg_dp};

// Digit encoding
// 7-segment encoding for digits 0-9
const uint8_t digit_codes[10] = {
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111  // 9
};

// Initialize the display GPIOs
int init_display()
{
    int ret;

    // Configure digit GPIOs
    ret = gpio_pin_configure_dt(&digit_1, GPIO_OUTPUT_INACTIVE);
    if (ret != 0)
        return ret;

    ret = gpio_pin_configure_dt(&digit_2, GPIO_OUTPUT_INACTIVE);
    if (ret != 0)
        return ret;

    ret = gpio_pin_configure_dt(&digit_3, GPIO_OUTPUT_INACTIVE);
    if (ret != 0)
        return ret;

    ret = gpio_pin_configure_dt(&digit_4, GPIO_OUTPUT_INACTIVE);
    if (ret != 0)
        return ret;

    // Configure segment GPIOs
    ret = gpio_pin_configure_dt(&seg_a, GPIO_OUTPUT_INACTIVE);
    if (ret != 0)
        return ret;

    ret = gpio_pin_configure_dt(&seg_b, GPIO_OUTPUT_INACTIVE);
    if (ret != 0)
        return ret;

    ret = gpio_pin_configure_dt(&seg_c, GPIO_OUTPUT_INACTIVE);
    if (ret != 0)
        return ret;

    ret = gpio_pin_configure_dt(&seg_d, GPIO_OUTPUT_INACTIVE);
    if (ret != 0)
        return ret;

    ret = gpio_pin_configure_dt(&seg_e, GPIO_OUTPUT_INACTIVE);
    if (ret != 0)
        return ret;

    ret = gpio_pin_configure_dt(&seg_f, GPIO_OUTPUT_INACTIVE);
    if (ret != 0)
        return ret;

    ret = gpio_pin_configure_dt(&seg_g, GPIO_OUTPUT_INACTIVE);
    if (ret != 0)
        return ret;

    ret = gpio_pin_configure_dt(&seg_dp, GPIO_OUTPUT_INACTIVE);
    if (ret != 0)
        return ret;

    LOG_INF("Display initialized");
    return 0;
}

// Simple demo showing sequentially all segments on all digits
int display_demo()
{
    bool enable_shutdown = false;
    // Iterate through each digit in digit_buffer
    for (int i = 0; i < ARRAY_SIZE(digit_buffer); i++)
    {
        // Turn off all segemnts first
        for (int m = 0; m < ARRAY_SIZE(segment_buffer); m++)
        {
            gpio_pin_set_dt(segment_buffer[m], 0);
        }

        // Turn off all digits first to prevent ghosting
        for (int j = 0; j < ARRAY_SIZE(digit_buffer); j++)
        {
            gpio_pin_set_dt(digit_buffer[j], 0);
        }

        // Activate current digit
        int response = gpio_pin_set_dt(digit_buffer[i], 1);

        // Light up each segment (A to G, DP) sequentially
        response = gpio_pin_set_dt(&seg_a, 1); // try out segment A
        k_sleep(K_MSEC(1000));
        if (enable_shutdown)
        {
            gpio_pin_set_dt(&seg_a, 0);
        };
        response = gpio_pin_set_dt(&seg_b, 1); // try out segment
        k_sleep(K_MSEC(1000));
        if (enable_shutdown)
        {
            response = gpio_pin_set_dt(&seg_b, 0); // try out segment
        };
        response = gpio_pin_set_dt(&seg_c, 1); // try out segment
        k_sleep(K_MSEC(1000));
        if (enable_shutdown)
        {
            response = gpio_pin_set_dt(&seg_c, 0); // try out segment
        };
        response = gpio_pin_set_dt(&seg_d, 1); // try out segment
        k_sleep(K_MSEC(1000));
        if (enable_shutdown)
        {
            response = gpio_pin_set_dt(&seg_d, 0); // try out segment
        };
        response = gpio_pin_set_dt(&seg_e, 1); // try out segment
        k_sleep(K_MSEC(1000));
        if (enable_shutdown)
        {
            response = gpio_pin_set_dt(&seg_e, 0); // try out segment
        }
        response = gpio_pin_set_dt(&seg_f, 1); // try out segment
        k_sleep(K_MSEC(1000));
        if (enable_shutdown)
        {
            response = gpio_pin_set_dt(&seg_f, 0); // try out segment
        }
        response = gpio_pin_set_dt(&seg_g, 1); // try out segment
        k_sleep(K_MSEC(1000));
        if (enable_shutdown)
        {
            response = gpio_pin_set_dt(&seg_g, 0); // try out segment
        }
        response = gpio_pin_set_dt(&seg_dp, 1); // try out segment
        k_sleep(K_MSEC(1000));
        if (enable_shutdown)
        {
            response = gpio_pin_set_dt(&seg_dp, 0); // try out segment
        };
    }
    return 0;
}

// Display a diven digit
// index - 0 to 3,  HH:MM
void display_digit(uint8_t digit, uint8_t index)
{
    reset_display(); // make sure only one digit is displayed at a time
    // prepare the digit's index (select the 8-bit segment)
    gpio_pin_set_dt(digit_buffer[index], 1);
    // draw the digit
    uint8_t temp_bin = digit_codes[digit];                   // binary representation of digit
    for (uint8_t i = 0; i < ARRAY_SIZE(segment_buffer); i++) // go through one segment (bit) after the other
    {
        gpio_pin_set_dt(segment_buffer[i], temp_bin % 2); // if the bit is 1 -> set the segment ACTIVE
        temp_bin = temp_bin >> 1;                         // right shift after the bit is processed
    }
}

// Reset (set INACTIVE) all segments of a digit (position)
void reset_digit(uint8_t index)
{
    for (uint8_t i = 0; i < ARRAY_SIZE(segment_buffer); i++) // go through one segment (bit) after the other
    {
        gpio_pin_set_dt(segment_buffer[i], 0);
    }
}

// Reset (set INACTIVE) all digits (positions)
void reset_display()
{
    for (uint8_t i = 0; i < ARRAY_SIZE(digit_buffer); i++)
    {
        reset_digit(i);
    }
    k_sleep(K_MSEC(20));
}