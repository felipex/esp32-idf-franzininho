/*
 Descrição: Blink usando as tasks do FreeRTOS no Franzininho wifi.
 
 Autor: Felipe Cavalcante
 Data : 21/04/22
*/


// Arquivos de cabeçalho
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gpio.h"
#include "esp_log.h"


// Definição de pino
#define LED1 GPIO_NUM_21
#define LED2 GPIO_NUM_33

#define DELAY_LED1 1000
#define DELAY_LED2 500

static const char *TAG = "Blinks in Task";


void configure_led(gpio_num_t led) {
	gpio_reset_pin(led);
    gpio_set_direction(led, GPIO_MODE_OUTPUT);
}

void blink_led(gpio_num_t led, TickType_t delay, const char *tag)
{
	configure_led(led);
    bool state = 0 ; // Varia o estado lógico do LED

    for (;;) {// Loop
       
        state = state^1; // Operação lógica XOR para alternar o estado do LED
        gpio_set_level(led, state); // Envia os níveis lógicos 1 ou 0
 
        ESP_LOGI(tag, "LED %d: i am Franzininho WiFi ----> LED status : %s", led, state == 0 ? "OFF" :  "ON");
        
        vTaskDelay(delay / portTICK_PERIOD_MS); // Rotina de Delay 
    
    }// end Loop

}

void task_LED1(void * pvParameters)
{
	blink_led(LED1, DELAY_LED1, TAG);
}

void task_LED2(void * pvParameters)
{
	blink_led(LED2, DELAY_LED2, TAG);
}

void app_main(void) // Main
{
	if (xTaskCreate(task_LED1, "task_LED1", 2048, NULL, 1, NULL) == pdPASS)
	{
		ESP_LOGI(TAG, "Task LED1 Iniciada...");
	}

	if (xTaskCreate(task_LED2, "task_LED2", 2048, NULL, 1, NULL) == pdPASS)
	{
		ESP_LOGI(TAG, "Task LED2 Iniciada...");
	} 
}// end Main
