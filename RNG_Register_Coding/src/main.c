#include "stm32f4xx.h"

uint32_t rng_value;

void RNG_Config(void);
void CLK_Config();

int main(void)
{
	CLK_Config();
	RNG_Config();

  while (1)
  {

  }
}

void CLK_Config() // Clock speed for 168MHz
{
	RCC->CR |= 0x00010000;                 // HSEON ENABLE
	while(!(RCC->CR & 0x00020000));        // HSEON Ready Flag wait
	RCC->CR |= 0x00080000;              // CSS ENABLE
	RCC->CR |= 0x01000000;				// PLL ON
	RCC->PLLCFGR |= 0x00400000;        // PLL SRC HSE is selected
	RCC->PLLCFGR |= 0x00000004;       // PLL M 4
	RCC->PLLCFGR |= 0x00005A00;        // PLL N 168
	RCC->PLLCFGR |= 0x00000000;       // PLL P 2
	RCC->CFGR |= 0x00000000;          // AHB PRESCALER 1
	RCC->CFGR |= 0x00080000;          // APB2 PRESCALER 2
	RCC->CFGR |= 0x00001400;          // APB1 PRESCALER 4
	RCC->CIR |= 0x00080000;             // HSE READY FLAG CLEAR
	RCC->CIR |= 0x00800000;             // CSS FLAG CLEAR
}

void RNG_Config()
{
	RCC->AHB2ENR |= 1 << 6;					// RNG CLK ENABLE
	RNG->CR |= 1 << 2;						// RNG ENABLE
	RNG->CR |= 1 << 3;						// RNG INTERRUPT ENABLE
	NVIC_EnableIRQ(HASH_RNG_IRQn);			// INTERRUPT IS ACTIVETED


}

void HASH_RNG_IRQHandler(void)
{
	if(RNG->SR & (1 << 0))					// IS DATA READY?
	{
		RNG->CR = 0;						// CLEAR CONTROL REGISTER (DISABLE RNG)
		rng_value = RNG->DR;				// GET RANDOM NUMBER
	}
	else
	{
		RNG->SR = 0;						// CLEAR STATUS REGISTER
		RNG->CR = 0;						// CLEAR CONTROL REGISTER (DISABLE RNG)
		RNG->CR |= 1 << 2;					// RNG ENABLE
		RNG->CR |= 1 << 3;					// RNG INTERRUPT ENABLE

	}
}
