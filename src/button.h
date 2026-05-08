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


extern  struct k_msgq button_msgq;
#endif