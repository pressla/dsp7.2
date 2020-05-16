/*
 * Konfiguration des Display 7 Boards:
 *
 * ext. Clock HSE: 8 MHz
 * M: 4
 * N: 180
 * P: 2
 * ergibt einen Sysclk von 180 MHz
 *
 * AHB Prescaler: 1
 * APB1 Prescaler: 4 ergibt einen APB1 = 45 MHz und einen APB1-Timer Clock: 90 MHz
 * APB2 Prescaler: 2 ergibt einen APB2 = 90 MHz und einen APB2-Timer Clock: 180 MHz
 *
 */
  
#include "stm32f4xx.h"

static void SetSysClock(void);

#define VECT_TAB_OFFSET 0x00
#define PLL_M     		4
#define PLL_N			180
#define PLL_P			2
#define PLL_Q			7	// nicht benutzt, ansonsten noch einzustellen

uint32_t SystemCoreClock = 180000000;
__I uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};

void SystemInit(void)
{
	// schalte die FPU ein
	SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));

  /* Reset the RCC clock configuration to the default reset state ------------*/
  RCC->CR |= (uint32_t)0x00000001;
  RCC->CFGR = 0x00000000;
  RCC->CR &= (uint32_t)0xFEF6FFFF;
  RCC->PLLCFGR = 0x24003010;
  RCC->CR &= (uint32_t)0xFFFBFFFF;
  RCC->CIR = 0x00000000;

  /* Configure the System clock source, PLL Multiplier and Divider factors, 
     AHB/APBx prescalers and Flash settings ----------------------------------*/
  SetSysClock();

  SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal FLASH */
}

static void SetSysClock(void)
{
/******************************************************************************/
/*            PLL (clocked by HSE) used as System clock source                */
/******************************************************************************/
  __IO uint32_t StartUpCounter = 0, HSEStatus = 0;
  
  // Enable HSE
  RCC->CR |= ((uint32_t)RCC_CR_HSEON);
 
  // Wait till HSE is ready and if Time out is reached exit
  do
  {
    HSEStatus = RCC->CR & RCC_CR_HSERDY;
    StartUpCounter++;
  } while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

  if ((RCC->CR & RCC_CR_HSERDY) != RESET)
    HSEStatus = (uint32_t)0x01;
  else
    HSEStatus = (uint32_t)0x00;

  if (HSEStatus == (uint32_t)0x01)
  {
    /* Select regulator voltage output Scale 1 mode */
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    PWR->CR |= PWR_CR_VOS;

    /* HCLK = SYSCLK / 1*/
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;

    /* PCLK2 = HCLK / 2*/
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;
    
    /* PCLK1 = HCLK / 4*/
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;


    /* Configure the main PLL */
    RCC->PLLCFGR = PLL_M | (PLL_N << 6) | (((PLL_P >> 1) -1) << 16) |
                   (RCC_PLLCFGR_PLLSRC_HSE) | (PLL_Q << 24);

    /* Enable the main PLL */
    RCC->CR |= RCC_CR_PLLON;

    /* Wait till the main PLL is ready */
    while((RCC->CR & RCC_CR_PLLRDY) == 0);
   
    /* Enable the Over-drive to extend the clock frequency to 180 Mhz */
    PWR->CR |= PWR_CR_ODEN;
    while((PWR->CSR & PWR_CSR_ODRDY) == 0);

    PWR->CR |= PWR_CR_ODSWEN;
    while((PWR->CSR & PWR_CSR_ODSWRDY) == 0);

    /* Configure Flash prefetch, Instruction cache, Data cache and wait state */
    FLASH->ACR = FLASH_ACR_PRFTEN | FLASH_ACR_ICEN |FLASH_ACR_DCEN |FLASH_ACR_LATENCY_5WS;

    /* Select the main PLL as system clock source */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= RCC_CFGR_SW_PLL;

    /* Wait till the main PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS ) != RCC_CFGR_SWS_PLL);
  }
  else
  { /* If HSE fails to start-up, the application will have wrong clock
         configuration. User can add here some code to deal with this error */
  }
}
