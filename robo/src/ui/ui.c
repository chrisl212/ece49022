#include <stdarg.h>
#include <string.h>
#include "state/state.h"
#include "stm32f0xx.h"
#include "ui.h"

#define ROW (2)
#define COL (16)

char lines[ROW][COL] = {0};
int z = 0;

void _setupTIM3(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    GPIOC->MODER |= (0x2 << (2*6)) + (0x0 << (2*7));
    
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    TIM3->CCMR1 |= TIM_CCMR1_CC1S_0 + TIM_CCMR1_IC1F + TIM_CCMR1_IC1PSC_1;
    TIM3->CCER |= TIM_CCER_CC1E;
    TIM3->DIER |= TIM_DIER_CC1IE;
    NVIC->ISER[0] |= 1 << TIM3_IRQn;
    TIM3->CR1 |= TIM_CR1_CEN;
}

void TIM3_IRQHandler(void) {
    if (state == STATE_SELECT) {
        if (GPIOC->IDR & (1 << 7)) {
            z--;
        } else {
            z++;
        }
        TIM3->SR &= ~(TIM_SR_CC1IF);
        ui_writeFormat(1, "num: %d", z);
    }
}

static void _setupSPI(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
    GPIOA->MODER |= (0x2 << (2*4)) + (0x2 << (2*5)) + (0x2 << (2*7));
    SPI1->CR1 |= SPI_CR1_MSTR + SPI_CR1_BR;
    SPI1->CR1 |=  SPI_CR1_BIDIMODE + SPI_CR1_BIDIOE;
    SPI1->CR2 = SPI_CR2_SSOE | SPI_CR2_NSSP | SPI_CR2_DS_3 | SPI_CR2_DS_0;
    SPI1->CR1 |= SPI_CR1_SPE;
}

static void _SPIWrite(unsigned int data) {
    while ((SPI1->SR & SPI_SR_TXE) != 2);
    SPI1->DR = data;
}

void ui_setup(void) {
    _setupSPI();
    _SPIWrite(0x0038); //set function
    _SPIWrite(0x000C); //turn on display
    _SPIWrite(0x0001); //clear display
    _SPIWrite(0x0002); //return home
    _SPIWrite(0x0006); //set entry mode
    ui_writeLine(0, "");
    _setupTIM3();
}

void ui_writeLine(int line, char *str) {
    _SPIWrite(0x0080 + 0x40*line);
    int l = strlen(str);
    for (int i = 0; i < COL; i++) {
        if (i < l) _SPIWrite(0x0200 | str[i]);
        else _SPIWrite(0x0200 | ' ');
    }
}

static int _pow(int a, int b) {
    int res = 1;
    while (b--) {
        res *= a;
    }
    return res;
}

static int _writeInt(int i) {
    int digs, tmp, div, j;
   
    j = 0;
    if (i < 0) {
        _SPIWrite(0x0200 | '-');
        i = -i;
        j++;
    } 
    digs = (i == 0);
    tmp = i;

    while (tmp) {
        tmp /= 10;
        digs++;
    }

    tmp = i;
    while (digs) {
        div = _pow(10, --digs);
        _SPIWrite(0x0200 | ('0' + tmp/div));
        tmp %= div;
        j++;
    }
    return j;
}

void ui_writeFormat(int line, char *fmt, ...) {
    va_list lst;
    int i = 0;
    char *s;

    va_start(lst, fmt);
    _SPIWrite(0x0080 + 0x40*line);

    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            switch (*fmt) {
                case 'd':
                    i += _writeInt(va_arg(lst, int));
                    break;
                case 's':
                    s = va_arg(lst, char *);
                    while (*s) {
                        _SPIWrite(0x0200 | *s++);
                        i++;
                    }
                    break;
                default:
                    _SPIWrite(0x0200 | *fmt++);
            }
        } else {
            _SPIWrite(0x0200 | *fmt);
            i++;
        }
        fmt++;
    }    

    while (i++ < COL) {
        _SPIWrite(0x0200 | ' ');
    }
    va_end(lst);    
}
