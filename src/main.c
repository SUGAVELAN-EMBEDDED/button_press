#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include "button.h"

void main(void)
{
    button_init();

    while (1) {

        button_event_t event = button_get_event();

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

        k_msleep(50);
    }
}
