#ifndef BUTTON_H
#define BUTTON_H

#include <zephyr/kernel.h>

typedef enum {
    BUTTON_IDLE,
    BUTTON_SINGLE_PRESS,
    BUTTON_DOUBLE_PRESS,
    BUTTON_LONG_PRESS
} button_event_t;

void button_init(void);
button_event_t button_get_event(void);

#endif
