#include "math/math.h"
#include "lsm9ds0.h"
#include "nav/i2c/i2c.h"

uint8_t lsm9ds0_read8(uint8_t addr, uint8_t reg) {
    uint8_t data;
    i2c_waitIdle();
    i2c_start(addr, I2C_WR);
    i2c_sendData(&reg, 1);
    i2c_start(addr, I2C_RD);
    i2c_readData(&data, 1);
    i2c_stop();
    return data;
}

void lsm9ds0_read(uint8_t addr, uint8_t reg, uint8_t *data, uint32_t size) {
    i2c_waitIdle();
    i2c_start(addr, I2C_WR);
    i2c_sendData(&reg, 1);
    i2c_start(addr, I2C_RD);
    i2c_readData(data, size);
    i2c_stop();
}

void lsm9ds0_write8(uint8_t addr, uint8_t reg, uint8_t data) {
    uint8_t buf[2] = {reg,data};
    i2c_waitIdle();
    i2c_start(addr, I2C_WR);
    i2c_sendData(buf, 2);
    i2c_stop();
}

uint8_t lsm9ds0_getAccelID(void) {
    return lsm9ds0_read8(SAD_XM, WHO_AM_I_XM);
}

uint8_t lsm9ds0_getGyroID(void) {
    return lsm9ds0_read8(SAD_G, WHO_AM_I_G);
}

int8_t lsm9ds0_setup(void) {
    i2c_setup();

    lsm9ds0_write8(SAD_G, CTRL_REG1_G, 0xFF);
    uint8_t r = lsm9ds0_read8(SAD_G, CTRL_REG4_G);
    r |= 0x30;
    lsm9ds0_write8(SAD_G, CTRL_REG4_G, r);

    lsm9ds0_write8(SAD_XM, CTRL_REG7_XM, 0x00);

    lsm9ds0_write8(SAD_XM, CTRL_REG1_XM, 0x67);
    lsm9ds0_write8(SAD_XM, CTRL_REG2_XM, 0x20);
    lsm9ds0_write8(SAD_XM, CTRL_REG5_XM, 0xF0);
    if (lsm9ds0_getGyroID() != 0xD4 || lsm9ds0_getAccelID() != 0x49)
        return -1;

    return 0;
}

coord_t lsm9ds0_getAccel(void) {
    coord_t c = {0};
    uint8_t buf[6], xlo, ylo, zlo;
    int16_t xhi, yhi, zhi;
    lsm9ds0_read(SAD_XM, 0x80 | OUT_X_L_A, buf, 6);
    xlo = buf[0];
    xhi = buf[1];
    ylo = buf[2];
    yhi = buf[3];
    zlo = buf[4];
    zhi = buf[5];

    // Shift values to create properly formed integer (low byte first)
    xhi <<= 8; xhi |= xlo;
    yhi <<= 8; yhi |= ylo;
    zhi <<= 8; zhi |= zlo;
    c.x = xhi*0.732F/1000;
    c.y = yhi*0.732F/1000;
    c.z = zhi*0.732F/1000;
    return c;
}

coord_t lsm9ds0_getGyro(void) {
    coord_t c = {0};
    uint8_t buf[6], xlo, ylo, zlo;
    int16_t xhi, yhi, zhi;
    lsm9ds0_read(SAD_G, 0x80 | OUT_X_L_G, buf, 6);
    xlo = buf[0];
    xhi = buf[1];
    ylo = buf[2];
    yhi = buf[3];
    zlo = buf[4];
    zhi = buf[5];

    // Shift values to create properly formed integer (low byte first)
    xhi <<= 8; xhi |= xlo;
    yhi <<= 8; yhi |= ylo;
    zhi <<= 8; zhi |= zlo;
    c.x = xhi*0.07000F;
    c.y = yhi*0.07000F;
    c.z = zhi*0.07000F;
    return c;
}

coord_t lsm9ds0_getMag(void) {
    coord_t c = {0};
    uint8_t buf[6], xlo, ylo, zlo;
    int16_t xhi, yhi, zhi;
    lsm9ds0_read(SAD_XM, 0x80 | OUT_X_L_M, buf, 6);
    xlo = buf[0];
    xhi = buf[1];
    ylo = buf[2];
    yhi = buf[3];
    zlo = buf[4];
    zhi = buf[5];

    // Shift values to create properly formed integer (low byte first)
    xhi <<= 8; xhi |= xlo;
    yhi <<= 8; yhi |= ylo;
    zhi <<= 8; zhi |= zlo;
    c.x = xhi*0.08/1000;
    c.y = yhi*0.08/1000;
    c.z = zhi*0.08/1000;
    return c;
}

uint16_t lsm9ds0_getHeading(void) {
    coord_t mag = lsm9ds0_getMag();
    int16_t heading;
    heading = atan2(mag.y, mag.x)*180/PI + 180;
    /*
    if (mag.y == 0.0) {
        if (mag.x >= 0.0) {
            heading = 0;
        } else {
            heading = 180;
        }
    } else {
        if (mag.y > 0.0) {
            heading = 90 - atan2(mag.y, mag.x)*180/M_PI;
        } else {
            heading = 270 - atan2(mag.y, mag.x)*180/M_PI;
        }
    }*/
    return heading;
}
