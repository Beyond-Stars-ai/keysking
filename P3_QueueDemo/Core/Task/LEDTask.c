#include "cmsis_os2.h"
#include "main.h"
#include "LEDType.h"
#include "FreeRTOS.h"

extern UART_HandleTypeDef huart2;

void StartLEDTask(void *argument) {
    for (;;) {
        //HAL_UART_Transmit(&huart2, (uint8_t*)"LED Task Running\r\n", 16, HAL_MAX_DELAY);
        LEDMessage *message;
        osMessageQueueGet(LEDQueueHandle, &message, 0, osWaitForever);
        switch (message->color) {
            case LEDColor_Red:
                HAL_GPIO_WritePin(RedLED_GPIO_Port, RedLED_Pin, message->state ? GPIO_PIN_SET : GPIO_PIN_RESET);
                break;
            case LEDColor_Green:
                HAL_GPIO_WritePin(GreenLED_GPIO_Port, GreenLED_Pin, message->state ? GPIO_PIN_SET : GPIO_PIN_RESET);
                break;
            case LEDColor_Blue:
                HAL_GPIO_WritePin(BlueLED_GPIO_Port, BlueLED_Pin, message->state ? GPIO_PIN_SET : GPIO_PIN_RESET);
                break;
        }
        vPortFree(message);
    }
}