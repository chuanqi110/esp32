#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "driver/dac.h"
#include "driver/uart.h"
#include "esp_system.h"

#include "soc/gpio_sig_map.h"
#include "soc/io_mux_reg.h"


#define USER_UART_GPIO_TX 18
#define USER_UART_GPIO_RX 19

#define BUF_SIZE (1024)

void user_uart_gpio_config(){
    //SELECT PIN FUNCTION
    //PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO16_U, FUNC_GPIO16_U2RXD);
    //PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO17_U, FUNC_GPIO17_U2TXD);
    
    // Route GPIO signals to/from pins
    gpio_matrix_out(USER_UART_GPIO_TX, U2TXD_OUT_IDX, 0, 0);
    gpio_matrix_in(USER_UART_GPIO_RX, U2RXD_IN_IDX, 0);
}


//an example of echo test with hardware flow control on UART1
static void echo_task()
{
    const int uart_num = UART_NUM_2;
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
       // .flow_ctrl = UART_HW_FLOWCTRL_CTS_RTS,
        .rx_flow_ctrl_thresh = 122,
    };
    //Configure UART2 parameters
    uart_param_config(uart_num, &uart_config);

    //Install UART driver (we don't need an event queue here)
    //In this example we don't even use a buffer for sending data.
    uart_driver_install(uart_num, BUF_SIZE * 2, 0, 0, NULL, 0);

    uint8_t* data = (uint8_t*) malloc(BUF_SIZE);
    while(1) {
        //Read data from UART
        int len = uart_read_bytes(uart_num, data, BUF_SIZE, 20 / portTICK_RATE_MS);
        if(len > 0){
            printf("data len : %d\n", len);
        }
        //Write data back to UART
        uart_write_bytes(uart_num, (const char*) data, len);
    }

    vTaskDelete(NULL);
}
void app_main(void)
{

    //config UART GPIO
    user_uart_gpio_config();
    xTaskCreate(echo_task, "uart_echo_task", 2048, NULL, 10, NULL);
}
