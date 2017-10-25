#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "driver/dac.h"
#include "esp_system.h"

void app_main(void)
{
    esp_err_t err;
    uint8_t dac_op_vol = 0;
    uint8_t dac_output_count = 0;
    err = dac_output_enable(DAC_CHANNEL_1);
    printf("%s\n",err == ESP_OK ? "DAC ENABLE SUCCESS":"DAC ENABLE FAIL");
    while(1){
        err = dac_output_voltage(DAC_CHANNEL_1, dac_op_vol);
        if(err == ESP_OK){
            printf("DAC OUTPUT VALTAGE : %d \n", dac_op_vol);
            vTaskDelay(1000);
        }else{
            printf("DAC OUTPUT FAIL");
        }
        dac_output_count++;
        if(dac_output_count%2 == 0)
            dac_op_vol = 0;
        else
            dac_op_vol = 255;
    }
    
    fflush(stdout);
}
