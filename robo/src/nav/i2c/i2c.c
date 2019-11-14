#include "stm32f0xx.h"
#include "i2c.h"

#define TIMEOUT (10000)

int i2c_timeout(int flag, int eq) {
    int timeout = 0;
    if (eq) {
        while ((I2C1->ISR & flag) == flag) {
            if (++timeout > TIMEOUT) {
                return -1;
            }
        }
    } else {
        while ((I2C1->ISR & flag) != flag) {
            if (++timeout > TIMEOUT) {
                return -1;
            }
        }
    }
    return 0;
}

int i2c_waitIdle(void) {
    return (i2c_timeout(I2C_ISR_BUSY, 1)) ? I2C_WAIT : I2C_OK;
}

int i2c_setup(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

    GPIOB->MODER |= (0x2 << (2*6)) + (0x2 << (2*7));
    GPIOB->AFR[0] |= (0x1 << (4*6)) + (0x1 << (4*7));

    I2C1->CR1 &= ~(I2C_CR1_PE);
    I2C1->CR2 &= ~(I2C_CR2_ADD10);
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
    return i2c_waitIdle();
}

int i2c_start(uint8_t addr, uint8_t dir) {
    I2C1->CR2 &= ~(I2C_CR2_SADD);
    I2C1->CR2 |= (addr << 1);
    if (dir == I2C_RD) {
        I2C1->CR2 |= I2C_CR2_RD_WRN;
    } else {
        I2C1->CR2 &= ~(I2C_CR2_RD_WRN);
    }
    I2C1->CR2 |= I2C_CR2_START;
    return I2C_OK;
}

int i2c_stop(void) {
    int timeout = 0;
    if (I2C1->ISR & I2C_ISR_STOPF) {
        return I2C_OK;
    }
    I2C1->CR2 |= I2C_CR2_STOP;
    if (i2c_timeout(I2C_ISR_STOPF, 0)) {
        return I2C_STOP;
    }
    if (i2c_timeout(I2C_ISR_BUSY, 1)) {
        return I2C_STOP;
    }
    I2C1->ICR |= I2C_ICR_STOPCF;
    return I2C_OK;
}

int i2c_sendData(uint8_t *data, uint32_t size) {
    int i;
    I2C1->CR2 &= ~(I2C_CR2_NBYTES);
    I2C1->CR2 |= (size << 16);
    for (i = 0; i < size; i++) {
        if (i2c_timeout(I2C_ISR_TXE, 0)) {
            return I2C_SEND;
        }
        I2C1->TXDR = data[i];
    }
    if (i2c_timeout(I2C_ISR_TC, 0)) {
        return I2C_SEND;
    }
    
    return I2C_OK;
}

int i2c_readData(uint8_t *data, uint32_t size) {
    I2C1->CR2 &= ~(I2C_CR2_NBYTES);
    I2C1->CR2 |= (size << 16);
    for (int i = 0; i < size; i++) {
        if (i2c_timeout(I2C_ISR_RXNE, 0)) {
            return I2C_READ;
        }
        data[i] = I2C1->RXDR;
    }
    if (i2c_timeout(I2C_ISR_TC, 0)) {
        return I2C_READ;
    }

    return I2C_OK;
}

char* i2c_error(i2cStatus_t s) {
    switch (s) {
        case I2C_OK:
            return "i2c: OK";
        case I2C_WAIT:
            return "i2c: waitIdle timeout";
        case I2C_SEND:
            return "i2c: send timeout";
        case I2C_READ:
            return "i2c: read timeout";
        case I2C_START:
            return "i2c: start timeout";
        case I2C_STOP:
            return "i2c: stop timeout";
    }
    return "i2c: unknown";
}
