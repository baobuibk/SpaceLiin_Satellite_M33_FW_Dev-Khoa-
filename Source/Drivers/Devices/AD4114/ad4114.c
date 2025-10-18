#include "ad4114.h"

/* ===== Local helpers ===== */

#define AD4114_COMMS_RW_READ   0x40u  /* Bit6 = 1 (read), 0 = write */
#define AD4114_COMMS_WEN0      0x00u  /* Bit7 = WEN (keep 0) */

static inline void cs_active(ad4114_t *d)  { do_reset(d->cs); }
static inline void cs_idle  (ad4114_t *d)  { do_set(d->cs);   }

static uint32_t wr_frame(ad4114_t *d, uint8_t ra, const uint8_t *data, size_t n)
{
    uint8_t tx[1 + 4];
    uint8_t rx[1 + 4];
    if (!d || !d->spi || !d->cs || n > 4) return (uint32_t)ERROR_INVALID_PARAM;

    tx[0] = (uint8_t)(AD4114_COMMS_WEN0 | (ra & 0x3Fu)); /* write */
    for (size_t i = 0; i < n; ++i) tx[1 + i] = data[i];

    cs_active(d);
    uint32_t st = spi_io_transfer_sync(d->spi, tx, rx, (uint32_t)(1 + n));
    cs_idle(d);
    return (st == ERROR_OK) ? (uint32_t)ERROR_OK : st;
}

static uint32_t rd_frame(ad4114_t *d, uint8_t ra, uint8_t *data, size_t n)
{
    if (!d || !d->spi || !d->cs || !data || n == 0 || n > 4) return (uint32_t)ERROR_INVALID_PARAM;

    uint8_t cmd = (uint8_t)(AD4114_COMMS_WEN0 | AD4114_COMMS_RW_READ | (ra & 0x3Fu));
    uint8_t rx;
    cs_active(d);

    uint32_t st = spi_io_transfer_sync(d->spi, &cmd, &rx, 1);
    if (st != ERROR_OK) { cs_idle(d); return st; }

    uint8_t dummy_tx[4] = {0,0,0,0};
    st = spi_io_transfer_sync(d->spi, dummy_tx, data, (uint32_t)n);

    cs_idle(d);
    return (st == ERROR_OK) ? (uint32_t)ERROR_OK : st;
}

/* Busy-wait best-effort */
static inline void short_delay(void) { __asm__ __volatile__("" ::: "memory"); }

/* ===== Core API ===== */

uint32_t ad4114_init(ad4114_t *dev, SPI_Io_t *spi, do_t *cs)
{
    if (!dev || !spi || !cs) return (uint32_t)ERROR_INVALID_PARAM;
    dev->spi = spi;
    dev->cs  = cs;
    dev->ifmode = 0;
    dev->adcmode = 0;

    cs_idle(dev);

    uint32_t rc = ad4114_sw_reset(dev);
    if (rc != (uint32_t)ERROR_OK) return rc;

    for (volatile int i=0;i<2000;i++) short_delay();

    rc = ad4114_set_ifmode(dev, (uint16_t)(dev->ifmode | AD4114_IF_DATA_STAT));
    if (rc != (uint32_t)ERROR_OK) return rc;

    rc = ad4114_set_adcmode(dev, (uint16_t)((dev->adcmode & ~AD4114_ADCMODE_MODE_MASK) | AD4114_MODE_CONTINUOUS));
    return rc;
}

uint32_t ad4114_sw_reset(ad4114_t *dev)
{
    if (!dev || !dev->spi || !dev->cs) return (uint32_t)ERROR_INVALID_PARAM;

    uint8_t tx[8]; for (int i=0;i<8;i++) tx[i]=0xFF;
    uint8_t rx[8];

    cs_active(dev);
    uint32_t st = spi_io_transfer_sync(dev->spi, tx, rx, 8);
    cs_idle(dev);
    return (st == ERROR_OK) ? (uint32_t)ERROR_OK : st;
}

uint32_t ad4114_read_id(ad4114_t *dev, uint16_t *id_out)
{
    if (!dev || !id_out) return (uint32_t)ERROR_INVALID_PARAM;
    uint8_t b[2];
    uint32_t rc = rd_frame(dev, AD4114_RA_ID, b, 2);
    if (rc != (uint32_t)ERROR_OK) return rc;
    *id_out = (uint16_t)((b[0] << 8) | b[1]);
    return (uint32_t)ERROR_OK;
}

/* ===== Raw R/W ===== */

