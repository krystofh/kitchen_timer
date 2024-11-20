#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>
#include <zephyr/logging/log.h>
#include <inttypes.h>

#include "timer.h"
#include "sound_player.h"

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

enum button_evt
{
    BUTTON_EVT_PRESSED,
    BUTTON_EVT_RELEASED
};

/*
 * Button structure to hold the GPIO configuration and callback data.
 */
struct button
{
    const struct gpio_dt_spec *spec;    // GPIO spec from device tree
    struct gpio_callback *cb_data;      // Callback structure
    gpio_callback_handler_t cb_handler; // Callback function
    const char *label;                  // Button label (e.g., "Button A")
};

extern const struct gpio_dt_spec button_a;
extern struct gpio_callback button_a_cb_data;
extern const struct gpio_dt_spec button_b;
extern struct gpio_callback button_b_cb_data;
extern const struct gpio_dt_spec button_c;
extern struct gpio_callback button_c_cb_data;
extern const struct gpio_dt_spec button_d;
extern struct gpio_callback button_d_cb_data;

// Function declarations

static void button_work_a_handler(struct k_work *work);
static void button_work_b_handler(struct k_work *work);
static void button_work_c_handler(struct k_work *work);
static void button_work_d_handler(struct k_work *work);

static void longpress_a_work_handler(struct k_work *work);
static void longpress_b_work_handler(struct k_work *work);
static void longpress_c_work_handler(struct k_work *work);
static void longpress_d_work_handler(struct k_work *work);

void button_a_isr(const struct device *dev, struct gpio_callback *cb, uint32_t pins);
void button_b_isr(const struct device *dev, struct gpio_callback *cb,
                  uint32_t pins);
void button_c_isr(const struct device *dev, struct gpio_callback *cb,
                  uint32_t pins);
void button_d_isr(const struct device *dev, struct gpio_callback *cb,
                  uint32_t pins);
int init_buttons(void);

#endif // EVENT_HANDLER_H