#include "event_handler.h"

#define DEBOUNCE_TIME_MS 80                            // Debouncing delay in milliseconds
#define LONG_PRESS_DURATION_MS 2000                    // Long press threshold in ms
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
    {&button_a, &button_a_cb_data, button_a_pressed, "A", false, 0},
    {&button_b, &button_b_cb_data, button_b_pressed, "B", false, 0},
    {&button_c, &button_c_cb_data, button_c_pressed, "C", false, 0},
    {&button_d, &button_d_cb_data, button_d_pressed, "D", false, 0}};
/* Work item for handling debouncing */
static struct k_work_delayable button_work;

// Button callback functions and debouncing

/* Debouncing handler */
void button_work_handler(struct k_work *work)
{
    // Iterate over each button to check their current state
    for (int i = 0; i < ARRAY_SIZE(buttons); i++)
    {
        bool current_state = gpio_pin_get_dt(buttons[i].spec);
        if (current_state != buttons[i].pressed)
        {
            buttons[i].pressed = current_state;
            if (current_state)
            {
                buttons[i].press_time = k_uptime_get(); // time since boot in ms
            }
            else
            {
                int64_t duration = k_uptime_get() - buttons[i].press_time;
                if (duration >= LONG_PRESS_DURATION_MS)
                {
                    // Long press detected
                    LOG_INF("%s long press (duration: %lld ms)", buttons[i].label, duration);
                }
                else
                {
                    // Short press detected
                    LOG_INF("%s short press (duration: %lld ms)", buttons[i].label, duration);
                }
            }
        }
    }
}

void button_a_pressed(const struct device *dev, struct gpio_callback *cb,
                      uint32_t pins)
{
    k_work_reschedule(&button_work, K_MSEC(DEBOUNCE_TIME_MS)); // wait until debounce processed
    LOG_INF("Button A pressed at %lld ms", k_uptime_get());
    timer_state current_state = get_state();
    switch (current_state)
    {
    case SLEEPING:
        LOG_INF("Exiting sleep mode. Setting seconds now.");
        set_state(SET_SECONDS);
        inc_seconds();
        break;
    case SET_SECONDS:
        inc_seconds();
        break;
    case SET_MINUTES:
        inc_minutes();
        break;
    default:
        break;
    }
    LOG_INF("Current state: %d", current_state);
}

void button_b_pressed(const struct device *dev, struct gpio_callback *cb,
                      uint32_t pins)
{
    k_work_reschedule(&button_work, K_MSEC(DEBOUNCE_TIME_MS)); // wait until debounce processed
    LOG_INF("Button B pressed at %lld ms", k_uptime_get());
    timer_state current_state = get_state();
    switch (current_state)
    {
    case SLEEPING:
        LOG_INF("Exiting sleep mode. Setting seconds now.");
        set_state(SET_SECONDS);
        dec_seconds();
        break;
    case SET_SECONDS:
        dec_seconds();
        break;
    case SET_MINUTES:
        dec_minutes();
        break;
    default:
        break;
    }
    LOG_INF("Current state: %d", current_state);
}

// Left button (C) switches to minutes control
void button_c_pressed(const struct device *dev, struct gpio_callback *cb,
                      uint32_t pins)
{
    k_work_reschedule(&button_work, K_MSEC(DEBOUNCE_TIME_MS)); // wait until debounce processed
    LOG_INF("Button C pressed at %lld ms", k_uptime_get());
    timer_state current_state = get_state();
    switch (current_state)
    {
    case SLEEPING:
        // play_sound(MODE_SOUND, 1); // TODO implement sound thread
        LOG_INF("Exiting sleep mode. Setting minutes now.");
        set_state(SET_MINUTES);
        break;
    case SET_SECONDS:
        // play_sound(MODE_SOUND, 1); // TODO implement sound thread
        LOG_INF("Setting minutes now.");
        set_state(SET_MINUTES);
        break;
    default:
        break;
    }
    LOG_INF("Current state: %d", current_state);
}

// Right button (D) switches to seconds control
void button_d_pressed(const struct device *dev, struct gpio_callback *cb,
                      uint32_t pins)
{
    k_work_reschedule(&button_work, K_MSEC(DEBOUNCE_TIME_MS)); // wait until debounce processed
    LOG_INF("Button D pressed at %lld ms", k_uptime_get());
    timer_state current_state = get_state();
    switch (current_state)
    {
    case SLEEPING:
        // play_sound(MODE_SOUND, 1); // TODO implement sound thread
        LOG_INF("Exiting sleep mode. Setting seconds now.");
        set_state(SET_SECONDS);
        break;
    case SET_MINUTES:
        // play_sound(MODE_SOUND, 1); // TODO implement sound thread
        LOG_INF("Setting seconds now.");
        set_state(SET_SECONDS);
        break;
    default:
        break;
    }
    LOG_INF("Current state: %d", current_state);
}

// Generalized function to initialize a button
int init_button(const struct button *btn)
{
    int ret;

    // Initialize debounce work
    k_work_init_delayable(&button_work, button_work_handler);

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
    int ret = 0;
    for (size_t i = 0; i < ARRAY_SIZE(buttons); i++)
    {
        ret = init_button(&buttons[i]);
        if (ret != 0)
        {
            LOG_ERR("Button initialization failed for button %zu", i);
        }
    }
    return ret;
}