uint32_t ad4114_write8 (ad4114_t *dev, uint8_t ra, uint8_t v) {
    if (!dev) return (uint32_t)ERROR_INVALID_PARAM;
    return wr_frame(dev, ra, &v, 1);
}
uint32_t ad4114_write16(ad4114_t *dev, uint8_t ra, uint16_t v) {
    if (!dev) return (uint32_t)ERROR_INVALID_PARAM;
    uint8_t b[2] = { (uint8_t)(v >> 8), (uint8_t)v };
    return wr_frame(dev, ra, b, 2);
}
uint32_t ad4114_write24(ad4114_t *dev, uint8_t ra, uint32_t v) {
    if (!dev) return (uint32_t)ERROR_INVALID_PARAM;
    uint8_t b[3] = { (uint8_t)(v >> 16), (uint8_t)(v >> 8), (uint8_t)v };
    return wr_frame(dev, ra, b, 3);
}
uint32_t ad4114_read8 (ad4114_t *dev, uint8_t ra, uint8_t *v) {
    if (!dev || !v) return (uint32_t)ERROR_INVALID_PARAM;
    return rd_frame(dev, ra, v, 1);
}
uint32_t ad4114_read16(ad4114_t *dev, uint8_t ra, uint16_t *v) {
    if (!dev || !v) return (uint32_t)ERROR_INVALID_PARAM;
    uint8_t b[2]; uint32_t rc = rd_frame(dev, ra, b, 2);
    if (rc != (uint32_t)ERROR_OK) return rc;
    *v = (uint16_t)((b[0] << 8) | b[1]);
    return (uint32_t)ERROR_OK;
}
uint32_t ad4114_read24(ad4114_t *dev, uint8_t ra, uint32_t *v) {
    if (!dev || !v) return (uint32_t)ERROR_INVALID_PARAM;
    uint8_t b[3]; uint32_t rc = rd_frame(dev, ra, b, 3);
    if (rc != (uint32_t)ERROR_OK) return rc;
    *v = ((uint32_t)b[0] << 16) | ((uint32_t)b[1] << 8) | b[2];
    return (uint32_t)ERROR_OK;
}

/* ===== Mode shortcuts ===== */

uint32_t ad4114_set_ifmode(ad4114_t *dev, uint16_t ifmode)
{
    if (!dev) return (uint32_t)ERROR_INVALID_PARAM;
    uint32_t rc = ad4114_write16(dev, AD4114_RA_IFMODE, ifmode);
    if (rc == (uint32_t)ERROR_OK) dev->ifmode = ifmode;
    return rc;
}

uint32_t ad4114_set_adcmode(ad4114_t *dev, uint16_t adcmode)
{
    if (!dev) return (uint32_t)ERROR_INVALID_PARAM;
    uint32_t rc = ad4114_write16(dev, AD4114_RA_ADCMODE, adcmode);
    if (rc == (uint32_t)ERROR_OK) dev->adcmode = adcmode;
    return rc;
}

/* ===== Channel config ===== */

uint32_t ad4114_config_channel(ad4114_t *dev, uint8_t ch, bool enable,
                               uint16_t input_map, uint8_t setup)
{
    if (!dev || ch >= AD4114_NUM_CHANNELS || setup >= AD4114_NUM_SETUPS)
        return (uint32_t)ERROR_INVALID_PARAM;

    uint16_t v = 0;
    if (enable) v |= AD4114_CH_EN;
    v |= ((uint16_t)setup & 0x7u) << AD4114_CH_SETUP_SHIFT;
    v |= (input_map & AD4114_CH_INPUT_MASK) << AD4114_CH_INPUT_SHIFT;
    return ad4114_write16(dev, AD4114_RA_CH(ch), v);
}

/* ===== Wait for RDY then read DATA ===== */

static uint32_t wait_rdy_low(ad4114_t *dev, uint32_t timeout_us)
{
    uint8_t st = 0x80;
    uint32_t spins = (timeout_us == 0) ? 20000u : timeout_us;
    while (spins--) {
        uint32_t rc = ad4114_read8(dev, AD4114_RA_STATUS, &st);
        if (rc != (uint32_t)ERROR_OK) return rc;
        if ((st & AD4114_STATUS_RDY) == 0) return (uint32_t)ERROR_OK;
    }
    return (uint32_t)ERROR_TIMEOUT;
}

