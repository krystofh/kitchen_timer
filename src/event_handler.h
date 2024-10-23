#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>
#include <zephyr/logging/log.h>
#include <inttypes.h>

#define BTN_A_NODE DT_ALIAS(btn0)
#if !DT_NODE_HAS_STATUS_OKAY(BTN_A_NODE)
#error "Unsupported board: btn0 devicetree alias is not defined"
#endif
#define BTN_B_NODE DT_ALIAS(btn1)
#if !DT_NODE_HAS_STATUS_OKAY(BTN_B_NODE)
#error "Unsupported board: btn1 devicetree alias is not defined"
#endif
#define BTN_C_NODE DT_ALIAS(btn2)
#if !DT_NODE_HAS_STATUS_OKAY(BTN_C_NODE)
#error "Unsupported board: btn2 devicetree alias is not defined"
#endif
#define BTN_D_NODE DT_ALIAS(btn3)
#if !DT_NODE_HAS_STATUS_OKAY(BTN_D_NODE)
#error "Unsupported board: btn3 devicetree alias is not defined"
#endif

/*
 * Button structure to hold the GPIO configuration and callback data.
 */
struct button
{
    const struct gpio_dt_spec *spec;    // GPIO spec from device tree
    struct gpio_callback *cb_data;      // Callback structure
    gpio_callback_handler_t cb_handler; // Callback function
    const char *label;                  // Button label (e.g., "Button A")
    bool pressed;                       // Button state (pressed/released)
    int64_t press_time;                 // Timestamp of when the button was pressed
};

extern const struct gpio_dt_spec button_a;
extern struct gpio_callback button_a_cb_data;
extern const struct gpio_dt_spec button_b;
extern struct gpio_callback button_b_cb_data;
extern const struct gpio_dt_spec button_c;
extern struct gpio_callback button_c_cb_data;
extern const struct gpio_dt_spec button_d;
extern struct gpio_callback button_d_cb_data;

// Time
typedef struct time_t
{
    uint8_t minutes;
    uint8_t seconds;
};

typedef enum timer_state
{
    SLEEPING,
    SET_SECONDS,
    SET_MINUTES,
    COUNTDOWN,
    ALARM
};

// Function declarations

void button_a_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins);
void button_b_pressed(const struct device *dev, struct gpio_callback *cb,
                      uint32_t pins);
void button_c_pressed(const struct device *dev, struct gpio_callback *cb,
                      uint32_t pins);
void button_d_pressed(const struct device *dev, struct gpio_callback *cb,
                      uint32_t pins);
int init_buttons(void);

#endif // EVENT_HANDLER_H