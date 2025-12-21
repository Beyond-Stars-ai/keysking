#include "cmsis_os2.h"
#include "main.h"

#include "LEDType.h"
#include "stdlib.h"
#include "FreeRTOS.h"

extern UART_HandleTypeDef huart2;

#define IS_KEY_PRESSED() (HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin) == GPIO_PIN_RESET)
// 按键检测间隔
#define KEY_CHECK_INTERVAL 10
// 按键消抖时间
#define KEY_DEBOUNCE_TIME 30
// 按键消抖次数 (30 / 10 = 3次)
#define KEY_DEBOUNCE_COUNT (KEY_DEBOUNCE_TIME / KEY_CHECK_INTERVAL)

uint8_t isKeyClicked() {
    static uint8_t count = 0;  // 消抖计数
    static uint8_t pressed = 0; // 用于每次按键只返回一次点击事件
    if (IS_KEY_PRESSED() && !pressed) {
        count++;
        if (count >= KEY_DEBOUNCE_COUNT && IS_KEY_PRESSED()) {
            pressed = 1;
            return 1; // 按键被点击
        }
    }
    if (!IS_KEY_PRESSED()) {
        pressed = 0; // 按键释放，重置状态
        count = 0;   // 按键释放，重置消抖计数
    }
    return 0;
}

// typedef enum {
//     LEDColor_Red = 0,
//     LEDColor_Green = 1,
//     LEDColor_Blue = 2
// } LEDColor;

// typedef enum {
//     LEDState_On = 1,
//     LEDState_Off = 0
// } LEDState;

// typedef struct {
//     LEDColor color;
//     LEDState state;
// } LEDMessage;

void StartKEYTask(void *argument) {
    LEDState state = LEDState_Off;
    for (;;) {
        if (isKeyClicked()) {
            //HAL_UART_Transmit(&huart2, (uint8_t *)"Key Pressed!\r\n", 15, HAL_MAX_DELAY);
            state = !state;
            LEDMessage* message = pvPortMalloc(sizeof(LEDMessage));
            //HAL_UART_Transmit(&huart2, (uint8_t *)"Key Precess!\r\n", 15, HAL_MAX_DELAY);
            message->color = LEDColor_Red;  // 或根据逻辑选择其他颜色
            message->state = state;    // 或根据逻辑设置状态
            // LEDMessage* pMessage = &message;
            osMessageQueuePut(LEDQueueHandle, &message, 0, 1000);
            //HAL_UART_Transmit(&huart2, (uint8_t *)"Key gogogo!\r\n", 15, HAL_MAX_DELAY);
        }
        //HAL_UART_Transmit(&huart2, (uint8_t *)"Key Task Running!\r\n", 18, HAL_MAX_DELAY);
        osDelay(10);
    }
}