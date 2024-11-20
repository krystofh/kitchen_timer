#include "event_handler.h"

#define DEBOUNCE_TIME_MS 15                            // Debouncing delay in milliseconds
#define LONG_PRESS_DURATION_MS 1000                    // Long press threshold in ms
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

static struct k_work_delayable button_a_work;
static struct k_work_delayable button_b_work;
static struct k_work_delayable button_c_work;
static struct k_work_delayable button_d_work;

static struct k_work_delayable longpress_a_work;
static struct k_work_delayable longpress_b_work;
static struct k_work_delayable longpress_c_work;
static struct k_work_delayable longpress_d_work;

/*
 * Array of button structures for all the buttons to be initialized programmatically.
 */
struct button buttons[] = {
    {&button_a, &button_a_cb_data, button_a_isr, "A"},
    {&button_b, &button_b_cb_data, button_b_isr, "B"},
    {&button_c, &button_c_cb_data, button_c_isr, "C"},
    {&button_d, &button_d_cb_data, button_d_isr, "D"}};

// Button A actions on press or release after debounce
static void button_work_a_handler(struct k_work *work)
{
    ARG_UNUSED(work);
    int val = gpio_pin_get_dt(&button_a);
    enum button_evt evt = val ? BUTTON_EVT_PRESSED : BUTTON_EVT_RELEASED;
    // Onpress of debounced button A
    if (evt == BUTTON_EVT_PRESSED)
    {
        play_sound(CLICK_SOUND, 1);
        LOG_INF("Button A pressed at %lld ms", k_uptime_get());
        timer_state current_state = get_state();
        switch (current_state)
        {
        case SLEEPING:
            LOG_INF("Exiting sleep mode. Setting minutes now.");
            set_state(SET_MINUTES);
            inc_minutes();
            break;
        case SET_SECONDS:
            inc_seconds();
            break;
        case SET_MINUTES:
            inc_minutes();
            break;
        case ALARM:
            stop_alarm(); // stops alarm sound on button press
            break;
        default:
            break;
        }
        LOG_INF("Current state: %d", current_state);
        k_work_reschedule(&longpress_a_work, K_MSEC(LONG_PRESS_DURATION_MS));
    }
    else
    {
        k_work_cancel_delayable(&longpress_a_work); // on release, clear longpress queue
    }
}

static void longpress_a_work_handler(struct k_work *work)
{
    LOG_INF("long press A detected");
}

// Interrupt service routine for BUTTON A (+)
void button_a_isr(const struct device *dev, struct gpio_callback *cb,
                  uint32_t pins)
{
    // Check if work queue has some pending work (button already being debounced)
    if (k_work_delayable_is_pending(&button_a_work))
    {
        // Cancel ongoing work and reschedule debounce timer to debounce
        k_work_cancel_delayable(&button_a_work);
    }
    // Reschedule debounce work for x ms
    k_work_reschedule(&button_a_work, K_MSEC(DEBOUNCE_TIME_MS));
}

// Button B actions on press or release after debounce
static void button_work_b_handler(struct k_work *work)
{
    ARG_UNUSED(work);
    int val = gpio_pin_get_dt(&button_b);
    enum button_evt evt = val ? BUTTON_EVT_PRESSED : BUTTON_EVT_RELEASED;
    // Onpress of debounced button B
    if (evt == BUTTON_EVT_PRESSED)
    {
        play_sound(CLICK_SOUND, 1);
        LOG_INF("Button B pressed at %lld ms", k_uptime_get());
        timer_state current_state = get_state();
        switch (current_state)
        {
        case SLEEPING:
            LOG_INF("Exiting sleep mode. Setting minutes now.");
            set_state(SET_MINUTES);
            dec_minutes();
            break;
        case SET_SECONDS:
            dec_seconds();
            break;
        case SET_MINUTES:
            dec_minutes();
            break;
        case ALARM:
            stop_alarm(); // stops alarm sound on button press
            break;
        default:
            break;
        }
        LOG_INF("Current state: %d", current_state);
        k_work_reschedule(&longpress_b_work, K_MSEC(LONG_PRESS_DURATION_MS));
    }
    else
    {
        k_work_cancel_delayable(&longpress_b_work); // on release, clear longpress queue
    }
}

static void longpress_b_work_handler(struct k_work *work)
{
    LOG_INF("long press B detected");
}

