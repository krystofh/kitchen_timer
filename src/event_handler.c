#include "event_handler.h"

LOG_MODULE_REGISTER(events, CONFIG_LOG_DEFAULT_LEVEL); // Registers the log level for the module

/*
 * Get button configuration from the devicetree alias. This is mandatory.
 */
const struct gpio_dt_spec button_a = GPIO_DT_SPEC_GET_OR(BTN_A_NODE, gpios,
                                                         {0});
const struct gpio_dt_spec button_b = GPIO_DT_SPEC_GET_OR(BTN_B_NODE, gpios,
                                                         {0});
struct gpio_callback button_a_cb_data;
struct gpio_callback button_b_cb_data;

void button_a_pressed(const struct device *dev, struct gpio_callback *cb,
                      uint32_t pins)
{
    LOG_INF("Button A pressed at %" PRIu32 "", k_cycle_get_32());
}

void button_b_pressed(const struct device *dev, struct gpio_callback *cb,
                      uint32_t pins)
{
    LOG_INF("Button B pressed at %" PRIu32 "", k_cycle_get_32());
}

// Initialise buttons
int init_buttons(void)
{
    // BUTTON A
    int ret;
    if (!gpio_is_ready_dt(&button_a))
    {
        LOG_ERR("Error: button device %s is not ready\n",
                button_a.port->name);
        return 1;
    }

    ret = gpio_pin_configure_dt(&button_a, GPIO_INPUT);
    if (ret != 0)
    {
        LOG_ERR("Error %d: failed to configure %s pin %d",
                ret, button_a.port->name, button_a.pin);
        return 1;
    }

    ret = gpio_pin_interrupt_configure_dt(&button_a,
                                          GPIO_INT_EDGE_TO_ACTIVE);
    if (ret != 0)
    {
        LOG_ERR("Error %d: failed to configure interrupt on %s pin %d",
                ret, button_a.port->name, button_a.pin);
        return 1;
    }

    gpio_init_callback(&button_a_cb_data, button_a_pressed, BIT(button_a.pin));
    gpio_add_callback(button_a.port, &button_a_cb_data);
    LOG_INF("Set up button at %s pin %d", button_a.port->name, button_a.pin);

    // BUTTON B
    if (!gpio_is_ready_dt(&button_b))
    {
        LOG_ERR("Error: button device %s is not ready\n",
                button_b.port->name);
        return 1;
    }

    ret = gpio_pin_configure_dt(&button_b, GPIO_INPUT);
    if (ret != 0)
    {
        LOG_ERR("Error %d: failed to configure %s pin %d",
                ret, button_b.port->name, button_b.pin);
        return 1;
    }

    ret = gpio_pin_interrupt_configure_dt(&button_b,
                                          GPIO_INT_EDGE_TO_ACTIVE);
    if (ret != 0)
    {
        LOG_ERR("Error %d: failed to configure interrupt on %s pin %d",
                ret, button_b.port->name, button_b.pin);
        return 1;
    }

    gpio_init_callback(&button_b_cb_data, button_b_pressed, BIT(button_b.pin));
    gpio_add_callback(button_b.port, &button_b_cb_data);
    LOG_INF("Set up button at %s pin %d", button_b.port->name, button_b.pin);
    return 0;
}