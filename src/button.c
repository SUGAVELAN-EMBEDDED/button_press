#include "button.h"
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>

#define DEBOUNCE_DELAY      50
#define LONG_PRESS_TIME     1000
#define DOUBLE_PRESS_TIME   400

#define BUTTON_NODE DT_NODELABEL(button0)

static const struct gpio_dt_spec button =
    GPIO_DT_SPEC_GET(BUTTON_NODE, gpios);

static struct gpio_callback button_cb_data;
static struct k_work_delayable button_work;

/* State variables */
static int last_state = 0;
static int press_time = 0;
static int release_time = 0;
static int click_count = 0;
static int long_press_handled = 0;

static button_event_t event = BUTTON_IDLE;

/* ISR */
static void button_isr(const struct device *dev,
                       struct gpio_callback *cb,
                       uint32_t pins)
{
    k_work_reschedule(&button_work, K_MSEC(DEBOUNCE_DELAY));
}

/* Debounce handler */
static void button_work_handler(struct k_work *work)
{
    int current_state = gpio_pin_get_dt(&button);
    int now = k_uptime_get();

    /* BUTTON PRESS */
    if (current_state && !last_state) {

        press_time = now;

        /* Fresh press */
        long_press_handled = 0;
    }

    /* BUTTON RELEASE */
    if (!current_state && last_state) {

        /* Count click only if long press did NOT happen */
        if (!long_press_handled) {

            release_time = now;
            click_count++;
        }
    }

    last_state = current_state;
}

/* Public: get event */
button_event_t button_get_event(void)
{
    int now = k_uptime_get();

    /* LONG PRESS */
    if (last_state &&
        (now - press_time > LONG_PRESS_TIME) &&
        !long_press_handled) {

        long_press_handled = 1;
        click_count = 0;
        return BUTTON_LONG_PRESS;
    }

    /* DOUBLE PRESS */
    if (click_count == 2 &&
        (now - release_time < DOUBLE_PRESS_TIME)) {

        click_count = 0;
        return BUTTON_DOUBLE_PRESS;
    }

    /* SINGLE PRESS */
    if (click_count == 1 &&
        (now - release_time > DOUBLE_PRESS_TIME)) {

        click_count = 0;
        return BUTTON_SINGLE_PRESS;
    }

    return BUTTON_IDLE;
}

/* Init */
void button_init(void)
{
    if (!device_is_ready(button.port)) {
        printk("Button not ready\n");
        return;
    }

    gpio_pin_configure_dt(&button, GPIO_INPUT);

    gpio_pin_interrupt_configure_dt(&button,
        GPIO_INT_EDGE_BOTH);

    gpio_init_callback(&button_cb_data,
        button_isr,
        BIT(button.pin));

    gpio_add_callback(button.port, &button_cb_data);

    k_work_init_delayable(&button_work,
        button_work_handler);
}
