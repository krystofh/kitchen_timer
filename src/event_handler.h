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

extern const struct gpio_dt_spec button_a;
extern struct gpio_callback button_a_cb_data;
extern const struct gpio_dt_spec button_b;
extern struct gpio_callback button_b_cb_data;

// Function declarations
void button_a_pressed(const struct device *dev, struct gpio_callback *cb,
                      uint32_t pins);
void button_b_pressed(const struct device *dev, struct gpio_callback *cb,
                      uint32_t pins);
int init_buttons(void);

#endif // EVENT_HANDLER_H