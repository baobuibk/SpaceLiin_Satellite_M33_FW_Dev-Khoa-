/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Include~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include "sp_i2c_port.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define I2C_BLOCKING_TIMEOUT 5000
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Private Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
void SP_I2C_master_blocking_init(sp_i2c_t* p_i2c, const _sp_i2c_init_t_* p_init)
{
    p_i2c->p_base = p_init->p_base;
}

int SP_I2C_master_write_blocking(sp_i2c_t* p_i2c, uint8_t dev_addr, uint8_t reg_addr, uint8_t data)
{
    uint32_t timeout = I2C_BLOCKING_TIMEOUT;
    size_t   txCount = 0xFFU;
    status_t ret_value = kStatus_Fail;

    /* Send master blocking data to slave */
    if (kStatus_Success == LPI2C_MasterStart(p_i2c->p_base, dev_addr, kLPI2C_Write))
    {
        /* Check master tx FIFO empty or not */
        LPI2C_MasterGetFifoCounts(p_i2c->p_base, NULL, &txCount);
        while (txCount)
        {
            LPI2C_MasterGetFifoCounts(p_i2c->p_base, NULL, &txCount);

            timeout--;
            if (timeout == 0)
            {
                return kStatus_LPI2C_Timeout;
            }
        }

        /* Check communicate with slave successful or not */
        if (LPI2C_MasterGetStatusFlags(p_i2c->p_base) & kLPI2C_MasterNackDetectFlag)
        {
            return kStatus_LPI2C_Nak;
        }

        /* subAddress = 0x01, data = g_master_txBuff - write to slave.
          start + slaveaddress(w) + subAddress + length of data buffer + data buffer + stop*/
        ret_value = LPI2C_MasterSend(p_i2c->p_base, &reg_addr, 1);
        if (ret_value != kStatus_Success)
        {
            if (ret_value == kStatus_LPI2C_Nak)
            {
                LPI2C_MasterStop(p_i2c->p_base);
            }
            return -1;
        }

        ret_value = LPI2C_MasterSend(p_i2c->p_base, &data, 1);
        if (ret_value != kStatus_Success)
        {
            if (ret_value == kStatus_LPI2C_Nak)
            {
                LPI2C_MasterStop(p_i2c->p_base);
            }
            return -1;
        }

        ret_value = LPI2C_MasterStop(p_i2c->p_base);
        if (ret_value != kStatus_Success)
        {
            return -1;
        }
    }

    return kStatus_Success;
}

// int SP_I2C_master_write_multi_blocking(sp_i2c_t* p_i2c, uint8_t dev_addr, uint8_t reg_addr, uint8_t* p_data, uint32_t size)
// {
//     uint32_t timeout = I2C_BLOCKING_TIMEOUT;
//     size_t txCount = 0xFFU;
//     status_t ret_value = kStatus_Fail;

//     /* Send master blocking data to slave */
//     if (kStatus_Success == LPI2C_MasterStart(p_i2c->p_base, dev_addr, kLPI2C_Write))
//     {
//         /* Check master tx FIFO empty or not */
//         LPI2C_MasterGetFifoCounts(p_i2c->p_base, NULL, &txCount);
//         while (txCount)
//         {
//             LPI2C_MasterGetFifoCounts(p_i2c->p_base, NULL, &txCount);

//             timeout--;
//             if (timeout == 0)
//             {
//                 return kStatus_LPI2C_Timeout;
//             }
//         }

//         /* Check communicate with slave successful or not */
//         if (LPI2C_MasterGetStatusFlags(p_i2c->p_base) & kLPI2C_MasterNackDetectFlag)
//         {
//             return kStatus_LPI2C_Nak;
//         }

//         /* subAddress = 0x01, data = g_master_txBuff - write to slave.
//           start + slaveaddress(w) + subAddress + length of data buffer + data buffer + stop*/
//         ret_value = LPI2C_MasterSend(p_i2c->p_base, &reg_addr, 1);
//         if (ret_value != kStatus_Success)
//         {
//             if (ret_value == kStatus_LPI2C_Nak)
//             {
//                 LPI2C_MasterStop(p_i2c->p_base);
//             }
//             return -1;
//         }

//         ret_value = LPI2C_MasterSend(p_i2c->p_base, p_data, size);
//         if (ret_value != kStatus_Success)
//         {
//             if (ret_value == kStatus_LPI2C_Nak)
//             {
//                 LPI2C_MasterStop(p_i2c->p_base);
//             }
//             return -1;
//         }

//         ret_value = LPI2C_MasterStop(p_i2c->p_base);
//         if (ret_value != kStatus_Success)
//         {
//             return -1;
//         }
//     }
// }

