#include "stm32c0xx_hal.h"

int main(void){
  HAL_Init();

  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  GPIO_InitTypeDef led = {.Pin = GPIO_PIN_5, .Mode = GPIO_MODE_OUTPUT_PP};
  HAL_GPIO_Init(GPIOA, &led);

  GPIO_InitTypeDef btn = {.Pin = GPIO_PIN_13, .Mode = GPIO_MODE_INPUT, .Pull = GPIO_PULLUP };
  HAL_GPIO_Init(GPIOC, &btn);

  // [상태 저장 변수] 0이면 꺼짐, 1이면 켜짐
  int led_state = 0;

  while(1){
    // 1. 버튼이 눌렸는지 확인
    // PULLUP 상태이기 때문에 초기 set(1) 상태
    if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET){

      // 2. 상태를 뒤집기
      led_state = !led_state;

      // 3. 바뀐 상태를 LED에 적용
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, (GPIO_PinState)led_state);

      // 4. 버튼을 떼기 전까지 기다려주기
      while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET);
      
      // 5. 아주 잠깐 쉬어서 전기적 노이즈 방지
      HAL_Delay(50);
    }
  }
}
