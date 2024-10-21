#include "display_driver.h"

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

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
    const struct gpio_dt_spec *digit_buffer[] = {&digit_1, &digit_2, &digit_3, &digit_4};
    const struct gpio_dt_spec *segment_buffer[] = {&seg_a, &seg_b, &seg_c, &seg_d, &seg_e, &seg_f, &seg_g, &seg_dp};

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