#ifndef _SP_I2C_API_H_
#define _SP_I2C_API_H_

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Include ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include <stdint.h>
#include <stdbool.h>

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/**
 * Forward declaration, go to respective port file to
 * read definition
 */
typedef struct _sp_i2c_t_ 	 	sp_i2c_t;
typedef struct _sp_i2c_init_t_  _sp_i2c_init_t_;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
void SP_I2C_master_blocking_init(sp_i2c_t* p_i2c, const _sp_i2c_init_t_* p_init);

int SP_I2C_master_write_blocking(sp_i2c_t* p_i2c, uint8_t dev_addr, uint8_t reg_addr, uint8_t data);
int SP_I2C_master_read_blocking(sp_i2c_t* p_i2c, uint8_t dev_addr, uint8_t reg_addr, uint8_t* p_data);
int SP_I2C_master_read_multi_blocking(sp_i2c_t* p_i2c, uint8_t dev_addr, uint8_t reg_addr, uint8_t* p_data, uint32_t size);

#endif /* _SP_I2C_API_H_ */