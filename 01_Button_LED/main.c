#include "stm32c0xx_hal.h"

int main(void){
  HAL_Init();

  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  GPIO_InitTypeDef led_struct = {0};
  led_struct.Pin = GPIO_PIN_5;
  led_struct.Mode = GPIO_MODE_OUTPUT_PP;
  HAL_GPIO_Init(GPIOA, &led_struct);

  GPIO_InitTypeDef btn_struct = {0};
  btn_struct.Pin = GPIO_PIN_13;
  btn_struct.Mode = GPIO_MODE_INPUT;
  btn_struct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &btn_struct);

  while(1){
    if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_SET){
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
    } else{
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
    }
  }
}
