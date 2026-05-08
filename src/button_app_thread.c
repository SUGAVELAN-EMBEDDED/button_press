#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#include "button.h"

#define APP_THREAD_STACK_SIZE   1024
#define APP_THREAD_PRIORITY     5

K_THREAD_STACK_DEFINE(app_stack,
                      APP_THREAD_STACK_SIZE);

static struct k_thread app_thread_data;

static void app_thread(void *a,
                       void *b,
                       void *c)
{
    button_event_t event;

    while (1) {

        k_msgq_get(&button_msgq,
                   &event,
                   K_FOREVER);

        switch (event) {

        case BUTTON_SINGLE_PRESS:
            printk("SINGLE PRESS\n");
            break;

        case BUTTON_DOUBLE_PRESS:
            printk("DOUBLE PRESS\n");
            break;

        case BUTTON_LONG_PRESS:
            printk("LONG PRESS\n");
            break;

        default:
            break;
        }
    }
}

void app_thread_start(void)
{
    k_thread_create(&app_thread_data,
                    app_stack,
                    K_THREAD_STACK_SIZEOF(app_stack),
                    app_thread,
                    NULL, NULL, NULL,
                    APP_THREAD_PRIORITY,
                    0,
                    K_NO_WAIT);
}