uint32_t ad4114_read_data_wait(ad4114_t *dev, uint32_t timeout_us, uint32_t *raw24, uint8_t *status_opt)
{
    if (!dev || !raw24) return (uint32_t)ERROR_INVALID_PARAM;

    uint32_t rc = wait_rdy_low(dev, timeout_us);
    if (rc != (uint32_t)ERROR_OK) return rc;

    if (dev->ifmode & AD4114_IF_DATA_STAT) {
        uint8_t b[4];
        cs_active(dev);
        uint8_t cmd = (uint8_t)(AD4114_COMMS_WEN0 | AD4114_COMMS_RW_READ | (AD4114_RA_DATA & 0x3F));
        uint8_t rx;
        uint32_t st = spi_io_transfer_sync(dev->spi, &cmd, &rx, 1);
        if (st == ERROR_OK) st = spi_io_transfer_sync(dev->spi, (uint8_t[4]){0,0,0,0}, b, 4);
        cs_idle(dev);
        if (st != ERROR_OK) return st;

        *raw24 = ((uint32_t)b[0] << 16) | ((uint32_t)b[1] << 8) | b[2];
        if (status_opt) *status_opt = b[3];
        return (uint32_t)ERROR_OK;
    } else {
        return ad4114_read24(dev, AD4114_RA_DATA, raw24);
    }
}

uint32_t ad4114_read_channel_once(ad4114_t *dev, uint8_t ch, uint32_t timeout_us, uint32_t *raw24)
{
    if (!dev || !raw24 || ch >= AD4114_NUM_CHANNELS) return (uint32_t)ERROR_INVALID_PARAM;

    for (uint8_t i = 0; i < AD4114_NUM_CHANNELS; ++i) {
        (void)ad4114_write16(dev, AD4114_RA_CH(i), 0x0000);
    }

    uint16_t v = 0;
    uint32_t rc = ad4114_read16(dev, AD4114_RA_CH(ch), &v);
    if (rc != (uint32_t)ERROR_OK) return rc;
    v |= AD4114_CH_EN;
    rc = ad4114_write16(dev, AD4114_RA_CH(ch), v);
    if (rc != (uint32_t)ERROR_OK) return rc;

    uint16_t m = dev->adcmode;
    m &= (uint16_t)~AD4114_ADCMODE_MODE_MASK;
    m |= AD4114_MODE_SINGLE;
    rc = ad4114_set_adcmode(dev, m);
    if (rc != (uint32_t)ERROR_OK) return rc;

    rc = ad4114_read_data_wait(dev, timeout_us ? timeout_us : 50000u, raw24, NULL);
    return rc;
}

/* ===== Helper APIs ===== */

uint32_t ad4114_channel_select_inputs(ad4114_t *dev, uint8_t ch, uint8_t ainp, uint8_t ainm)
{
    if (!dev || ch >= AD4114_NUM_CHANNELS) return (uint32_t)ERROR_INVALID_PARAM;
    uint16_t v;
    uint32_t rc = ad4114_read16(dev, AD4114_RA_CH(ch), &v);
    if (rc != (uint32_t)ERROR_OK) return rc;

    v &= (uint16_t)~(AD4114_CH_INPUT_MASK << AD4114_CH_INPUT_SHIFT);
    v |= (uint16_t)(AD4114_INPUT_MAP(ainp, ainm) & AD4114_CH_INPUT_MASK);
    return ad4114_write16(dev, AD4114_RA_CH(ch), v);
}

uint32_t ad4114_channel_select_setup(ad4114_t *dev, uint8_t ch, uint8_t setup)
{
    if (!dev || ch >= AD4114_NUM_CHANNELS || setup >= AD4114_NUM_SETUPS) return (uint32_t)ERROR_INVALID_PARAM;
    uint16_t v;
    uint32_t rc = ad4114_read16(dev, AD4114_RA_CH(ch), &v);
    if (rc != (uint32_t)ERROR_OK) return rc;

    v = (uint16_t)((v & (uint16_t)~AD4114_CH_SETUP_MASK) | (((uint16_t)setup << AD4114_CH_SETUP_SHIFT) & AD4114_CH_SETUP_MASK));
    return ad4114_write16(dev, AD4114_RA_CH(ch), v);
}

