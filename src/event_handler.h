#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>
#include <zephyr/logging/log.h>
#include <inttypes.h>

#define BTN_A_NODE DT_ALIAS(sw0)
#if !DT_NODE_HAS_STATUS_OKAY(BTN_A_NODE)
#error "Unsupported board: button0 devicetree alias is not defined"
#endif

extern const struct gpio_dt_spec button_a;
extern struct gpio_callback button_a_cb_data;

// Function declarations
void button_a_pressed(const struct device *dev, struct gpio_callback *cb,
                      uint32_t pins);
int init_buttons(void);

#endif // EVENT_HANDLER_H