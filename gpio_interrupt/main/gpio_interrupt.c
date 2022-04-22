/******************************************************************************
* GPIO INPUT - Interrupt
* This example shows how to configure and use GPIO input with external interrupt on ESP32.
* This code is part of the course "Aprenda programar o ESP32 com ESP-IDF" by Fábio Souza
* The course is available on https://cursos.embarcados.com.br
*
* This example code Creative Commons Attribution 4.0 International License.
* When using the code, you must keep the above copyright notice,
* this list of conditions and the following disclaimer in the source code.
* (http://creativecommons.org/licenses/by/4.0/)

* Author: Fábio Souza
* Adaptado por Felipe Cavalcante em 21/04/2022
* This code is for teaching purposes only.
* No warranty of any kind is provided.
*******************************************************************************/

#include <stdio.h>

//freeRTOS includes
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

//ESP32 includes
#include "driver/gpio.h"
#include "esp_log.h"

//pin mapping
#define LED_PIN_1 21     //GPI21
#define LED_PIN_2 33     //GPI33
#define BUTTON_PIN 12    //GPI12
#define BUTTON_PIN2 13   //GPI13

//queue handle
QueueHandle_t gpio_evt_queue = NULL;  //queue to handle gpio events

//ISR handler
static void IRAM_ATTR gpio_isr_handler(void* arg)   
{
	//get the GPIO number
    uint32_t gpio_num = (uint32_t) arg;                  
	
	//send the GPIO number to the queue
	xQueueSendFromISR(gpio_evt_queue , &gpio_num, NULL); 
}

//button task
void buttonTask(void *pvpameters)
{
    uint32_t gpio_num;           //variable to store the GPIO number
    uint8_t led1 = 0, led2 = 0;  //variables to store the state of the LEDs

    for(;;)
    {
        xQueueReceive(gpio_evt_queue, &gpio_num, portMAX_DELAY);                                //wait for a value on queue
        ESP_LOGI("buttonTask", "GPIO[%d] intr, val: %d\n", gpio_num, gpio_get_level(gpio_num)); //print message on console

        if (gpio_num == BUTTON_PIN) 
        {
            gpio_set_level(LED_PIN_1, led1 ^= 1); 
        }
        else if (gpio_num == BUTTON_PIN2)          
        {
            gpio_set_level(LED_PIN_2, led2 ^= 1); 
        }
    }
}

//main function
void app_main(void)
{
	// Output in LEDs
	gpio_config_t io_config; 
 	io_config.pin_bit_mask = (1ULL<<LED_PIN_1) | (1ULL<<LED_PIN_2); 
    io_config.mode = GPIO_MODE_OUTPUT;                      
    io_config.pull_up_en = GPIO_PULLUP_DISABLE;            
    io_config.pull_down_en = GPIO_PULLDOWN_DISABLE;       
    io_config.intr_type = GPIO_INTR_DISABLE;             
    gpio_config(&io_config);                            
    gpio_set_level(LED_PIN_1, 0); 
    gpio_set_level(LED_PIN_2, 0); 

	// Input in buttons
    io_config.pin_bit_mask = (1ULL<<BUTTON_PIN)|(1ULL<<BUTTON_PIN2);
    io_config.mode = GPIO_MODE_INPUT;                              
    io_config.pull_up_en = GPIO_PULLUP_ENABLE;                    
    io_config.pull_down_en = GPIO_PULLDOWN_DISABLE;              
    io_config.intr_type = GPIO_INTR_NEGEDGE;                    
    gpio_config(&io_config);                       

	// Create the queue to communicate interruption callback function and the task
    gpio_evt_queue  = xQueueCreate(5, sizeof(uint32_t));       
	// Create task to manipulate data/button captured by interruption
    xTaskCreate(buttonTask, "buttonTask", 2048, NULL, 2, NULL);

    //install interrupt service routine
    gpio_install_isr_service(ESP_INTR_FLAG_LEVEL1);     
	//add ISR handler for button
	gpio_isr_handler_add(BUTTON_PIN, gpio_isr_handler, (void*) BUTTON_PIN); 
	//add ISR handler for button2
	gpio_isr_handler_add(BUTTON_PIN2, gpio_isr_handler, (void*) BUTTON_PIN2);
}

