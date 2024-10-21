#include "event_handler.h"

LOG_MODULE_REGISTER(events, CONFIG_LOG_DEFAULT_LEVEL); // Registers the log level for the module

/*
 * Get button configuration from the devicetree alias. This is mandatory.
 */
const struct gpio_dt_spec button_a = GPIO_DT_SPEC_GET_OR(BTN_A_NODE, gpios,
                                                         {0});
const struct gpio_dt_spec button_b = GPIO_DT_SPEC_GET_OR(BTN_B_NODE, gpios,
                                                         {0});
const struct gpio_dt_spec button_c = GPIO_DT_SPEC_GET_OR(BTN_C_NODE, gpios,
                                                         {0});
const struct gpio_dt_spec button_d = GPIO_DT_SPEC_GET_OR(BTN_D_NODE, gpios,
                                                         {0});
struct gpio_callback button_a_cb_data;
struct gpio_callback button_b_cb_data;
struct gpio_callback button_c_cb_data;
struct gpio_callback button_d_cb_data;

/*
 * Array of button structures for all the buttons to be initialized programmatically.
 */
struct button buttons[] = {
    {&button_a, &button_a_cb_data, button_a_pressed},
    {&button_b, &button_b_cb_data, button_b_pressed},
    {&button_c, &button_c_cb_data, button_c_pressed},
    {&button_d, &button_d_cb_data, button_d_pressed}};

// Functions

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

void button_c_pressed(const struct device *dev, struct gpio_callback *cb,
                      uint32_t pins)
{
    LOG_INF("Button C pressed at %" PRIu32 "", k_cycle_get_32());
}

void button_d_pressed(const struct device *dev, struct gpio_callback *cb,
                      uint32_t pins)
{
    LOG_INF("Button D pressed at %" PRIu32 "", k_cycle_get_32());
}

// Generalized function to initialize a button
int init_button(const struct button *btn)
{
    int ret;

    if (!gpio_is_ready_dt(btn->spec))
    {
        LOG_ERR("Error: button device %s is not ready", btn->spec->port->name);
        return 1;
    }

    ret = gpio_pin_configure_dt(btn->spec, GPIO_INPUT);
    if (ret != 0)
    {
        LOG_ERR("Error %d: failed to configure %s pin %d", ret, btn->spec->port->name, btn->spec->pin);
        return 1;
    }

    ret = gpio_pin_interrupt_configure_dt(btn->spec, GPIO_INT_EDGE_TO_ACTIVE);
    if (ret != 0)
    {
        LOG_ERR("Error %d: failed to configure interrupt on %s pin %d", ret, btn->spec->port->name, btn->spec->pin);
        return 1;
    }

    gpio_init_callback(btn->cb_data, btn->cb_handler, BIT(btn->spec->pin));
    gpio_add_callback(btn->spec->port, btn->cb_data);
    LOG_INF("Set up button at %s pin %d", btn->spec->port->name, btn->spec->pin);
    return 0;
}

// Initialise buttons
int init_buttons(void)
{
    int ret;
    size_t num_buttons = ARRAY_SIZE(buttons); // Number of buttons

    for (size_t i = 0; i < num_buttons; i++)
    {
        ret = init_button(&buttons[i]);
        if (ret != 0)
        {
            LOG_ERR("Button initialization failed for button %zu", i);
            return ret;
        }
    }
}