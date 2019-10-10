#ifndef I2C_H
#define I2C_H

#include <stdint.h>

#define I2C_WR 0
#define I2C_RD 1

void i2c_waitIdle(void);
void i2c_setup(void);
void i2c_start(uint8_t addr, uint8_t dir);
void i2c_stop(void);
void i2c_sendData(uint8_t *data, uint32_t size);
void i2c_readData(uint8_t *data, uint32_t size);

#endif
