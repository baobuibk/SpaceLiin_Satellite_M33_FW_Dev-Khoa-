#include "adg1414.h"
#include <string.h>

/* Ghi toàn bộ chuỗi: đẩy byte từ cuối về đầu, giữ CS ở mức tích cực trong suốt quá trình */
static int adg1414_chain_write(ADG1414_Device_t *dev)
{
    if (!dev || !dev->spi) return (int)ERROR_INVALID_PARAM;

    /* Chuẩn bị buffer theo thứ tự i = num-1 .. 0 giống mã gốc */
    uint8_t tx[ADG1414_CHAIN_NUM_CHIPS_MAX];
    uint8_t rx[ADG1414_CHAIN_NUM_CHIPS_MAX];
    if (dev->num_of_sw > ADG1414_CHAIN_NUM_CHIPS_MAX) return (int)ERROR_INVALID_PARAM;

    for (int i = 0; i < dev->num_of_sw; i++) {
        tx[i] = dev->switch_state[dev->num_of_sw - 1 - i];
    }

    /* CS active (low) -> shift -> CS inactive (high)
       Nếu PCB của bạn dùng CS-active-high thì đảo do_set/do_reset tương ứng. */
    do_reset(&dev->cs);

    uint32_t st = spi_io_transfer_sync(dev->spi, tx, rx, dev->num_of_sw);
    do_set(&dev->cs);

    return (st == ERROR_OK) ? (int)ERROR_OK : (int)st;
}

int ADG1414_Chain_Flush(ADG1414_Device_t *dev)
{
    return adg1414_chain_write(dev);
}

int ADG1414_Chain_Init(ADG1414_Device_t *dev, SPI_Io_t *spi, const do_t *cs, uint8_t num)
{
    if (!dev || !spi || !cs) return (int)ERROR_INVALID_PARAM;
    if (num == 0 || num > ADG1414_CHAIN_NUM_CHIPS_MAX) return (int)ERROR_INVALID_PARAM;

    dev->spi = spi;
    dev->cs  = *cs;          /* copy port/pin CS */
    dev->num_of_sw = num;
    memset(dev->switch_state, 0x00, sizeof(dev->switch_state));

    /* CS idle = high (thường gặp). Nếu phần cứng của bạn cần idle low thì đổi lại. */
    do_set(&dev->cs);

    /* Đẩy giá trị mặc định (tất cả off) xuống phần cứng */
    return adg1414_chain_write(dev);
}

int ADG1414_Chain_SwitchAllOff(ADG1414_Device_t *dev)
{
    if (!dev) return (int)ERROR_INVALID_PARAM;
    for (int i = 0; i < dev->num_of_sw; i++) {
        dev->switch_state[i] = 0x00;
    }
    return adg1414_chain_write(dev);
}

int ADG1414_Chain_SwitchOn(ADG1414_Device_t *dev, uint8_t channel_num)
{
    if (!dev) return (int)ERROR_INVALID_PARAM;

    /* INTERNAL mode (6 bytes => 36 channel, mỗi byte dùng 6 bit thấp) */
    if (dev->num_of_sw == INTERNAL_CHAIN_SWITCH_NUM) {
        if (channel_num > INTERNAL_CHAIN_CHANNEL_NUM) return (int)ERROR_INVALID_PARAM;

        /* Yêu cầu logic cũ: 0 => all off; ngược lại chỉ bật đúng 1 kênh */
        for (int i = 0; i < dev->num_of_sw; i++) dev->switch_state[i] = 0x00;

        if (channel_num) {
            uint8_t chip_idx = (uint8_t)((channel_num - 1) / 6);
            uint8_t bit_idx  = (uint8_t)((channel_num - 1) % 6);
            dev->switch_state[chip_idx] = (uint8_t)((1u << bit_idx) & 0x3Fu);
        }
    }
    /* EXTERNAL mode (1 byte => 8 kênh) */
    else if (dev->num_of_sw == EXTERNAL_CHAIN_SWITCH_NUM) {
        if (channel_num > EXTERNAL_CHAIN_CHANNEL_NUM) return (int)ERROR_INVALID_PARAM;
        if (channel_num) dev->switch_state[0] = (uint8_t)(1u << (channel_num - 1));
        else             dev->switch_state[0] = 0x00;
    }
    /* Tùy biến khác: coi như mỗi byte là 8 kênh đầy đủ (0..7) – chỉ bật duy nhất channel_num? */
    else {
        /* Mặc định bật đúng 1 kênh (nếu channel_num > 0) */
        for (int i = 0; i < dev->num_of_sw; i++) dev->switch_state[i] = 0x00;
        if (channel_num) {
            uint8_t chip_idx = (uint8_t)((channel_num - 1) / 8);
            uint8_t bit_idx  = (uint8_t)((channel_num - 1) % 8);
            if (chip_idx >= dev->num_of_sw) return (int)ERROR_INVALID_PARAM;
            dev->switch_state[chip_idx] = (uint8_t)(1u << bit_idx);
        }
    }

    return adg1414_chain_write(dev);
}

int ADG1414_Chain_SwitchOff(ADG1414_Device_t *dev, uint8_t channel_num)
{
    if (!dev || channel_num == 0) return (int)ERROR_INVALID_PARAM;

    if (dev->num_of_sw == INTERNAL_CHAIN_SWITCH_NUM) {
        if (channel_num > INTERNAL_CHAIN_CHANNEL_NUM) return (int)ERROR_INVALID_PARAM;
        uint8_t chip_idx = (uint8_t)((channel_num - 1) / 6);
        uint8_t bit_idx  = (uint8_t)((channel_num - 1) % 6);
        dev->switch_state[chip_idx] = (uint8_t)(dev->switch_state[chip_idx] & (uint8_t)~(1u << bit_idx));
    }
    else if (dev->num_of_sw == EXTERNAL_CHAIN_SWITCH_NUM) {
        if (channel_num > EXTERNAL_CHAIN_CHANNEL_NUM) return (int)ERROR_INVALID_PARAM;
        dev->switch_state[0] = (uint8_t)(dev->switch_state[0] & (uint8_t)~(1u << (channel_num - 1)));
    }
    else {
        uint8_t chip_idx = (uint8_t)((channel_num - 1) / 8);
        uint8_t bit_idx  = (uint8_t)((channel_num - 1) % 8);
        if (chip_idx >= dev->num_of_sw) return (int)ERROR_INVALID_PARAM;
        dev->switch_state[chip_idx] = (uint8_t)(dev->switch_state[chip_idx] & (uint8_t)~(1u << bit_idx));
    }

    return adg1414_chain_write(dev);
}
