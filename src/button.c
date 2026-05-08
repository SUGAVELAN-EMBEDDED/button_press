#include "button.h"

#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>

#define DEBOUNCE_DELAY      50
#define LONG_PRESS_TIME     1000
#define DOUBLE_PRESS_TIME   400

#define BUTTON_NODE DT_NODELABEL(button0)

static const struct gpio_dt_spec button =
    GPIO_DT_SPEC_GET(BUTTON_NODE, gpios);

#define MSGQ_SIZE 10

static char button_msgq_buffer[
    MSGQ_SIZE * sizeof(button_event_t)
];

struct k_msgq button_msgq;

static struct gpio_callback button_cb_data;
static struct k_work_delayable button_work;

/* State Variables */
static int last_state = 0;
static int press_time = 0;
static int release_time = 0;
static int click_count = 0;
static int long_press_handled = 0;

/* ISR */
static void button_isr(const struct device *dev,
                       struct gpio_callback *cb,
                       uint32_t pins)
{
    k_work_reschedule(&button_work,
                      K_MSEC(DEBOUNCE_DELAY));
}

/* Debounce Handler */
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

        /* Count click only if NOT long press */
        if (!long_press_handled) {

            release_time = now;
            click_count++;
        }
    }

    last_state = current_state;
}

/* Detect Events */
static void button_event_check(void)
{
    int now = k_uptime_get();

    button_event_t event;

    /* LONG PRESS */
    if (last_state &&
        (now - press_time > LONG_PRESS_TIME) &&
        !long_press_handled) {

        long_press_handled = 1;
        click_count = 0;

        event = BUTTON_LONG_PRESS;

        k_msgq_put(&button_msgq,
                   &event,
                   K_NO_WAIT);
    }

    /* DOUBLE PRESS */
    if (click_count == 2 &&
        (now - release_time < DOUBLE_PRESS_TIME)) {

        click_count = 0;

        event = BUTTON_DOUBLE_PRESS;

        k_msgq_put(&button_msgq,
                   &event,
                   K_NO_WAIT);
    }

    /* SINGLE PRESS */
    if (click_count == 1 &&
        (now - release_time > DOUBLE_PRESS_TIME)) {

        click_count = 0;

        event = BUTTON_SINGLE_PRESS;

        k_msgq_put(&button_msgq,
                   &event,
                   K_NO_WAIT);
    }
}

/* Button Thread */
#define BUTTON_THREAD_STACK_SIZE   1024
#define BUTTON_THREAD_PRIORITY     5

K_THREAD_STACK_DEFINE(button_stack,
                      BUTTON_THREAD_STACK_SIZE);

static struct k_thread button_thread_data;

static void button_thread(void *a,
                          void *b,
                          void *c)
{
    while (1) {

        button_event_check();

        k_msleep(20);
    }
}

/* Init */
void button_init(void)
{
    if (!device_is_ready(button.port)) {

        printk("Button device not ready\n");
        return;
    }

    gpio_pin_configure_dt(&button,
                          GPIO_INPUT);

    gpio_pin_interrupt_configure_dt(&button,
                                    GPIO_INT_EDGE_BOTH);

    gpio_init_callback(&button_cb_data,
                       button_isr,
                       BIT(button.pin));

    gpio_add_callback(button.port,
                      &button_cb_data);

    k_work_init_delayable(&button_work,
                          button_work_handler);


    k_msgq_init(&button_msgq,
            button_msgq_buffer,
            sizeof(button_event_t),
            MSGQ_SIZE);

    /* Start Thread */
    k_thread_create(&button_thread_data,
                    button_stack,
                    K_THREAD_STACK_SIZEOF(button_stack),
                    button_thread,
                    NULL, NULL, NULL,
                    BUTTON_THREAD_PRIORITY,
                    0,
                    K_NO_WAIT);
}