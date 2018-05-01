/*
 * @file definitions.h
 * @brief definitions for esp drivers
 *
 * @author Naveen
 * @author Vikram Shanker (vshanker@andrew.cmu.edu)
 * @bug None
 */

#ifndef __DEFINITIONS_H
#define __DEFINITIONS_H

#define LIDAR_PORT UART_NUM_1
#define ANDROID_PORT UART_NUM_2
#define MOTOR_PORT UART_NUM_0

#define PHONE_TXD  (GPIO_NUM_5)
#define PHONE_RXD  (GPIO_NUM_17)

#define LIDAR_TXD  (GPIO_NUM_16)
#define LIDAR_RXD  (GPIO_NUM_4)

#define SERIAL_RTS  (UART_PIN_NO_CHANGE)
#define SERIAL_CTS  (UART_PIN_NO_CHANGE)

#define RX_SIZE (8192)
#define TX_SIZE (256)

#define HBRIDGE_LEFT_IN1   (GPIO_NUM_22)
#define HBRIDGE_LEFT_IN2   (GPIO_NUM_23)
#define HBRIDGE_LEFT_PWM   (GPIO_NUM_21)
#define HBRIDGE_RIGHT_IN1   (GPIO_NUM_26)
#define HBRIDGE_RIGHT_IN2   (GPIO_NUM_25)
#define HBRIDGE_RIGHT_PWM   (GPIO_NUM_33)

#define PATTERN_NUM 1

#define MESSAGE_LEN 20
#define OUTPUT_DATA_POINTS 6
#define POINTS_PER_SECTION 20
#define MAX_RETRY 5

QueueHandle_t android_uart_queue;
QueueHandle_t lidar_uart_queue;

QueueHandle_t motor_in_queue;
QueueHandle_t lidar_in_queue;
QueueHandle_t android_out_queue;

typedef enum pulse_state_definitions {PULSE_HIGH, PULSE_LOW} pulse_state;
typedef enum direction_states {FORWARD, REVERSE} direction;

typedef struct compact_lidar_data {
	char data[(sizeof(uint32_t)*OUTPUT_DATA_POINTS)];
	uint8_t size;
} __attribute__((packed)) output_info;

void lidar_main();
void motor_main();
#endif