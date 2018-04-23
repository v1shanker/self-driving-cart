/* UART Echo Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "esp_log.h"
#include "definitions.h"

static const char *N = "Serial";

static void android_main()
{
	uart_event_t event;
	size_t bytes_read;
	int pos;
	
	uint8_t pattern_dump[5];
	uint8_t data[512];

	for (;;){
		if(xQueueReceive(android_uart_queue, (void *)&event, (portTickType)portMAX_DELAY)) {
			switch(event.type){
				case UART_PATTERN_DET:
					/* Get actual data */
					pos = uart_pattern_pop_pos(ANDROID_PORT);
					bytes_read = uart_read_bytes(ANDROID_PORT, data, pos , 20/portTICK_RATE_MS);
					data[pos] = '\0';
					
					/* Remove pattern */
					uart_read_bytes(ANDROID_PORT, pattern_dump, 1, 20/portTICK_RATE_MS);
					pattern_dump[1] = '\0';
					printf("Data buffer is %s\n", data);
					
					/* Send to motor */
					if (data[0] == 'M'){
						xQueueSend(motor_in_queue, (void *)(data), (portTickType)portMAX_DELAY);
					} 
					
					/* Send to LIDAR */
					else if (data[0] == 'L'){
						xQueueSend(lidar_in_queue, (void *)(data), (portTickType)portMAX_DELAY);
					}
					
					else {
						printf("My friend this could not have gone worse\n");
					}
					break;
				default:
					break;
					//printf("Different event\n\n");
			}
		}
		
		
	}
}

void app_main()
{
	esp_log_level_set(N, ESP_LOG_INFO);
	
	uart_config_t uart_config1 = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
	
	uart_config_t uart_config2 = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
	
	/* Set up android serial connection */
    uart_param_config(ANDROID_PORT, &uart_config1);
    uart_set_pin(ANDROID_PORT, PHONE_TXD, PHONE_RXD, SERIAL_RTS, SERIAL_CTS);
    uart_driver_install(ANDROID_PORT, BUF_SIZE, BUF_SIZE, 20, &android_uart_queue, 0);
    uart_enable_pattern_det_intr(ANDROID_PORT, '\n', PATTERN_NUM, 10000, 10, 10);
	
    /* Reset the pattern queue length to record at most 20 pattern positions. */
    uart_pattern_queue_reset(ANDROID_PORT, 20);

	/* Set up LIDAR serial connection */
	uart_param_config(LIDAR_PORT, &uart_config2);
    uart_set_pin(LIDAR_PORT, LIDAR_TXD, LIDAR_RXD, SERIAL_RTS, SERIAL_CTS);
    uart_driver_install(LIDAR_PORT, BUF_SIZE, BUF_SIZE, 20, &lidar_uart_queue, 0);
    uart_enable_pattern_det_intr(LIDAR_PORT, 0x04, PATTERN_NUM, 10000, 10, 10);
	
	uart_pattern_queue_reset(LIDAR_PORT, 20);
	
	//Create message queues
	motor_in_queue = xQueueCreate(10,sizeof(char)*15);
	lidar_in_queue = xQueueCreate(10,sizeof(char)*15);
	
    xTaskCreate(android_main, "android_interface", 4096, NULL, 1, NULL);
	xTaskCreate(lidar_main, "lidar_interface",4096, NULL, 2, NULL);
	xTaskCreate(motor_main, "motor_interface",4096, NULL,2, NULL);
}