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

// Simple demo showing 0 sequentially on all digits
int display_demo()
{
    int response = gpio_pin_set_dt(&digit_2, 1);
    if (response < 0)
    {
        return 1;
    }
    response = gpio_pin_set_dt(&seg_f, 0); // try out segment A
    if (response < 0)
    {
        return 1;
    }
    return 0;
}