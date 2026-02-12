#include "stm32c0xx_hal.h"

// 타이머 핸들 구조체 선언
TIM_HandleTypeDef htim1;

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
      HAL_Delay(50); // 버틈 누름 시 디바운싱
      mode = (mode + 1) % 3;
      __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, brightness[mode]);

      // 3. 디바운싱: 손 뗄 때까지 기다리고 넉넉히 지연
      while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET){
      }
      HAL_Delay(50);
    }
  }
}

// 타이머 설정 함수 (PA8를 PWM 모드로 설정)
void MX_TIM1_Init(void){
  __HAL_RCC_TIM1_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  // PA8 핀을 '타이머 출력' 모드로 설정
  GPIO_InitTypeDef GPIO_InitStruct = {
    .Pin = GPIO_PIN_8,
    .Mode = GPIO_MODE_AF_PP, // Alternative Function (특수 기능)
    .Pull = GPIO_NOPULL,
    .Speed = GPIO_SPEED_FREQ_LOW,
    .Alternate = GPIO_AF2_TIM1 // PA8 TIM1_CH1에 해당
  };
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 48 - 1; // 48MHz -> 1Mhz
  htim1.Init.Period = 1000 - 1; // 0~1000까지 주기
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP; // 카운터 모드 명시
  htim1.Init.RepetitionCounter = 0; // TIM1 필수 설정
  HAL_TIM_PWM_Init(&htim1);

  TIM_OC_InitTypeDef sConfigOC = {
    .OCMode = TIM_OCMODE_PWM1,
    .Pulse = 0, // 초기 밝기 0
    .OCPolarity = TIM_OCPOLARITY_HIGH,
    .OCNPolarity = TIM_OCNPOLARITY_HIGH,
    .OCIdleState = TIM_OCIDLESTATE_RESET,
    .OCNIdleState = TIM_OCNIDLESTATE_RESET
  };
  HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1);
}

void SystemClock_Config(void){
// 시뮬레이터 기본 설정인 48MHz
}