// Interrupt service routine for BUTTON B (-)
void button_b_isr(const struct device *dev, struct gpio_callback *cb,
                  uint32_t pins)
{
    // Check if work queue has some pending work (button already being debounced)
    if (k_work_delayable_is_pending(&button_b_work))
    {
        // Cancel ongoing work and reschedule debounce timer to debounce
        k_work_cancel_delayable(&button_b_work);
    }
    // Reschedule debounce work for x ms
    k_work_reschedule(&button_b_work, K_MSEC(DEBOUNCE_TIME_MS));
}

// Button C actions on press or release after debounce
static void button_work_c_handler(struct k_work *work)
{
    ARG_UNUSED(work);
    int val = gpio_pin_get_dt(&button_c);
    enum button_evt evt = val ? BUTTON_EVT_PRESSED : BUTTON_EVT_RELEASED;
    // Onpress of debounced button C
    if (evt == BUTTON_EVT_PRESSED)
    {
        play_sound(MODE_SOUND, 1);
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
        case ALARM:
            stop_alarm(); // stops alarm sound on button press
            break;
        default:
            break;
        }
        LOG_INF("Current state: %d", current_state);
        k_work_reschedule(&longpress_c_work, K_MSEC(LONG_PRESS_DURATION_MS));
    }
    else
    {
        k_work_cancel_delayable(&longpress_c_work); // on release, clear longpress queue
    }
}

static void longpress_c_work_handler(struct k_work *work)
{
    LOG_INF("long press C detected");
}

// Interrupt service routine for BUTTON C
void button_c_isr(const struct device *dev, struct gpio_callback *cb,
                  uint32_t pins)
{
    // Check if work queue has some pending work (button already being debounced)
    if (k_work_delayable_is_pending(&button_c_work))
    {
        // Cancel ongoing work and reschedule debounce timer to debounce
        k_work_cancel_delayable(&button_c_work);
    }
    // Reschedule debounce work for x ms
    k_work_reschedule(&button_c_work, K_MSEC(DEBOUNCE_TIME_MS));
}

// Button D actions on press or release after debounce
static void button_work_d_handler(struct k_work *work)
{
    ARG_UNUSED(work);
    int val = gpio_pin_get_dt(&button_d);
    enum button_evt evt = val ? BUTTON_EVT_PRESSED : BUTTON_EVT_RELEASED;
    // Onpress of debounced button D
    if (evt == BUTTON_EVT_PRESSED)
    {
        play_sound(MODE_SOUND, 1);
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
        case ALARM:
            stop_alarm(); // stops alarm sound on button press
            break;
        default:
            break;
        }
        LOG_INF("Current state: %d", current_state);
        k_work_reschedule(&longpress_d_work, K_MSEC(LONG_PRESS_DURATION_MS));
    }
    else
    {
        k_work_cancel_delayable(&longpress_d_work); // on release, clear longpress queue
    }
}

static void longpress_d_work_handler(struct k_work *work)
{
    LOG_INF("long press D detected");
}

// Interrupt service routine for BUTTON D
void button_d_isr(const struct device *dev, struct gpio_callback *cb,
                  uint32_t pins)
{
    // Check if work queue has some pending work (button already being debounced)
    if (k_work_delayable_is_pending(&button_d_work))
    {
        // Cancel ongoing work and reschedule debounce timer to debounce
        k_work_cancel_delayable(&button_d_work);
    }
    // Reschedule debounce work for x ms
    k_work_reschedule(&button_d_work, K_MSEC(DEBOUNCE_TIME_MS));
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

    ret = gpio_pin_interrupt_configure_dt(btn->spec, GPIO_INT_EDGE_BOTH);
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

    // Init works
    k_work_init_delayable(&button_a_work, button_work_a_handler);
    k_work_init_delayable(&button_b_work, button_work_b_handler);
    k_work_init_delayable(&button_c_work, button_work_c_handler);
    k_work_init_delayable(&button_d_work, button_work_d_handler);

    k_work_init_delayable(&longpress_a_work, longpress_a_work_handler);
    k_work_init_delayable(&longpress_b_work, longpress_b_work_handler);
    k_work_init_delayable(&longpress_c_work, longpress_c_work_handler);
    k_work_init_delayable(&longpress_d_work, longpress_d_work_handler);

    return ret;
}