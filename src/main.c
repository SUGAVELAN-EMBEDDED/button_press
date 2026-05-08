#include <zephyr/kernel.h>
#include "button.h"

void app_thread_start(void);

void main(void)
{
    button_init();

    app_thread_start();
}