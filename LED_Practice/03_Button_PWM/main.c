#include "stm32c0xx_hal.h"

// 타이머 핸들 구조체 선언
TIM_HandleTypeDef htim2;

void SystemClock_Config(void);
void MX_TIM1_Init(void);

int main(void){
  HAL_Init();
  SystemClock_Config();
  MX_TIM1_Init();

  // 버튼 설정 (PC13)
  __HAL_RCC_GPIOC_CLK_ENABLE();
  GPIO_InitTypeDef btn = {.Pin = GPIO_PIN_13, .Mode = GPIO_MODE_INPUT, .Pull = GPIO_PULLUP };
  HAL_GPIO_Init(GPIOC, &btn);

  // PWM 시작
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

  int mode = 0; // 0: 꺼짐 1: 약하게 2: 밝게
  uint32_t brightness[] = {0, 300, 1000}; // PWM 값 0~1000 범

  while(1){
    // 버튼이 눌렸을 때 (RESET)
    if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET){
      // 1. 모드 전환 ( 0 -> 1 -> 2 -> 0 ...)
      mode = (mode + 1) % 3;

      // 2. PWM 값 변경 (듀티 사이클 설정)
      __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, brightness[mode]);

      // 3. 디바운싱: 손 뗄 때까지 기다리고 넉넉히 지연
      while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET)
      HAL_Delay(100);
    }
  }
}

// 타이머 설정 함수 (PA5를 PWM 모드로 설정)
void MX_TIM1_Init(void){
  __HAL_RCC_TIM1_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  // PA5 핀을 '타이머 출력' 모드로 설정
  GPIO_InitTypeDef GPIO_InitStruct = {
    .Pin = GPIO_PIN_5,
    .Mode = GPIO_MODE_AF_PP, // Alternative Function (특수 기능)
    .Pull = GPIO_NOPULL,
    .Speed = GPIO_SPEED_FREQ_LOW,
    .Alternate = GPIO_AF1_TIM1 // PA5를 TIM1 채널1에 연결
  };
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 48 - 1; // 48MHz -> 1Mhz
  htim1.Init.Period = 1000 - 1; // 0~1000까지 주기
  HAL_TIM_PWM_Init(&htim1);

  TIM_OC_InitTypeDef sConfigOC = {
    .OCMode = TIM_OCMODE_PWM1,
    .Pulse = 0, // 초기 밝기 0
    .OCPolarity = TIM_OCPOLARITY_HIGH
  };
  HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1);
}

void SystemClock_Config(void){
// 시뮬레이터 기본 설정인 48MHz
}
