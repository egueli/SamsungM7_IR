#include "esp_common.h"
#include "freertos/task.h"
#include "gpio.h"

void task_blink(void* ignore)
{
    GPIO_OUTPUT_SET(2, 1);
    while(true) {
    	GPIO_OUTPUT_SET(2, 1);
        vTaskDelay(1000/portTICK_RATE_MS);
    	GPIO_OUTPUT_SET(2, 0);
        vTaskDelay(1000/portTICK_RATE_MS);
    }

    vTaskDelete(NULL);
}

void user_init(void)
{
    xTaskCreate(&task_blink, "startup", 2048, NULL, 1, NULL);
}