uint32_t ad4114_channels_enable_mask(ad4114_t *dev, uint16_t mask)
{
    if (!dev) return (uint32_t)ERROR_INVALID_PARAM;
    for (uint8_t ch = 0; ch < AD4114_NUM_CHANNELS; ++ch) {
        uint16_t v;
        uint32_t rc = ad4114_read16(dev, AD4114_RA_CH(ch), &v);
        if (rc != (uint32_t)ERROR_OK) return rc;
        if (mask & (1u << ch)) v |= AD4114_CH_EN;
        rc = ad4114_write16(dev, AD4114_RA_CH(ch), v);
        if (rc != (uint32_t)ERROR_OK) return rc;
    }
    return (uint32_t)ERROR_OK;
}

uint32_t ad4114_channels_disable_mask(ad4114_t *dev, uint16_t mask)
{
    if (!dev) return (uint32_t)ERROR_INVALID_PARAM;
    for (uint8_t ch = 0; ch < AD4114_NUM_CHANNELS; ++ch) {
        uint16_t v;
        uint32_t rc = ad4114_read16(dev, AD4114_RA_CH(ch), &v);
        if (rc != (uint32_t)ERROR_OK) return rc;
        if (mask & (1u << ch)) v &= (uint16_t)~AD4114_CH_EN;
        rc = ad4114_write16(dev, AD4114_RA_CH(ch), v);
        if (rc != (uint32_t)ERROR_OK) return rc;
    }
    return (uint32_t)ERROR_OK;
}

uint32_t ad4114_setup_write(ad4114_t *dev, uint8_t setup, uint16_t setupcon)
{
    if (!dev || setup >= AD4114_NUM_SETUPS) return (uint32_t)ERROR_INVALID_PARAM;
    return ad4114_write16(dev, AD4114_RA_SETUPCON(setup), setupcon);
}
uint32_t ad4114_setup_read(ad4114_t *dev, uint8_t setup, uint16_t *setupcon)
{
    if (!dev || !setupcon || setup >= AD4114_NUM_SETUPS) return (uint32_t)ERROR_INVALID_PARAM;
    return ad4114_read16(dev, AD4114_RA_SETUPCON(setup), setupcon);
}
uint32_t ad4114_filt_write(ad4114_t *dev, uint8_t setup, uint16_t filtcon)
{
    if (!dev || setup >= AD4114_NUM_SETUPS) return (uint32_t)ERROR_INVALID_PARAM;
    return ad4114_write16(dev, AD4114_RA_FILTCON(setup), filtcon);
}
uint32_t ad4114_filt_read(ad4114_t *dev, uint8_t setup, uint16_t *filtcon)
{
    if (!dev || !filtcon || setup >= AD4114_NUM_SETUPS) return (uint32_t)ERROR_INVALID_PARAM;
    return ad4114_read16(dev, AD4114_RA_FILTCON(setup), filtcon);
}
uint32_t ad4114_offset_write(ad4114_t *dev, uint8_t setup, uint32_t offset24)
{
    if (!dev || setup >= AD4114_NUM_SETUPS) return (uint32_t)ERROR_INVALID_PARAM;
    return ad4114_write24(dev, AD4114_RA_OFFSET(setup), offset24 & 0xFFFFFFu);
}
uint32_t ad4114_offset_read(ad4114_t *dev, uint8_t setup, uint32_t *offset24)
{
    if (!dev || !offset24 || setup >= AD4114_NUM_SETUPS) return (uint32_t)ERROR_INVALID_PARAM;
    return ad4114_read24(dev, AD4114_RA_OFFSET(setup), offset24);
}
uint32_t ad4114_gain_write(ad4114_t *dev, uint8_t setup, uint32_t gain24)
{
    if (!dev || setup >= AD4114_NUM_SETUPS) return (uint32_t)ERROR_INVALID_PARAM;
    return ad4114_write24(dev, AD4114_RA_GAIN(setup), gain24 & 0xFFFFFFu);
}
uint32_t ad4114_gain_read(ad4114_t *dev, uint8_t setup, uint32_t *gain24)
{
    if (!dev || !gain24 || setup >= AD4114_NUM_SETUPS) return (uint32_t)ERROR_INVALID_PARAM;
    return ad4114_read24(dev, AD4114_RA_GAIN(setup), gain24);
}

/* 5) Shortcuts chế độ chuyển đổi */
uint32_t ad4114_set_mode_continuous(ad4114_t *dev, bool ref_en)
{
    if (!dev) return (uint32_t)ERROR_INVALID_PARAM;
    uint16_t m = dev->adcmode;
    m &= (uint16_t)~AD4114_ADCMODE_MODE_MASK;
    m |= AD4114_MODE_CONTINUOUS;
    if (ref_en) m |= AD4114_ADCMODE_REF_EN; else m &= (uint16_t)~AD4114_ADCMODE_REF_EN;
    return ad4114_set_adcmode(dev, m);
}

