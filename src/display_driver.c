#include "display_driver.h"

#define STACK_SIZE 1024
#define DISP_THREAD_PRIORITY 5

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

// Digits and segments
const struct gpio_dt_spec *digits[] = {&digit_1, &digit_2, &digit_3, &digit_4};
const struct gpio_dt_spec *segments[] = {&seg_a, &seg_b, &seg_c, &seg_d, &seg_e, &seg_f, &seg_g, &seg_dp};
// Buffer to store the current digits to display
uint8_t display_buffer[4] = {0, 0, 0, 0};

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

// Spawn thread
K_THREAD_DEFINE(display_refresh_tid, STACK_SIZE, display_refresh_thread, NULL, NULL, NULL,
                DISP_THREAD_PRIORITY, 0, 0);

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

void display_refresh_thread()
{
    while (1)
    {
        for (uint8_t i = 0; i < ARRAY_SIZE(digits); i++)
        {
            display_digit(display_buffer[i], i);
            k_sleep(K_MSEC(1000 / (ARRAY_SIZE(digits) * CONFIG_DISPLAY_FPS))); // period [ms]
        }
    }
}

// Simple demo showing sequentially all segments on all digits
int display_demo()
{
    bool enable_shutdown = false;
    // Iterate through each digit in digits
    for (int i = 0; i < ARRAY_SIZE(digits); i++)
    {
        // Turn off all segemnts first
        for (int m = 0; m < ARRAY_SIZE(segments); m++)
        {
            gpio_pin_set_dt(segments[m], 0);
        }

        // Turn off all digits first to prevent ghosting
        for (int j = 0; j < ARRAY_SIZE(digits); j++)
        {
            gpio_pin_set_dt(digits[j], 0);
        }

        // Activate current digit
        int response = gpio_pin_set_dt(digits[i], 1);

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
    gpio_pin_set_dt(digits[index], 1);
    // draw the digit
    uint8_t temp_bin = digit_codes[digit];             // binary representation of digit
    for (uint8_t i = 0; i < ARRAY_SIZE(segments); i++) // go through one segment (bit) after the other
    {
        gpio_pin_set_dt(segments[i], temp_bin % 2); // if the bit is 1 -> set the segment ACTIVE
        temp_bin = temp_bin >> 1;                   // right shift after the bit is processed
    }
}

// Set the value to be displayed on the 4-digit display
void set_display_value(uint16_t value)
{
    // Convert the integer value to individual digits
    display_buffer[0] = (value / 1000) % 10;
    display_buffer[1] = (value / 100) % 10;
    display_buffer[2] = (value / 10) % 10;
    display_buffer[3] = value % 10;
}

// Checks if number is valid and sets it to the buffer for being regurarly refreshed
void display_number(uint16_t number)
{
    if (number < 10000)
    {
        set_display_value(number);
    }
    else
    {
        LOG_ERR("Can't display numbers > 9999");
    }
}

void display_time(timevar_t *displayed_time)
{
    LOG_INF("Setting time to: %d : %d ", displayed_time->minutes, displayed_time->seconds);
    set_display_value(displayed_time->minutes * 100 + displayed_time->seconds);
}

// Reset (set INACTIVE) all segments of the current position(s)
void reset_segments(void)
{
    for (uint8_t i = 0; i < ARRAY_SIZE(segments); i++) // go through one segment (bit) after the other
    {
        gpio_pin_set_dt(segments[i], 0);
    }
}

// Reset (set INACTIVE) all digits (positions)
void reset_display()
{
    reset_segments();
    for (uint8_t i = 0; i < ARRAY_SIZE(digits); i++)
    {
        gpio_pin_set_dt(digits[i], 0);
    }
}