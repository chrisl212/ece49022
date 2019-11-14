#include "math/math.h"
#include "lsm9ds0.h"
#include "nav/i2c/i2c.h"

static int i2c_err;

int lsm9ds0_read8(uint8_t addr, uint8_t reg) {
    uint8_t data;
    if ((i2c_err = i2c_waitIdle()) != I2C_OK) {
        return LSM9DS0_I2C;
    }

    if ((i2c_err = i2c_start(addr, I2C_WR)) != I2C_OK) {
        return LSM9DS0_I2C;
    }

    if ((i2c_err = i2c_sendData(&reg, 1)) != I2C_OK) {
        return LSM9DS0_I2C;
    }

    if ((i2c_err = i2c_start(addr, I2C_RD)) != I2C_OK) {
        return LSM9DS0_I2C;
    }

    if ((i2c_err = i2c_readData(&data, 1)) != I2C_OK) {
        return LSM9DS0_I2C;
    }

    if ((i2c_err = i2c_stop()) != I2C_OK) {
        return LSM9DS0_I2C;
    }
    
    return data;
}

int lsm9ds0_read(uint8_t addr, uint8_t reg, uint8_t *data, uint32_t size) {
    if ((i2c_err = i2c_waitIdle()) != I2C_OK) {
        return LSM9DS0_I2C;
    }

    if ((i2c_err = i2c_start(addr, I2C_WR)) != I2C_OK) {
        return LSM9DS0_I2C;
    }
    
    if ((i2c_err = i2c_sendData(&reg, 1)) != I2C_OK) {
        return LSM9DS0_I2C;
    }

    if ((i2c_err = i2c_start(addr, I2C_RD)) != I2C_OK) {
        return LSM9DS0_I2C;
    }
    
    if ((i2c_err = i2c_readData(data, size)) != I2C_OK) {
        return LSM9DS0_I2C;
    }

    if ((i2c_err = i2c_stop()) != I2C_OK) {
        return LSM9DS0_I2C;
    }
    return LSM9DS0_OK;
}

int lsm9ds0_write8(uint8_t addr, uint8_t reg, uint8_t data) {
    uint8_t buf[2] = {reg,data};
    if ((i2c_err = i2c_waitIdle()) != I2C_OK) {
        return LSM9DS0_I2C;
    }

    if ((i2c_err = i2c_start(addr, I2C_WR)) != I2C_OK) {
        return LSM9DS0_I2C;
    }
    
    if ((i2c_err = i2c_sendData(buf, 2)) != I2C_OK) {
        return LSM9DS0_I2C;
    }

    if ((i2c_err = i2c_stop()) != I2C_OK) {
        return LSM9DS0_I2C;
    }
    return LSM9DS0_OK;
}

int lsm9ds0_getAccelID(void) {
    return lsm9ds0_read8(SAD_XM, WHO_AM_I_XM);
}

int lsm9ds0_getGyroID(void) {
    return lsm9ds0_read8(SAD_G, WHO_AM_I_G);
}

int lsm9ds0_setup(void) {
    int s; 
    uint8_t r;

    if ((s = i2c_setup()) != I2C_OK) {
        return LSM9DS0_I2C;
    }

    if (lsm9ds0_write8(SAD_G, CTRL_REG1_G, 0xFF) < 0) {
        return LSM9DS0_I2C;
    }
    r = lsm9ds0_read8(SAD_G, CTRL_REG4_G);
    if (r == 0xFF) {
        return LSM9DS0_I2C;
    }
    r |= 0x30;
    if (lsm9ds0_write8(SAD_G, CTRL_REG4_G, r) < 0) {
        return LSM9DS0_I2C;
    }

    if (lsm9ds0_write8(SAD_XM, CTRL_REG7_XM, 0x00) < 0) {
        return LSM9DS0_I2C;
    }

    if (lsm9ds0_write8(SAD_XM, CTRL_REG1_XM, 0x67) < 0) {
        return LSM9DS0_I2C;
    }
    
    if (lsm9ds0_write8(SAD_XM, CTRL_REG2_XM, 0x20) < 0) {
        return LSM9DS0_I2C;
    }

    if (lsm9ds0_write8(SAD_XM, CTRL_REG5_XM, 0xF0) < 0) {
        return LSM9DS0_I2C;
    }
    if (lsm9ds0_getGyroID() != 0xD4 || lsm9ds0_getAccelID() != 0x49)
        return LSM9DS0_ID;

    return LSM9DS0_OK;
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
    int16_t heading;

    coord_t mag = lsm9ds0_getMag();
    heading = atan2(mag.y, mag.x)*180/PI + 180;
    return heading;
}

char* lsm9ds0_error(lsm9ds0Status_t s) {
    switch (s) {
        case LSM9DS0_OK:
            return "lsm9ds0: OK";
        case LSM9DS0_TIMEOUT:
            return "lsm9ds0: timeout";
        case LSM9DS0_I2C:
            return i2c_error(i2c_err);
        case LSM9DS0_ID:
            return "lsm9ds0: incorrect id";
    }
    return "lsm9ds0: unknown";
}
