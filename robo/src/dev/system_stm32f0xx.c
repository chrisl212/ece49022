#include "stm32f0xx.h"

#define PLL_SOURCE_HSI        // HSI (~8MHz) used to clock the PLL, and the PLL is used as system clock source
uint32_t clock_freq    = 48000000;
__I uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
static void _setSysClock(void);

void dev_init(void) {    
  /* Set HSION bit */
  RCC->CR |= (uint32_t)0x00000001;
  /* Reset SW[1:0], HPRE[3:0], PPRE[2:0], ADCPRE and MCOSEL[2:0] bits */
  RCC->CFGR &= (uint32_t)0xF8FFB80C;
  /* Reset HSEON, CSSON and PLLON bits */
  RCC->CR &= (uint32_t)0xFEF6FFFF;
  /* Reset HSEBYP bit */
  RCC->CR &= (uint32_t)0xFFFBFFFF;
  /* Reset PLLSRC, PLLXTPRE and PLLMUL[3:0] bits */
  RCC->CFGR &= (uint32_t)0xFFC0FFFF;
  /* Reset PREDIV1[3:0] bits */
  RCC->CFGR2 &= (uint32_t)0xFFFFFFF0;
  /* Reset USARTSW[1:0], I2CSW, CECSW and ADCSW bits */
  RCC->CFGR3 &= (uint32_t)0xFFFFFEAC;
  /* Reset HSI14 bit */
  RCC->CR2 &= (uint32_t)0xFFFFFFFE;
  /* Disable all interrupts */
  RCC->CIR = 0x00000000;
  /* Configure the System clock frequency, AHB/APBx prescalers and Flash settings */
  _setSysClock();
}

void dev_cloclUpdate(void) {
  uint32_t tmp = 0, pllmull = 0, pllsource = 0, prediv1factor = 0;

  /* Get SYSCLK source -------------------------------------------------------*/
  tmp = RCC->CFGR & RCC_CFGR_SWS;
  
  switch (tmp) {
    case 0x00:  /* HSI used as system clock */
      clock_freq = HSI_VALUE;
      break;
    case 0x04:  /* HSE used as system clock */
      clock_freq = HSE_VALUE;
      break;
    case 0x08:  /* PLL used as system clock */
      /* Get PLL clock source and multiplication factor ----------------------*/
      pllmull = RCC->CFGR & RCC_CFGR_PLLMULL;
      pllsource = RCC->CFGR & RCC_CFGR_PLLSRC;
      pllmull = ( pllmull >> 18) + 2;
      
      if (pllsource == 0x00) {
        /* HSI oscillator clock divided by 2 selected as PLL clock entry */
        clock_freq = (HSI_VALUE >> 1) * pllmull;
      } else {
        prediv1factor = (RCC->CFGR2 & RCC_CFGR2_PREDIV1) + 1;
        /* HSE oscillator clock selected as PREDIV1 clock entry */
        clock_freq = (HSE_VALUE / prediv1factor) * pllmull; 
      }      
      break;
    default: /* HSI used as system clock */
      clock_freq = HSI_VALUE;
      break;
  }
  /* Compute HCLK clock frequency ----------------*/
  /* Get HCLK prescaler */
  tmp = AHBPrescTable[((RCC->CFGR & RCC_CFGR_HPRE) >> 4)];
  /* HCLK clock frequency */
  clock_freq >>= tmp;  
}

static void _setSysClock(void) {
  __IO uint32_t StartUpCounter = 0, HSEStatus = 0;
  
  /* SYSCLK, HCLK, PCLK configuration ----------------------------------------*/
#if defined (PLL_SOURCE_HSI)
  /* At this stage the HSI is already enabled */

  /* Enable Prefetch Buffer and set Flash Latency */
  FLASH->ACR = FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY;
 
  /* HCLK = SYSCLK */
  RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;
      
  /* PCLK = HCLK */
  RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE_DIV1;

  /* PLL configuration = (HSI/2) * 12 = ~48 MHz */
  RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL));
  RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_HSI_Div2 | RCC_CFGR_PLLXTPRE_PREDIV1 | RCC_CFGR_PLLMULL12);
            
  /* Enable PLL */
  RCC->CR |= RCC_CR_PLLON;

  /* Wait till PLL is ready */
  while((RCC->CR & RCC_CR_PLLRDY) == 0);

  /* Select PLL as system clock source */
  RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
  RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;    

  /* Wait till PLL is used as system clock source */
  while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)RCC_CFGR_SWS_PLL);
#else
 #if defined (PLL_SOURCE_HSE)
  /* Enable HSE */    
  RCC->CR |= ((uint32_t)RCC_CR_HSEON);
 #elif defined (PLL_SOURCE_HSE_BYPASS)
  /* HSE oscillator bypassed with external clock */    
  RCC->CR |= (uint32_t)(RCC_CR_HSEON | RCC_CR_HSEBYP);
 #endif /* PLL_SOURCE_HSE */
   
  /* Wait till HSE is ready and if Time out is reached exit */
  do {
    HSEStatus = RCC->CR & RCC_CR_HSERDY;
    StartUpCounter++;  
  } while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

  if ((RCC->CR & RCC_CR_HSERDY) != RESET) {
    HSEStatus = (uint32_t)0x01;
  } else {
    HSEStatus = (uint32_t)0x00;
  }  

  if (HSEStatus == (uint32_t)0x01) {
    /* Enable Prefetch Buffer and set Flash Latency */
    FLASH->ACR = FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY;
 
    /* HCLK = SYSCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;
      
    /* PCLK = HCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE_DIV1;

    /* PLL configuration = HSE * 6 = 48 MHz */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL));
    RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_PREDIV1 | RCC_CFGR_PLLXTPRE_PREDIV1 | RCC_CFGR_PLLMULL6);
            
    /* Enable PLL */
    RCC->CR |= RCC_CR_PLLON;

    /* Wait till PLL is ready */
    while((RCC->CR & RCC_CR_PLLRDY) == 0);

    /* Select PLL as system clock source */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;    

    /* Wait till PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)RCC_CFGR_SWS_PLL);
  }
#endif /* PLL_SOURCE_HSI */  
}