uint32_t ad4114_set_mode_single(ad4114_t *dev, bool single_cycle, bool ref_en)
{
    if (!dev) return (uint32_t)ERROR_INVALID_PARAM;
    uint16_t m = dev->adcmode;
    m &= (uint16_t)~AD4114_ADCMODE_MODE_MASK;
    m |= AD4114_MODE_SINGLE;
    if (single_cycle) m |= AD4114_ADCMODE_SING_CYC; else m &= (uint16_t)~AD4114_ADCMODE_SING_CYC;
    if (ref_en) m |= AD4114_ADCMODE_REF_EN; else m &= (uint16_t)~AD4114_ADCMODE_REF_EN;
    return ad4114_set_adcmode(dev, m);
}

/* 6) Đọc 1 mẫu cho tất cả kênh đang enable */
uint32_t ad4114_read_all(ad4114_t *dev,
                         uint32_t timeout_us_per_sample,
                         uint16_t *out_ch_mask,
                         uint32_t samples[AD4114_NUM_CHANNELS])
{
    if (!dev || !samples) return (uint32_t)ERROR_INVALID_PARAM;

    /* 1) Xác định các kênh đang enable */
    uint16_t enabled_mask = 0;
    for (uint8_t ch = 0; ch < AD4114_NUM_CHANNELS; ++ch) {
        uint16_t v = 0;
        uint32_t rc = ad4114_read16(dev, AD4114_RA_CH(ch), &v);
        if (rc != (uint32_t)ERROR_OK) return rc;
        if (v & AD4114_CH_EN) enabled_mask |= (uint16_t)(1u << ch);
    }
    if (enabled_mask == 0) return (uint32_t)ERROR_NOT_READY;
    if (out_ch_mask) *out_ch_mask = 0;

    /* 2) Bật tạm DATA_STAT nếu chưa bật */
    const bool had_data_stat = (dev->ifmode & AD4114_IF_DATA_STAT) != 0;
    if (!had_data_stat) {
        uint32_t rc = ad4114_set_ifmode(dev, (uint16_t)(dev->ifmode | AD4114_IF_DATA_STAT));
        if (rc != (uint32_t)ERROR_OK) return rc;
    }

    /* 3) Chuyển continuous mode (nếu chưa) */
    uint16_t adcmode_before = dev->adcmode;
    uint16_t m = (uint16_t)((adcmode_before & (uint16_t)~AD4114_ADCMODE_MODE_MASK) | AD4114_MODE_CONTINUOUS);
    uint32_t rc = ad4114_set_adcmode(dev, m);
    if (rc != (uint32_t)ERROR_OK) goto restore_modes;

    /* 4) Đọc vòng: cần N mẫu với N = số kênh enable */
    uint8_t  seen[AD4114_NUM_CHANNELS] = {0};
    uint16_t remaining = 0;
    for (uint8_t ch = 0; ch < AD4114_NUM_CHANNELS; ++ch)
        if (enabled_mask & (uint16_t)(1u << ch)) remaining++;

    for (uint8_t ch = 0; ch < AD4114_NUM_CHANNELS; ++ch)
        if (enabled_mask & (uint16_t)(1u << ch)) samples[ch] = 0;

    while (remaining) {
        uint8_t status = 0;
        uint32_t raw   = 0;
        rc = ad4114_read_data_wait(dev, timeout_us_per_sample, &raw, &status);
        if (rc != (uint32_t)ERROR_OK) { rc = (uint32_t)ERROR_TIMEOUT; goto restore_modes; }

        uint8_t ch = (uint8_t)(status & AD4114_STATUS_CH_MASK);
        if (ch >= AD4114_NUM_CHANNELS) continue;

        if ((enabled_mask & (uint16_t)(1u << ch)) && !seen[ch]) {
            samples[ch] = raw;
            seen[ch]    = 1;
            remaining--;
            if (out_ch_mask) *out_ch_mask |= (uint16_t)(1u << ch);
        }
    }
    rc = (uint32_t)ERROR_OK;

restore_modes:
    if (!had_data_stat) {
        (void)ad4114_set_ifmode(dev, (uint16_t)(dev->ifmode & (uint16_t)~AD4114_IF_DATA_STAT));
    }
    if (dev->adcmode != adcmode_before) {
        (void)ad4114_set_adcmode(dev, adcmode_before);
    }
    return rc;
}
