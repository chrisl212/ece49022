#include "stm32f0xx.h"
#include "i2c.h"

void i2c_waitIdle(void) {
    while ((I2C1->ISR & I2C_ISR_BUSY) == I2C_ISR_BUSY);
}

void i2c_setup(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

    GPIOB->MODER |= (0x2 << (2*6)) + (0x2 << (2*7));
    GPIOB->AFR[0] |= (0x1 << (4*6)) + (0x1 << (4*7));

    I2C1->CR1 &= ~(I2C_CR1_PE);
    I2C1->CR2 &= ~(I2C_CR2_ADD10);
    I2C1->CR2 |= I2C_CR2_NACK;
    I2C1->TIMINGR &= ~(I2C_TIMINGR_PRESC);
    //PRESC: 0xB
    //SCLDEL: 0x4
    //SDADEL: 0x2
    //SCLH: 0xF
    //SCLL: 0x13
    I2C1->TIMINGR |= ((0xB - 1) << 28) +
                     ((0x4 - 1) << 20) +
                     (0x2 << 16) +
                     ((0xF - 1) << 8) +
                     ((0x13 - 1) << 0);
    I2C1->OAR1 &= ~(I2C_OAR1_OA1EN);
    I2C1->OAR2 &= ~(I2C_OAR2_OA2EN);
    I2C1->CR1 |= I2C_CR1_PE;
}

void i2c_start(uint8_t addr, uint8_t dir) {
    I2C1->CR2 &= ~(I2C_CR2_SADD);
    I2C1->CR2 |= (addr << 1);
    if (dir == I2C_RD) {
        I2C1->CR2 |= I2C_CR2_RD_WRN;
    } else {
        I2C1->CR2 &= ~(I2C_CR2_RD_WRN);
    }
    I2C1->CR2 |= I2C_CR2_START;
}

void i2c_stop(void) {
    if (I2C1->ISR & I2C_ISR_STOPF) {
        return;
    }
    I2C1->CR2 |= I2C_CR2_STOP;
    while (!(I2C1->ISR & I2C_ISR_STOPF));
    I2C1->ICR |= I2C_ICR_STOPCF;
}

void i2c_sendData(uint8_t *data, uint32_t size) {
    I2C1->CR2 &= ~(I2C_CR2_NBYTES);
    I2C1->CR2 |= (size << 16);
    for (int i = 0; i < size; i++) {
        uint32_t timeout = 0;
        while (!(I2C1->ISR & I2C_ISR_TXIS)) {
            timeout++;
            if (timeout > 1000000) {
                return;
            }
        }
        I2C1->TXDR = data[i];
    }
    while (!((I2C1->ISR & I2C_ISR_TC) || (I2C1->ISR & I2C_ISR_NACKF)));
}

void i2c_readData(uint8_t *data, uint32_t size) {
    I2C1->CR2 &= ~(I2C_CR2_NBYTES);
    I2C1->CR2 |= (size << 16);
    for (int i = 0; i < size; i++) {
        uint32_t timeout = 0;
        while (!(I2C1->ISR & I2C_ISR_RXNE)) {
            timeout++;
            if (timeout > 1000000) {
                return;
            }
        }
        data[i] = I2C1->RXDR;
    }
    while (!((I2C1->ISR & I2C_ISR_TC) || (I2C1->ISR & I2C_ISR_NACKF)));
}
