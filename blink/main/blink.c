/*
 Descrição: Blink usando os dois leds do Franzininho wifi.
 
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
#define LEDS (1ULL << LED1) | (1ULL << LED2)

#define DELAY 100

static const char *TAG = "Blink";


void configure_leds(void) {
	
	gpio_config_t io_conf = {
		.pin_bit_mask = LEDS,
		.mode = GPIO_MODE_DISABLE,
		.pull_up_en = GPIO_PULLUP_ENABLE,
		.pull_down_en = GPIO_PULLDOWN_DISABLE,
		.intr_type = GPIO_INTR_DISABLE,
	};
	gpio_config(&io_conf);
    gpio_set_direction(LED1, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED2, GPIO_MODE_OUTPUT);
}


void app_main(void) // Main
{
	configure_leds();
	
	ESP_LOGI(TAG, "Hello World \n");// Mostra informação no terminal
       
    //Variaveis Local
    bool state = 0 ; // Varia o estado lógico do LED

    for (;;) {// Loop
       
        state = state^1; // Operação lógica XOR para alternar o estado do LED
        gpio_set_level(LED1, state); // Envia os níveis lógicos 1 ou 0
        gpio_set_level(LED2, state^1); // Envia os níveis lógicos 0 ou 1
 
        ESP_LOGI(TAG, "Hello World, i am Franzininho WiFi ----> LED status : %s", state == 0 ? "OFF" :  "ON");
        
        vTaskDelay(DELAY / portTICK_PERIOD_MS); // Rotina de Delay 
    
    }// end Loop
    
    
}// end Main