int SP_I2C_master_read_blocking(sp_i2c_t* p_i2c, uint8_t dev_addr, uint8_t reg_addr, uint8_t* p_data)
{
    uint32_t timeout = I2C_BLOCKING_TIMEOUT;
    size_t   txCount = 0xFFU;
    status_t ret_value = kStatus_Fail;

    /* Receive blocking data from slave */
    /* subAddress = 0x01, data = g_master_rxBuff - read from slave.
      start + slaveaddress(w) + subAddress + repeated start + slaveaddress(r) + rx data buffer + stop */
    if (kStatus_Success == LPI2C_MasterStart(p_i2c->p_base, dev_addr, kLPI2C_Write))
    {
        /* Check master tx FIFO empty or not */
        LPI2C_MasterGetFifoCounts(p_i2c->p_base, NULL, &txCount);
        while (txCount)
        {
            LPI2C_MasterGetFifoCounts(p_i2c->p_base, NULL, &txCount);

            timeout--;
            if (timeout == 0)
            {
                return kStatus_LPI2C_Timeout;
            }
        }

        /* Check communicate with slave successful or not */
        if (LPI2C_MasterGetStatusFlags(p_i2c->p_base) & kLPI2C_MasterNackDetectFlag)
        {
            return kStatus_LPI2C_Nak;
        }

        ret_value = LPI2C_MasterSend(p_i2c->p_base, &reg_addr, 1);
        if (ret_value != kStatus_Success)
        {
            if (ret_value == kStatus_LPI2C_Nak)
            {
                LPI2C_MasterStop(p_i2c->p_base);
            }
            return -1;
        }

        ret_value = LPI2C_MasterRepeatedStart(p_i2c->p_base, dev_addr, kLPI2C_Read);
        if (ret_value != kStatus_Success)
        {
            return -1;
        }

        ret_value = LPI2C_MasterReceive(p_i2c->p_base, p_data, 1);
        if (ret_value != kStatus_Success)
        {
            if (ret_value == kStatus_LPI2C_Nak)
            {
                LPI2C_MasterStop(p_i2c->p_base);
            }
            return -1;
        }

        ret_value = LPI2C_MasterStop(p_i2c->p_base);
        if (ret_value != kStatus_Success)
        {
            return -1;
        }
    }

    return kStatus_Success;
}

int SP_I2C_master_read_multi_blocking(sp_i2c_t* p_i2c, uint8_t dev_addr, uint8_t reg_addr, uint8_t* p_data, uint32_t size)
{
    uint32_t timeout = I2C_BLOCKING_TIMEOUT;
    size_t   txCount = 0xFFU;
    status_t ret_value = kStatus_Fail;

    /* Receive blocking data from slave */
    /* subAddress = 0x01, data = g_master_rxBuff - read from slave.
      start + slaveaddress(w) + subAddress + repeated start + slaveaddress(r) + rx data buffer + stop */
    if (kStatus_Success == LPI2C_MasterStart(p_i2c->p_base, dev_addr, kLPI2C_Write))
    {
        /* Check master tx FIFO empty or not */
        LPI2C_MasterGetFifoCounts(p_i2c->p_base, NULL, &txCount);
        while (txCount)
        {
            LPI2C_MasterGetFifoCounts(p_i2c->p_base, NULL, &txCount);

            timeout--;
            if (timeout == 0)
            {
                return kStatus_LPI2C_Timeout;
            }
        }

        /* Check communicate with slave successful or not */
        if (LPI2C_MasterGetStatusFlags(p_i2c->p_base) & kLPI2C_MasterNackDetectFlag)
        {
            return kStatus_LPI2C_Nak;
        }

        ret_value = LPI2C_MasterSend(p_i2c->p_base, &reg_addr, 1);
        if (ret_value != kStatus_Success)
        {
            if (ret_value == kStatus_LPI2C_Nak)
            {
                LPI2C_MasterStop(p_i2c->p_base);
            }
            return -1;
        }

        ret_value = LPI2C_MasterRepeatedStart(p_i2c->p_base, dev_addr, kLPI2C_Read);
        if (ret_value != kStatus_Success)
        {
            return -1;
        }

        ret_value = LPI2C_MasterReceive(p_i2c->p_base, p_data, size);
        if (ret_value != kStatus_Success)
        {
            if (ret_value == kStatus_LPI2C_Nak)
            {
                LPI2C_MasterStop(p_i2c->p_base);
            }
            return -1;
        }

        ret_value = LPI2C_MasterStop(p_i2c->p_base);
        if (ret_value != kStatus_Success)
        {
            return -1;
        }
    }

    return kStatus_Success;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ End of the program ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */